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
#ifndef __TIHU__POS_TAGGER__AFX_MANAGER_H
#define __TIHU__POS_TAGGER__AFX_MANAGER_H

#pragma once

#include "pfx_entry.h"
#include "sfx_entry.h"
#include "file_manager.h"
#include "hentry.h"
#include "word.h"

#define SETSIZE 256

class CHashManager;
class CAfxManager
{
public:
    CAfxManager(CHashManager* hash_manager);
    ~CAfxManager();

    bool Load(std::string filename);

    struct hentry* Lookup(const char* text);

    void AffixCheck(const char* text, int len,
                    const FLAG needflag, CWordPtr &word);

    void PrefixCheck(const char* text, int len,
                     const FLAG needflag, CWordPtr &word);

    void SuffixCheck(const char* text, int len,
                     CPfxEntry* pfx, const FLAG needflag, CWordPtr &word);

    bool IsPrefix(const char* prefix);

    void ParsEntry(struct hentry* he, CPfxEntry* pfx, CSfxEntry* sfx, CWordPtr &word);

protected:

    int ParseAffix(char afx_type, CFileManager &file_manager);

    int BuildPrefixTree(CPfxEntry* entry);
    int BuildSuffixTree(CSfxEntry* entry);
    int ProcessPrefixOrder();
    int ProcessSuffixOrder();

    int ProcessPfxTreeToList();
    int ProcessSfxTreeToList();

    CPfxEntry* ProcessPfxInOrder(CPfxEntry* ptr, CPfxEntry* nptr);
    CSfxEntry* ProcessSfxInOrder(CSfxEntry* ptr, CSfxEntry* nptr);

    inline int isSubset(const char* s1, const char* s2);

    inline int isRevSubset(const char* s1,
                           const char* end_of_s2, int len);




private:
    CPfxEntry* PrefixStart[SETSIZE];
    CSfxEntry* SuffixStart[SETSIZE];
    CPfxEntry* PrefixFlag[SETSIZE];
    CSfxEntry* SuffixFlag[SETSIZE];

    CHashManager* HashManager;
};

#endif
