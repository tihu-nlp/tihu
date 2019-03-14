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
#ifndef __TIHU__WORD_TO_PHONETIC_H
#define __TIHU__WORD_TO_PHONETIC_H

#pragma once

#include "../parser.h"
#include "english_to_phonetic.h"
#include "number_to_phonetic.h"
#include "persian_to_phonetic.h"
#include "punctuation_to_phonetic.h"

class CWordToPhonetic : public IParser {
  public:
    CWordToPhonetic();
    ~CWordToPhonetic();

    bool Load(std::string param = "") override;

    void ParsText(CCorpus *corpus) override;

  private:
    CPersianToPhoneme PersianToPhoneme;
    CEnglishToPhoneme EnglishToPhoneme;
    CNumberToPhoneme NumberToPhoneme;
    CPunctuationToPhoneme PunctuationToPhoneme;
};

#endif
