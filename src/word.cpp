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
#include "word.h"


#define WORD_FLAG_END_OF_SENTENCE       0x0001
#define WORD_FLAG_END_OF_PARAGRAPH      0x0002
#define WORD_FLAG_HAS_DIACRITIC         0x0004
#define WORD_FLAG_LTS_PHONETICS         0x0008

CWord::CWord()
    : Type(TIHU_TOKEN_TYPE::UNKNOWN)
    , Frequency(0)
    , Length(0)
    , Offset(0)
    , Flags(0)
{
}

CWord::~CWord()
{
}

void CWord::SetText(const std::string &text)
{
    Text = text;
}

void CWord::SetType(TIHU_TOKEN_TYPE type)
{
    Type = type;
}

void CWord::SetPron(const std::string &pron)
{
    Pron = pron;
}

void CWord::SetPOSTag(const std::string &pos_tag)
{
    POSTag = pos_tag;
}

void CWord::SetFrequency(size_t frequency)
{
    Frequency = frequency;
}

void CWord::SetLength(size_t length)
{
    Length = length;
}

void CWord::SetOffset(size_t offset)
{
    Offset = offset;
}

std::string CWord::GetText() const
{
    return Text;
}

TIHU_TOKEN_TYPE CWord::GetType() const
{
    return Type;
}

std::string CWord::GetPron() const
{
    return Pron;
}

std::string CWord::GetPOSTag() const
{
    return POSTag;
}

size_t CWord::GetFrequency() const
{
    return Frequency;
}

size_t CWord::GetLength() const
{
    return Length;
}

size_t CWord::GetOffset() const
{
    return Offset;
}

/// TODO: find better name for this function
void CWord::SetLTSPhonetics(bool lts)
{
    SET_FLAG(Flags, WORD_FLAG_LTS_PHONETICS);
}

bool CWord::LTSPhonetics() const
{
    return IS_FLAG_SET(Flags, WORD_FLAG_LTS_PHONETICS);
}

bool CWord::IsPersianWord() const
{
    return true;
}

bool CWord::IsEnglishWord() const
{
    return POSTag == "RES";
}

bool CWord::IsPunctuation() const
{
    return POSTag == "PUNC";
}

bool CWord::IsNumber() const
{
    return false;
}

bool CWord::IsVerb() const
{
    return false;
}

bool CWord::IsNoun() const
{
    return false;
}

bool CWord::IsNounCommon() const
{
    return false;
}

bool CWord::IsNounProper() const
{
    return false;
}

bool CWord::IsPronoun() const
{
    return false;
}

bool CWord::IsAdjective() const
{
    return false;
}

bool CWord::IsDeterminer() const
{
    return false;
}

bool CWord::IsAdverb() const
{
    return false;
}

bool CWord::IsAdposition() const
{
    return false;
}

bool CWord::IsConjunction() const
{
    return false;
}

bool CWord::IsNumeral() const
{
    return false;
}

bool CWord::IsInterjection() const
{
    return false;
}

void CWord::SetIsEndOfSentence(bool is_end_of_sentence)
{

}

CPhonemeList &CWord::GetPhonemeList()
{
    return PhonemeList;
}

CEventList &CWord::GetEventList()
{
    return EventList;
}


void CWord::ParsPronunciation(std::string pronuciation)
{
    for(size_t index = 0; index < Pron.length(); index++) {
        CPhonemePtr phoneme = std::make_unique<CPhoneme>();

        char prv_pho = (index > 0) ? Pron[index - 1] : 0;
        char cur_pho = Pron[index];
        char nxt_pho = Pron[index + 1];

        if(cur_pho == '^') {
            continue;
        }

        phoneme->SetPhonetic(prv_pho, cur_pho, nxt_pho);

        PhonemeList.push_back(std::move(phoneme));
    }
}

void CWord::AddEvent(CEventPtr &event)
{
    EventList.push_back(std::move(event));
}

void CWord::AddEvent(TIHU_EVENT_TYPE event_type, TIHU_EVENT_VALUE &event_value)
{
    EventList.push_back(
        std::make_unique<CEvent>(event_type, event_value));
}
