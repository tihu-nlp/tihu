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
#ifndef __TIHU__CHAR_MAP_H
#define __TIHU__CHAR_MAP_H

#pragma once

#include "../helper.h"

class CCharMap {
public:
  CCharMap();
  CCharMap(const CCharMap &char_map);
  CCharMap(std::string normed, uint16_t code, TIHU_TOKEN_TYPE type);

  void Fill(std::string normed, uint16_t code, TIHU_TOKEN_TYPE type);
  CCharMap &operator=(const CCharMap &right);

  std::string GetNormed() const;
  TIHU_TOKEN_TYPE GetType() const;
  uint16_t GetCode() const;

  bool IsDiacritic() const;
  bool IsLineBreak() const;
  /// bool         IsNullTerminated() const;

private:
  std::string Normed; // Standard Form of the letter
  uint16_t Code;
  TIHU_TOKEN_TYPE Type; // Type of the letter
};

#endif
