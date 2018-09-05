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

#include "ipa_transcript/tihu_dict/tihu_dict.h"
#include "ipa_transcript/letter_to_sound/letter_to_sound.h"
#include "synthesis/mbrola/mbrola_syn.h"
#include "synthesis/espeak/espeak_syn.h"
#include "hazm/hazm.h"
#include "text_tagger.h"
#include "engine.h"
#include "tihu.h"

#include <iostream>
#include <locale>



#define LOG_ENABLED


CEngine::CEngine()
{
    Hazm            = nullptr;
    TihuDict        = nullptr;
    LetterToSound   = nullptr;
    Synthesizer     = nullptr;
    TextTagger      = nullptr;
    Corpus          = new CCorpus();
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
    if(TextTagger) {
        delete TextTagger;
    }
    delete Corpus;
}

int CEngine::LoadModules()
{
    Hazm = new CHazm();
    TihuDict = new CTihuDict();
    LetterToSound = new CLetterToSound();
    TextTagger = new CTextTagger();

    if (!static_cast<CHazm*>(Hazm)->Load("Hazm")) {
        return TIHU_ERROR_LOAD_DATA;
    }

    if(!static_cast<CTihuDict*>(TihuDict)->Load("Dict")) {
        return TIHU_ERROR_LOAD_DATA;
    }

    if(!static_cast<CLetterToSound*>(LetterToSound)->Load("LTS")) {
        return TIHU_ERROR_LOAD_DATA;
    }

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

    return TIHU_ERROR_NONE;
}

void CEngine::SetCallback(TIHU_CALLBACK callback, void* userData)
{
    (TihuDict) ? TihuDict->SetCallBack(callback, userData) : void();
    (LetterToSound) ? LetterToSound->SetCallBack(callback, userData) : void();
    (Synthesizer) ? Synthesizer->SetCallBack(callback, userData) : void();
    (TextTagger) ? TextTagger->SetCallBack(callback, userData) : void();
}

void CEngine::Stop()
{
    if (Synthesizer) {
        Synthesizer->Stop();
    }
}

void CEngine::Speak(const std::string &text)
{
    Corpus->Clear();
    Corpus->SetText(text);

    Hazm->ParsText(Corpus);
    TihuDict->ParsText(Corpus);
    LetterToSound->ParsText(Corpus);
    Synthesizer->ParsText(Corpus);

#ifdef LOG_ENABLED
    LogText("./log/text.txt");
    LogCorpus("./log/text.lbl");
    LogCorpus("./log/text.xml");
#endif
}

void CEngine::Diacritize(const std::string &text)
{

}

void CEngine::AutoTag(const std::string &text)
{

}

bool CEngine::SetParam(TIHU_PARAM param, int value)
{
    bool applied = true;

    switch(param) {
    case TIHU_PARAM_PITCH:
        Synthesizer->ApplyPitch(value);
        break;
    case TIHU_PARAM_VOLUME:
        Synthesizer->ApplyVolume(value);
        break;
    case TIHU_PARAM_RATE:
        Synthesizer->ApplyRate(value);
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
        //value = Synthesizer->GetPitch();
        break;
    case TIHU_PARAM_VOLUME:
        Synthesizer->ApplyVolume(value);
        break;
    case TIHU_PARAM_RATE:
        Synthesizer->ApplyRate(value);
        break;
    case TIHU_PARAM_FREQUENCY:
        value = Synthesizer->GetFrequency();
        break;
    }

    return true;
}

void CEngine::SetText(const std::string &text) const
{
    Corpus->Clear();
    Corpus->SetText(text);

#ifdef LOG_ENABLED
    LogText("text.txt");
#endif
}

void CEngine::Process() const
{


}

void CEngine::LogText(const std::string& filename) const
{
    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    std::ofstream writer(filename);

    if(!writer.is_open()) {
        return;
    }

    writer.imbue(utf8_locale);

    writer << std::endl;
    writer << std::endl;
    writer << Corpus->GetText();

    writer.close();
}

void CEngine::LogCorpus(const std::string& filename) const
{
    Corpus->Dump(filename);
}
