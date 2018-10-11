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
#include "word_to_phonetic.h"

CLetterToSound::CLetterToSound() {}

CLetterToSound::~CLetterToSound() {}

bool CLetterToSound::Load(std::string name) {
    if (!PersianToPhoneme.LoadModel("./data/g2p-seq2seq-tihudict")) {
        return false;
    }

    /*
    if(!EnglishToPhoneme.LoadModel("./data/g2p-seq2seq-cmudict")) {
        return false;
    }
    */

    if (!PunctuationToPhoneme.Load("data/punctuations.txt")) {
        return false;
    }

    return true;
}

void CLetterToSound::ParsText(CCorpus *corpus) {
    CWordList &word_list = corpus->GetWordList();
    for (auto &word : word_list) {
        auto &entry = word->GetBestEntry();

        if (entry->GetPron().empty()) {

            std::string pron;
            if (word->IsPersianWord()) {
                ///
                pron = PersianToPhoneme.Convert(word->GetText());

                word->SetIsAutoPhonetics(true);

                /// Add Kasre-Ezafe
                if (entry->HasKasreEzafe()) {
                    entry->AddKasreEzafe();
                }
            } else if (word->IsEnglishWord()) {
                //
                pron = EnglishToPhoneme.Convert(word->GetText());
            } else if (word->IsNumber()) {
                ///
                pron = NumberToPhoneme.Convert(word->GetText());
            } else if (word->IsPunctuation()) {
                ///
                pron = PunctuationToPhoneme.Convert(word->GetText());
            }

            entry->SetPron(pron);
        }
    }
}
