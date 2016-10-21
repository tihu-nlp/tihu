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
#include "tokenization/tokenizer/tokenizer.h"
#include "morphology/pos_tagger/pos_tagger.h"
#include "morphology/pos_disambiguator/pos_disambiguator.h"
#include "correction/text_tagger/text_tagger.h"
#include "diacritization/letter_to_sound/letter_to_sound.h"
#include "synthesis/mbrola/mbrola_syn.h"
#include "synthesis/espeak/espeak_syn.h"
#include "path_manager.h"
#include "engine.h"
#include "tihu.h"

#include <iostream>
#include <locale>



#define LOG_ENABLED


CEngine::CEngine()
{
    Tokenizer       = nullptr;
    POSTagger       = nullptr;
    POSDisamb		= nullptr;
	LetterToSound	= nullptr;
    Normalizer      = nullptr;
    Synthesizer     = nullptr;
    TextTagger      = nullptr;
	Corpus		    = new CCorpus();
}

CEngine::~CEngine()
{
    if(Tokenizer) {
        delete Tokenizer;
    }
    if(POSTagger) {
        delete POSTagger;
    }
    if(POSDisamb) {
        delete POSDisamb;
    }
    if(LetterToSound) {
        delete LetterToSound;
    }
    if(Normalizer) {
        delete Normalizer;
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
    /// if module re-loaded, it's better to re-initialize it.
    CPathManager::GetInstance()->Initialize();

    Tokenizer = new CTokenizer();
    POSTagger = new CPOSTagger();
    POSDisamb = new CPOSDisambiguator();
	LetterToSound = new CLetterToSound();
    TextTagger = new CTextTagger();
    
    std::string tokens_path = CPathManager::GetInstance()->GetDataFolder();
    std::string affixes_path = CPathManager::GetInstance()->GetDataFolder();
    std::string dictionary_path = CPathManager::GetInstance()->GetDataFolder();
    std::string g2p_persian_model = CPathManager::GetInstance()->GetDataFolder();
    std::string g2p_english_model = CPathManager::GetInstance()->GetDataFolder();
	std::string punctuations_path = CPathManager::GetInstance()->GetDataFolder();

    tokens_path += "tokens.txt";
    affixes_path += "lexicon.aff";
    dictionary_path += "lexicon.dic";
    g2p_english_model += "g2p-seq2seq-cmudict";
    g2p_persian_model += "g2p-seq2seq-tihudict";
	punctuations_path += "punctuations.txt";
    
    if(!static_cast<CTokenizer*>(Tokenizer)->Load(tokens_path)) {
        return TIHU_ERROR_LOAD_DATA;
    }

    if(!static_cast<CPOSTagger*>(POSTagger)->Load(affixes_path, 
        dictionary_path, "")) {
        return TIHU_ERROR_LOAD_DATA;
    }
    
    if(!static_cast<CLetterToSound*>(LetterToSound)->Load(g2p_persian_model, 
        g2p_english_model, punctuations_path)) {
        return TIHU_ERROR_LOAD_DATA;
    }

    return TIHU_ERROR_NONE;
}

int CEngine::LoadSynthesizer(TIHU_VOICE vocie)
{
    if(Synthesizer) {
        delete Synthesizer;
    }

    std::string data_path = CPathManager::GetInstance()->GetDataFolder();

    switch(vocie) {
    case TIHU_VOICE_MBROLA_MALE: {
        Synthesizer = new CMbrolaSyn();
        data_path += "mbrola/ir1";
    }
    break;

    case TIHU_VOICE_MBROLA_FEMALE: {
        Synthesizer = new CMbrolaSyn();
        data_path += "mbrola/ir2";
    }
    break;

    case TIHU_VOICE_ESPEAK_MALE: {
        Synthesizer = new CeSpeakSyn();
        data_path += "espeak";
    }
    break;

    case TIHU_VOICE_ESPEAK_FEMALE: {
        Synthesizer = new CeSpeakSyn();
        data_path += "espeak";
    }
    break;
    }

    if(!Synthesizer->InitializeVoice(data_path)) {
        return TIHU_ERROR_LOAD_VOICE;
    }

    return TIHU_ERROR_NONE;
}

void CEngine::SetCallback(TIHU_CALLBACK callback, void* userData)
{
    Tokenizer->SetCallBack(callback, userData);
    POSTagger->SetCallBack(callback, userData);
    POSDisamb->SetCallBack(callback, userData);
	LetterToSound->SetCallBack(callback, userData);
    Synthesizer->SetCallBack(callback, userData);
    TextTagger->SetCallBack(callback, userData);
}

void CEngine::Stop()
{
	if (Synthesizer) {
		Synthesizer->Stop();
	}
}

void CEngine::Speak(const std::string &text)
{
	SetText(text);
	Tokenize();
	POSTag();
	Diacritize();
	Synthesize();
}

void CEngine::Diacritize(const std::string &text)
{
	SetText(text);
	Tokenize();
	POSTag();
	Diacritize();
}

void CEngine::AutoTag(const std::string &text)
{
	SetText(text);
	Tokenize();
	POSTag();
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

void CEngine::Tokenize() const
{
	Tokenizer->ParsText(Corpus);
#ifdef LOG_ENABLED
	LogCorpus("tokenize.xml");
#endif
}

void CEngine::POSTag() const
{
	POSTagger->ParsText(Corpus);
#ifdef LOG_ENABLED
	LogCorpus("pos-tagger.xml");
#endif

	POSDisamb->ParsText(Corpus);

#ifdef LOG_ENABLED
	LogCorpus("pos-disamb.txt");
#endif
}

void CEngine::Diacritize() const
{
	LetterToSound->ParsText(Corpus);

#ifdef LOG_ENABLED
	LogCorpus("letter-to-sound.txt");
#endif
}

void CEngine::Synthesize() const
{
	Synthesizer->ParsText(Corpus);
}

void CEngine::LogText(const std::string& filename) const
{
    std::string path = CPathManager::GetInstance()->GetLogFile(filename);

    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    std::ofstream writer(path);

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
	std::string path = CPathManager::GetInstance()->GetLogFile(filename);

	Corpus->Dump(path);
}

void CEngine::Dump(const std::string &filename)
{
	Corpus->Dump(filename);
}
