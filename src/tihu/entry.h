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
#ifndef __TIHU__CORPUS__ENTRY_H
#define __TIHU__CORPUS__ENTRY_H

#pragma once

#include "helper.h"


//
class CEntry
{
public:
    CEntry();
    CEntry(CEntry &entry);

    void            SetRoot(const std::string &root);
    void            SetDictation(const std::string &dictation);
    void            SetPronunciation(const std::string &pronunciation);
    void            SetLabel(const std::string &label);
    void            SetFrequency(int frequency);

    std::string     GetRoot() const;
    std::string     GetDictation() const;
    std::string     GetPronunciation() const;
    std::string     GetLabel() const;
    int             GetFrequency() const;

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
    

private:
    std::string     Root;           // Root word
    std::string     Dictation;      // Dictation
    std::string     Pronunciation;  // Pronunciation
    std::string     Lable;          //
    uint32_t        Flags;
    int             Frequency;      // Frequency
};

typedef std::unique_ptr<CEntry> CEntryPtr;
typedef std::list<CEntryPtr> CEntryList;
static const std::unique_ptr<CEntry> null_entry;
#endif
