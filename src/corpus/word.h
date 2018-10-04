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
#ifndef __TIHU__WORD_H
#define __TIHU__WORD_H

#pragma once

#include "event.h"
#include "phoneme.h"

class CWord
{
public:
    CWord();
    ~CWord();

    void            SetText(const std::string &text);
    void            SetType(TIHU_TOKEN_TYPE type);
    void            SetPron(const std::string &pron);
    void            SetPOSTag(const std::string &pos_tag);
    void            SetFrequency(size_t frequency);
    void            SetLength(size_t length);
    void            SetOffset(size_t offset);
    void            SetIsEndOfParagraph(bool is_end_of_paragraph);
    void            SetIsEndOfSentence(bool is_end_of_sentence);
    void            SetHasDiacritic(bool has_diacritic);

    std::string     GetText() const;
    TIHU_TOKEN_TYPE GetType() const;
    std::string     GetPron() const;
    std::string     GetPOSTag() const;
    size_t          GetFrequency() const;
    size_t          GetLength() const;
    size_t          GetOffset() const;
    bool            IsEndOfParagraph() const;
    bool            IsEndOfSentence() const;
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

    bool            HasKasreEzafe() const;

    void            SetLTSPhonetics(bool lts);
    bool            LTSPhonetics() const;


    CPhonemeList    &GetPhonemeList();
    CEventList      &GetEventList();


    void            ParsPronunciation(std::string pronuciation = std::string());

    void            AddEvent(CEventPtr &event);
    void            AddEvent(TIHU_EVENT_TYPE event_type, TIHU_EVENT_VALUE &event_value);

protected:
    std::string      Text;          // Normal word text
    TIHU_TOKEN_TYPE  Type;          // Type of the the word(word)
    std::string      Pron;          // Pronunciation
    std::string      POSTag;        // Part-Of-Speech tag
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
