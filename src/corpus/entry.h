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
#ifndef __TIHU__ENTRY_H
#define __TIHU__ENTRY_H

#pragma once

#include "../helper.h"

//
class CEntry {
  public:
    CEntry();
    CEntry(CEntry &entry);

    void SetLemma(const std::string &lemma);
    void SetStem(const std::string &stem);
    void SetPron(const std::string &pron);
    void SetPOS(const std::string &pos);

    std::string GetLemma() const;
    std::string GetStem() const;
    std::string GetPron() const;
    std::string GetPOS() const;

    bool IsVerb() const;
    bool IsNoun() const;
    bool IsNounCommon() const;
    bool IsNounProper() const;
    bool IsPronoun() const;
    bool IsAdjective() const;
    bool IsDeterminer() const;
    bool IsAdverb() const;
    bool IsAdposition() const;
    bool IsConjunction() const;
    bool IsNumeral() const;
    bool IsInterjection() const;

    bool HasKasreEzafe() const;
    void AddKasreEzafe();

  private:
    std::string Stem;  // Stem of the word
    std::string Lemma; //
    std::string Pron;  // Pronunciation
    std::string POS;   // Part-Of-Speech tag
};

typedef std::unique_ptr<CEntry> CEntryPtr;
typedef std::list<CEntryPtr> CEntryList;
static const CEntryPtr null_entry;
static const CEntryPtr nop_entry = std::make_unique<CEntry>();
#endif
