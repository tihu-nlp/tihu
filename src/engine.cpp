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

#include "engine.h"
#include "hazm/hazm.h"
#include "phonetics/word_to_phonetic.h"
#include "synthesis/espeak/espeak_syn.h"
#include "synthesis/mbrola/mbrola_syn.h"
#include "tihu.h"
#include "tihu_dict/tihu_dict.h"
#include "tokenizer/tokenizer.h"

#include <dlfcn.h>
#include <iostream>
#include <locale>
#include <sstream>

CEngine::CEngine() {
    Hazm = nullptr;
    TihuDict = nullptr;
    Phonetics = nullptr;
    Tokenizer = nullptr;
    for (int i = 0; i < TIHU_VOICE_COUNT; ++i) {
        Synthesizers[i] = nullptr;
    }
    Settings = new CSettings();
}

CEngine::~CEngine() {
    if (Hazm) {
        delete Hazm;
    }
    if (TihuDict) {
        delete TihuDict;
    }
    if (Phonetics) {
        delete Phonetics;
    }
    if (Tokenizer) {
        delete Tokenizer;
    }
    for (int i = 0; i < TIHU_VOICE_COUNT; ++i) {
        if (Synthesizers[i]) {
            delete Synthesizers[i];
        }
    }
    delete Settings;
}

int CEngine::LoadModules() {
    /// change current directory
    std::string dir = GetCurrentModulePath();
    int res = chdir(dir.c_str());
    if (res != 0) {
        std::cout << "Can not change current directory to " << dir << std::endl;
    }

    Hazm = new CHazm();
    TihuDict = new CTihuDict();
    Phonetics = new CWordToPhonetic();
    Tokenizer = new CTokenizer();

    if (!Hazm->Load()) {
        return TIHU_ERR_LOADING;
    }
    if (!TihuDict->Load()) {
        return TIHU_ERR_LOADING;
    }
    if (!Phonetics->Load()) {
        return TIHU_ERR_LOADING;
    }
    if (!Tokenizer->Load()) {
        return TIHU_ERR_LOADING;
    }

    Hazm->SetSettings(Settings);
    TihuDict->SetSettings(Settings);
    Phonetics->SetSettings(Settings);
    Tokenizer->SetSettings(Settings);

    return LoadSynthesizers();
}

int CEngine::LoadSynthesizers() {
    Synthesizers[TIHU_VOICE_MBROLA_MALE] = new CMbrolaSyn();
    Synthesizers[TIHU_VOICE_MBROLA_FEMALE] = new CMbrolaSyn();
    Synthesizers[TIHU_VOICE_ESPEAK_MALE] = new CeSpeakSyn();
    Synthesizers[TIHU_VOICE_ESPEAK_FEMALE] = new CeSpeakSyn();

    for (int i = 0; i < TIHU_VOICE_COUNT; ++i) {
        Synthesizers[i]->SetSettings(Settings);
    }

    if (!Synthesizers[TIHU_VOICE_MBROLA_MALE]->Load("mbrola/ir1")) {
        return TIHU_ERR_LOADING;
    }

    if (!Synthesizers[TIHU_VOICE_MBROLA_FEMALE]->Load("mbrola/ir2")) {
        return TIHU_ERR_LOADING;
    }

    if (!Synthesizers[TIHU_VOICE_ESPEAK_MALE]->Load("eSpeak/male")) {
        return TIHU_ERR_LOADING;
    }

    if (!Synthesizers[TIHU_VOICE_ESPEAK_FEMALE]->Load("eSpeak/female")) {
        return TIHU_ERR_LOADING;
    }

    return TIHU_ERR_NONE;
}

void CEngine::SetCallback(TIHU_CALLBACK callback, void *userData) {
    Callback = callback;
    UserData = userData;

    IParser::callback_t fn = [&](int t, long l, long w) {
        return Callback((TIHU_CALLBACK_MESSAGE)t, l, w, UserData);
    };

    (Tokenizer) ? Tokenizer->SetCallback(fn) : void();
    (TihuDict) ? TihuDict->SetCallback(fn) : void();
    (Phonetics) ? Phonetics->SetCallback(fn) : void();
    (Hazm) ? Hazm->SetCallback(fn) : void();

    for (int i = 0; i < TIHU_VOICE_COUNT; ++i) {
        Synthesizers[i]->SetCallback(fn);
    }
}

void CEngine::Stop() {
    Synthesizers[TIHU_VOICE_MBROLA_MALE]->Stop();
    Synthesizers[TIHU_VOICE_MBROLA_FEMALE]->Stop();
    Synthesizers[TIHU_VOICE_ESPEAK_MALE]->Stop();
    Synthesizers[TIHU_VOICE_ESPEAK_FEMALE]->Stop();
}

void CEngine::Tag(const std::string &text) {
    std::list<IParser *> parsers;
    parsers.push_back(Tokenizer);
    parsers.push_back(TihuDict);
    parsers.push_back(Hazm);
    parsers.push_back(Phonetics);

    ParsText(text, parsers, true);
}

void CEngine::Speak(const std::string &text, TIHU_VOICE voice) {
    std::list<IParser *> parsers;
    parsers.push_back(Tokenizer);
    parsers.push_back(TihuDict);
    parsers.push_back(Hazm);
    parsers.push_back(Phonetics);
    parsers.push_back(Synthesizers[voice]);

    ParsText(text, parsers, true);
}

void CEngine::Diacritize(const std::string &text) {
    /// TODO::::
}

void CEngine::ParsText(const std::string &text, std::list<IParser *> parsers,
                       bool report_tags) {
    std::stringstream ss(text);
    std::string line;
    int offset = 0;
    while (std::getline(ss, line, '\n')) {
        CCorpus corpus(line, offset);

        for (auto parser : parsers) {
            parser->ParsText(&corpus);
        }

        if (report_tags) {
            std::string tags = corpus.ToTxt();
            Callback(TIHU_TEXT_TAGS, (long)tags.c_str(), tags.length(),
                     UserData);
        }

        offset += line.length();
    }
}

bool CEngine::SetParam(TIHU_PARAM param, int value) {
    bool applied = true;

    switch (param) {
    case TIHU_PARAM_PITCH:
        Settings->SetPitch(value);
        break;
    case TIHU_PARAM_VOLUME:
        Settings->SetVolume(value);
        break;
    case TIHU_PARAM_RATE:
        Settings->SetRate(value);
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

bool CEngine::GetParam(TIHU_PARAM param, int &value) {
    value = -1;

    switch (param) {
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
        value = Settings->GetFrequency();
        break;
    case TIHU_PARAM_READ_PUNCS:
        // TODO
        break;
    default:
        return false;
    }

    return true;
}

void CEngine::EnableDebugMode(bool enable) { Settings->SetIsDebugMode(enable); }

void CEngine::LogText(const std::string &text) const {
    const std::locale utf8_locale =
        std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    std::ofstream writer("text.txt", std::ofstream::out | std::ofstream::app);

    if (!writer.is_open()) {
        return;
    }

    writer.imbue(utf8_locale);

    writer << std::endl;
    writer << std::endl;
    writer << text;

    writer.close();
}

std::string CEngine::GetCurrentModulePath() const {
    std::string path;

    Dl_info dl_info;
    if (!dladdr((void *)tihu_Init, &dl_info)) {
        return path;
    }

    path = dl_info.dli_fname;

    size_t found = path.find_last_of("/\\");

    return path.substr(0, found + 1);
}
