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
#ifndef __TIHU__CORPUS__WORD_H
#define __TIHU__CORPUS__WORD_H

#pragma once

#include "event.h"
#include "phoneme.h"

class CWord
{
public:
    CWord();
    ~CWord();

    void            SetText(const std::string &text);
    void            SetPronunc(const std::string &pronunc);
    void            SetLable(const std::string &lable);
    void            SetFrequency(size_t frequency);
    void            SetLength(size_t length);
    void            SetOffset(size_t offset);
    void            SetIsEndOfSentence(bool is_end_of_sentence);

    std::string     GetText() const;
    std::string     GetPronunc() const;
    std::string     GetLable() const;
    size_t          GetFrequency() const;
    size_t          GetLength() const;
    size_t          GetOffset() const;
    bool            IsPersianWord() const;
    bool            IsEnglishWord() const;
    bool            IsPunctuation() const;
    bool            IsNumber() const;

    bool            IsVerb() const;
    bool            IsNoun() const;
    bool            IsNounCommon() const;
    bool            IsNounProper() const;
    bool            IsPronoun() const;
    bool            IsAdjective() const;
    bool            IsDeterminer() const;
    bool            IsAdverb() const;
    bool            IsAdposition() const;
    bool            IsConjunction() const;
    bool            IsNumeral() const;
    bool            IsInterjection() const;


    CPhonemeList    &GetPhonemeList();
    CEventList      &GetEventList();


    void            ParsPronunciation(std::string pronuciation = std::string());

    void            AddEvent(CEventPtr &event);
    void            AddEvent(TIHU_EVENT_TYPE event_type, TIHU_EVENT_VALUE &event_value);

protected:
    std::string      Text;            // Normal word text
    std::string      Pronunc;        // Pronunciation
    std::string      Lable;         // lable
    size_t           Frequency;     // Frequency
    size_t           Length;        // Length of the current word inside of input text
    size_t           Offset;        // offset of the current word inside of input text
    size_t           Flags;

    CEventList       EventList;
    CPhonemeList     PhonemeList;
};

typedef std::unique_ptr<CWord> CWordPtr;
typedef std::list<CWordPtr> CWordList;
static const std::unique_ptr<CWord> null_word;
#endif
