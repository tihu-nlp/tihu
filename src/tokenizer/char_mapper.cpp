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
#include "char_mapper.h"
#include "../helper.h"

CCharMapper::CCharMapper() {}

void CCharMapper::SetCharMap(uint16_t letter, const CCharMap &char_map) {
    uint8_t row_no = GET_HIBYTE(letter); // Row number in the char mapper table
    uint8_t col_no =
        GET_LOBYTE(letter); // Column number in the char mapper table

    if (CharMapTable[row_no] ==
        nullptr) { // If this row is not existed, create a new one.
        CharMapTable[row_no] = std::make_unique<CCharMapRow>();
    }

    CharMapTable[row_no]->SetCharMap(col_no, char_map);
}

CCharMap CCharMapper::GetCharMap(uint16_t letter) const {
    uint8_t row_no = GET_HIBYTE(letter); // Row number in the char mapper table
    uint8_t col_no = GET_LOBYTE(letter); // Column number in the char mapper table

    if (CharMapTable[row_no] != nullptr) {
        return CharMapTable[row_no]->GetCharMap(col_no);
    }

    std::u16string str = std::u16string(1, letter);
    std::string u8 = UTF16ToUTF8(str);

    return CCharMap(u8, letter, TIHU_TOKEN_TYPE::UNKNOWN);
}
