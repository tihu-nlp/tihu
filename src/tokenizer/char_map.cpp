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
#include "char_map.h"

CCharMap::CCharMap() : Code(0), Type(TIHU_TOKEN_TYPE::UNKNOWN) {}

CCharMap::CCharMap(const CCharMap &char_map)
    : Normed(char_map.Normed), Code(char_map.Code), Type(char_map.Type)
{}

CCharMap::CCharMap(std::string normed, uint16_t code, TIHU_TOKEN_TYPE type)
    : Normed(normed), Code(code), Type(type) {}

void CCharMap::Fill(std::string normed, uint16_t code, TIHU_TOKEN_TYPE type) {
    Normed = normed;
    Type = type;
    Code = code;
}

CCharMap &CCharMap::operator=(const CCharMap &right) {
    Fill(right.Normed, right.Code, right.Type);

    return *this;
}

std::string CCharMap::GetNormed() const {
    return Normed; //
}

TIHU_TOKEN_TYPE CCharMap::GetType() const {
    return Type; //
}

uint16_t CCharMap::GetCode() const {
    return Code; //
}

bool CCharMap::IsDiacritic() const {
    return (Code >= 0x064B && Code <= 0x0652);
}

bool CCharMap::IsLineBreak() const { return (Code == '\n' || Code == '\r'); }
