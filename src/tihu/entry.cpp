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

CEntry::CEntry()
    : Flags(0)
    , Frequency(0)
{ }

CEntry::CEntry(CEntry &entry)
    : Flags(0)
    , Frequency(0)
{
}

void CEntry::SetRoot(const std::string &root)
{
    Root = root;
}

void CEntry::SetDictation(const std::string &dictation)
{
    Dictation = dictation;
}

void CEntry::SetPronunciation(const std::string &pronunciation)
{
    Pronunciation = pronunciation;
}

void CEntry::SetLabel(const std::string &label)
{
    Lable = label;
}

void CEntry::SetFrequency(int frequency)
{
    Frequency = frequency;
}

std::string CEntry::GetRoot() const
{
    return Root;
}

std::string CEntry::GetDictation() const
{
    return Dictation;
}

std::string CEntry::GetPronunciation() const
{
    return Pronunciation;
}

std::string CEntry::GetLabel() const
{
    return Lable;
}

int CEntry::GetFrequency() const
{
    return Frequency;
}

bool CEntry::IsVerb() const
{
    return StartsWith(Lable, "V");
}

bool CEntry::IsNoun() const
{
	return StartsWith(Lable, "N");
}

bool CEntry::IsPronoun() const
{
	return StartsWith(Lable, "P");
}

bool CEntry::IsAdjective() const
{
	return StartsWith(Lable, "ADJ");
}

bool CEntry::IsDeterminer() const
{
	return StartsWith(Lable, "DET");
}

bool CEntry::IsAdverb() const
{
	return StartsWith(Lable, "ADV");
}

bool CEntry::IsAdposition() const
{
	return StartsWith(Lable, "POS");
}

bool CEntry::IsConjunction() const
{
    return false;
}

bool CEntry::IsNumeral() const
{
	return StartsWith(Lable, "NUM");
}

bool CEntry::IsInterjection() const
{
    return false;
}

bool CEntry::IsNounCommon() const
{
    assert(IsNoun());

    return false;
}

bool CEntry::IsNounProper() const
{
	assert(IsNoun());

	return false;
}
