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
#ifndef __TIHU__PHONETICS_H
#define __TIHU__PHONETICS_H

#pragma once

#include "../parser.h"
#include "number2phoneme.h"
#include "punctuation2phoneme.h"
#include "word2phoneme.h"

class CPhonetics : public IParser {
public:
  CPhonetics();
  ~CPhonetics();

  bool Load() override;

  void ParsText(CCorpus *corpus) override;

private:
  CWord2Phoneme Word2Phoneme;
  CNumber2Phoneme Number2Phoneme;
  CPunctuation2Phoneme Punctuation2Phoneme;
};

#endif
