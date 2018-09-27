/*******************************************************************************
* This file is part of Tihu.
*
* Tihu is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Tihu is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Tihu.  If not, see <http://www.gnu.org/licenses/>.
*
* Contributor(s):
*    Mostafa Sedaghat Joo (mostafa.sedaghat@gmail.com)
*
*******************************************************************************/

#include "phonetics/tihu_dict/tihu_dict.h"
#include "phonetics/letter_to_sound/letter_to_sound.h"
#include "synthesis/mbrola/mbrola_syn.h"
#include "synthesis/espeak/espeak_syn.h"
#include "hazm/hazm.h"
#include "engine.h"
#include "tihu.h"

#include <iostream>
#include <locale>
#include <dlfcn.h>


CEngine::CEngine()
{
    Hazm          = nullptr;
    TihuDict      = nullptr;
    LetterToSound = nullptr;
    Synthesizer   = nullptr;
    Settings      = new CSettings();
}

CEngine::~CEngine()
{
    if(Hazm) {
        delete Hazm;
    }
    if(TihuDict) {
        delete TihuDict;
    }
    if(LetterToSound) {
        delete LetterToSound;
    }
    if(Synthesizer) {
        delete Synthesizer;
    }
    delete Settings;
}

int CEngine::LoadModules()
{
    /// change current directory
    std::string dir = GetCurrentModulePath();
    int res = chdir(dir.c_str());
    if (res != 0) {
        std::cout << "Can not change current directory to " << dir << std::endl;
    }

    Hazm = new CHazm();
    TihuDict = new CTihuDict();
    LetterToSound = new CLetterToSound();

    if(!Hazm->Load("Hazm")) {
        return TIHU_ERROR_LOAD_DATA;
    }

    if(!TihuDict->Load("Dict")) {
        return TIHU_ERROR_LOAD_DATA;
    }

    if(!LetterToSound->Load("LTS")) {
        return TIHU_ERROR_LOAD_DATA;
    }

    Hazm->SetSettings(Settings);
    TihuDict->SetSettings(Settings);
    LetterToSound->SetSettings(Settings);

    return TIHU_ERROR_NONE;
}

int CEngine::LoadSynthesizer(TIHU_VOICE voice)
{
    if(Synthesizer) {
        delete Synthesizer;
    }

    std::string voice_name;

    switch(voice) {
    case TIHU_VOICE_MBROLA_MALE: {
        Synthesizer = new CMbrolaSyn();
        voice_name = "mbrola/ir1";
    }
    break;

    case TIHU_VOICE_MBROLA_FEMALE: {
        Synthesizer = new CMbrolaSyn();
        voice_name = "mbrola/ir2";
    }
    break;

    case TIHU_VOICE_ESPEAK_MALE: {
        Synthesizer = new CeSpeakSyn();
        voice_name = "eSpeak/male";
    }
    break;

    case TIHU_VOICE_ESPEAK_FEMALE: {
        Synthesizer = new CeSpeakSyn();
        voice_name = "eSpeak/female";
    }
    break;
    }

    if(!Synthesizer->Load(voice_name)) {
        return TIHU_ERROR_LOAD_VOICE;
    }

    Synthesizer->SetSettings(Settings);

    return TIHU_ERROR_NONE;
}

void CEngine::SetCallback(TIHU_CALLBACK callback, void* userData)
{
    (TihuDict) ? TihuDict->SetCallBack((void*)callback, userData) : void();
    (LetterToSound) ? LetterToSound->SetCallBack((void*)callback, userData) : void();
    (Synthesizer) ? Synthesizer->SetCallBack((void*)callback, userData) : void();
}

void CEngine::Stop()
{
    if (Synthesizer) {
        Synthesizer->Stop();
    }
}

void CEngine::Speak(const std::string &text)
{
    CCorpus corpus(text);

    Hazm->ParsText(&corpus);
    TihuDict->ParsText(&corpus);
    LetterToSound->ParsText(&corpus);
    Synthesizer->ParsText(&corpus);
}

void CEngine::Diacritize(const std::string &text)
{

}

bool CEngine::SetParam(TIHU_PARAM param, int value)
{
    bool applied = true;

    switch(param) {
    case TIHU_PARAM_PITCH:
        Settings->SetPitch(value);
        Synthesizer->ApplyChanges();
        break;
    case TIHU_PARAM_VOLUME:
        Settings->SetVolume(value);
        Synthesizer->ApplyChanges();
        break;
    case TIHU_PARAM_RATE:
        Settings->SetRate(value);
        Synthesizer->ApplyChanges();
        break;
    case TIHU_PARAM_READ_PUNCS:
        // TODO
        break;
    case TIHU_PARAM_FREQUENCY:
        applied = false;
        break;
    }

    return applied;
}

bool CEngine::GetParam(TIHU_PARAM param, int &value)
{
    value = -1;

    switch(param) {
    case TIHU_PARAM_PITCH:
        value = Settings->GetPitch();
        break;
    case TIHU_PARAM_VOLUME:
        value = Settings->GetVolume();
        break;
    case TIHU_PARAM_RATE:
        value = Settings->GetRate();
        break;
    case TIHU_PARAM_FREQUENCY:
        value = Synthesizer->GetFrequency();
        break;
    case TIHU_PARAM_READ_PUNCS:
        // TODO
        break;
    default:
        return false;
    }

    return true;
}

void CEngine::LogText(const std::string& text) const
{
    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    std::ofstream writer("text.txt", std::ofstream::out | std::ofstream::app);

    if(!writer.is_open()) {
        return;
    }

    writer.imbue(utf8_locale);

    writer << std::endl;
    writer << std::endl;
    writer << text;

    writer.close();
}

std::string CEngine::GetCurrentModulePath() const
{
    std::string path;

#ifdef WIN32
    char buffer[_MAX_PATH];
    buffer[0] = 0;

    HMODULE module = NULL;
    if(!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                           GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           "tihu.dll",
                           &module)) {
        int ret = GetLastError();
        fprintf(stderr, "GetModuleHandle returned %d\n", ret);
        return path;
    }

    if(!GetModuleFileNameA(module, buffer, _MAX_PATH)) {
        return path;
    }

    path = buffer;
#else
    Dl_info dl_info;
    if(!dladdr((void*)tihu_Init, &dl_info)) {
        return path;
    }

    path = dl_info.dli_fname;
#endif

    size_t found=path.find_last_of("/\\");

    return path.substr(0,found+1);
}