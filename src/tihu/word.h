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
#include "entry.h"
#include "phoneme.h"

class CWord
{
public:
    CWord();
    CWord(CWord &word);
    ~CWord();

    void             SetText(const std::string &text);
    void             SetType(TIHU_TOKEN_TYPE type);
    void             SetLength(size_t length);
    void             SetOffset(size_t offset);
    void             SetIsEndOfParagraph(bool is_end_of_paragraph);
    void             SetIsEndOfSentence(bool is_end_of_sentence);
    void             SetHasDiacritic(bool has_diacritic);

    std::string      GetText() const;
    TIHU_TOKEN_TYPE  GetType() const;
    size_t           GetLength() const;
    size_t           GetOffset() const;
    bool             IsEndOfParagraph() const;
    bool             IsEndOfSentence() const;
    bool             IsPersianWord() const;
	bool             IsEnglishWord() const;
    bool             IsPunctuation() const;
    bool             IsNumber() const;
    bool             HasDiacritic() const;
    bool             IsEmpty() const;

    CPhonemeList    &GetPhonemeList();
    CEventList      &GetEventList();
    CEntryList      &GetEntryList();

    const CEntryPtr &GetBestEntry() const;

    void             ParsPronunciation(std::string pronuciation = std::string());

    void             AddEntry(CEntryPtr &entry);
    void             AddEvent(CEventPtr &event);
    void             AddEvent(TIHU_EVENT_TYPE event_type, TIHU_EVENT_VALUE &event_value);

protected:
    std::string      Text;       // Normal word text
    TIHU_TOKEN_TYPE  Type;       // Type of the the word(word)
    size_t           Length;     // Length of the current word inside of input text
    size_t           Offset;     // offset of the current word inside of input text
    uint32_t         ExtraInfo;  // Extra information

    CEventList       EventList;
    CPhonemeList     PhonemeList;
    CEntryList       EntryList;
};

typedef std::unique_ptr<CWord> CWordPtr;
typedef std::list<CWordPtr> CWordList;
static const std::unique_ptr<CWord> null_word;
#endif
