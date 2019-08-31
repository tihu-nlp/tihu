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
#include "phonetics.h"

CPhonetics::CPhonetics() {}

CPhonetics::~CPhonetics() {}

bool CPhonetics::Load(std::string param) {
    if (!Word2Phoneme.LoadModel("./data/g2p-seq2seq-tihudict")) {
        return false;
    }

    if (!Punctuation2Phoneme.Load("data/punctuations.txt")) {
        return false;
    }

    return true;
}

void CPhonetics::ParsText(CCorpus *corpus) {
    CWordList &word_list = corpus->GetWordList();
    for (auto &word : word_list) {
        auto &entry = word->GetBestEntry();

        if (entry->GetPron().empty()) {
            std::string pron;

            if (word->IsPersianWord()) {
                ///
                pron = Word2Phoneme.Convert(word->GetText());
                word->SetIsAutoPhonetics(true);
            } else if (word->IsNumber()) {
                ///
                pron = Number2Phoneme.Convert(word->GetText());
            } else if (word->IsPunctuation()) {
                ///
                pron = Punctuation2Phoneme.Convert(word->GetText());
            }

            entry->SetPron(pron);
        }
    }

    corpus->Dump("w2p.xml");
}
