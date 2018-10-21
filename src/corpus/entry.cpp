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
#include "entry.h"

CEntry::CEntry() {}

CEntry::CEntry(CEntry &entry) {}

void CEntry::SetStem(const std::string &stem) {
    Stem = stem; //
}

void CEntry::SetPron(const std::string &pron) {
    Pron = pron; //
}

void CEntry::SetPOS(const std::string &pos) {
    POS = pos; //
}

std::string CEntry::GetStem() const {
    return Stem; //
}

std::string CEntry::GetPron() const {
    return Pron; //
}

std::string CEntry::GetPOS() const {
    return POS; //
}

std::string CEntry::GetLemma() const {
    return Lemma; //
}

bool CEntry::IsVerb() const {
    return StartsWith(POS, "V"); //
}

bool CEntry::IsNoun() const {
    return StartsWith(POS, "N"); //
}

bool CEntry::IsPronoun() const {
    return StartsWith(POS, "P"); //
}

bool CEntry::IsAdjective() const {
    return StartsWith(POS, "AJ"); //
}

bool CEntry::IsDeterminer() const {
    return StartsWith(POS, "DET"); //
}

bool CEntry::IsAdverb() const {
    return StartsWith(POS, "ADV"); //
}

bool CEntry::IsAdposition() const {
    return StartsWith(POS, "POS"); //
}

bool CEntry::IsConjunction() const {
    return false; //
}

bool CEntry::IsNumeral() const {
    return StartsWith(POS, "NUM"); //
}

bool CEntry::IsInterjection() const {
    return false; //
}

bool CEntry::IsNounCommon() const {
    assert(IsNoun());

    return false;
}

bool CEntry::IsNounProper() const {
    assert(IsNoun());

    return false;
}

void CEntry::SetIsGenitive(bool is_genitive) {
    if (is_genitive) {
        POS += "e";
    }
}

bool CEntry::HasGenitive() const {
    return POS.back() == 'e'; //
}

void CEntry::AddGenitive() {
    Pron = ConcatPronunciations(Pron, "e"); //
}