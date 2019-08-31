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
#ifndef __TIHU__SFX_ENTRY_H
#define __TIHU__SFX_ENTRY_H

#include "afx_entry.h"
#include "../corpus/word.h"

class CPfxEntry;
class CSfxEntry
    : public CAfxEntry
{
public:
    CSfxEntry(CAfxManager* afx_manager);

    void CheckWord(const char* text,
                   int len,
                   CPfxEntry* pfx,
                   const FLAG needflag, CWordPtr &word);

    inline FLAG GetFlag() const {
        return Flag;
    }
    inline const char* GetKey() const {
        return rAppnd.c_str();
    }
    inline CSfxEntry* GetNext() const {
        return next;
    }
    inline CSfxEntry* GetNextNE() const {
        return nextne;
    }
    inline CSfxEntry* GetNextEQ() const {
        return nexteq;
    }

    inline void SetNext(CSfxEntry* ptr) {
        next = ptr;
    }
    inline void SetNextNE(CSfxEntry* ptr) {
        nextne = ptr;
    }
    inline void SetNextEQ(CSfxEntry* ptr) {
        nexteq = ptr;
    }

    void initReverseWord();

protected:
    std::string rAppnd;

    CSfxEntry*   next;
    CSfxEntry*   nexteq;
    CSfxEntry*   nextne;
};

#endif
