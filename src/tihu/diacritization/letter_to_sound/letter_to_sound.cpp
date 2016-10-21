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
#include "letter_to_sound.h"

CLetterToSound::CLetterToSound()
{
}

CLetterToSound::~CLetterToSound()
{
}

bool CLetterToSound::Load(const std::string &persian_model, 
    const std::string &english_model, const std::string &punctuations_path)
{
    if(!PersianToPhoneme.LoadModel(persian_model)) {
        return false;
    }
    
    if(!EnglishToPhoneme.LoadModel(english_model)) {
        return false;
    }

	if (!PunctuationToPhoneme.Load(punctuations_path)) {
		return false;
	}
    
    return true;
}

void CLetterToSound::ParsText(CCorpus* corpus)
{
    CWordList &word_list = corpus->GetWordList();
    for(auto &word : word_list) {

		if (word->IsEmpty()) {
			CEntryPtr entry = std::make_unique<CEntry>();

			std::string phonemes;
			if (word->IsPersianWord()) {
				///
				phonemes =
					PersianToPhoneme.Convert(word->GetText());
			} else if (word->IsEnglishWord()) {
				//
				phonemes =
					EnglishToPhoneme.Convert(word->GetText());
			} else if (word->IsNumber()) {
				/// 
				phonemes =
					NumberToPhoneme.Convert(word->GetText());
			} else if (word->IsPunctuation()) {
				/// 
				phonemes =
					PunctuationToPhoneme.Convert(word->GetText());
			}

			entry->SetPronunciation(phonemes);

			word->AddEntry(entry);
		}
    }
}
