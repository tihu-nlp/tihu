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
#ifndef __TIHU__PHONEME_H
#define __TIHU__PHONEME_H

#pragma once

#include "pitch_range.h"

class CPhoneme {
public:
  CPhoneme();
  CPhoneme(char prev_phoneme, char phoneme, char next_phoneme);

  void SetPhonetic(char prev_phoneme, char phoneme, char next_phoneme);

  bool IsStop() const;
  bool IsFricative() const;
  bool IsAffricative() const;
  bool IsNasal() const;
  bool IsLiquid() const;
  bool IsApproximant() const;

  // void  SetPitch(int pitch_pos, int pitch_val,...);

  std::string GetMbrName();
  std::string GetIPAName();

  std::string GetMbrolString();

public:
  CPitchRange PitchRange; // this is a data that keep range of pitch
  int PhonemeIndex;       // The index of the current phoneme in phonemes table
  int Duration;           // duration of the phoneme in milli_seconds
  bool PrevVoweled;
};

typedef std::unique_ptr<CPhoneme> CPhonemePtr;
typedef std::list<CPhonemePtr> CPhonemeList;

#endif
