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
#ifndef __TIHU__POS_TAGGER__PFX_ENTRY_H
#define __TIHU__POS_TAGGER__PFX_ENTRY_H

#include "afx_entry.h"
#include "word.h"

class CPfxEntry
    : public CAfxEntry
{
public:
    CPfxEntry(CAfxManager* afx_manager);

    inline FLAG GetFlag() const
    {
        return Flag;
    }
    inline const char* GetKey() const
    {
        return Appnd.c_str();
    }
    inline CPfxEntry* GetNext() const
    {
        return next;
    }
    inline CPfxEntry* GetNextNE() const
    {
        return nextne;
    }
    inline CPfxEntry* GetNextEQ() const
    {
        return nexteq;
    }

    inline void SetNext(CPfxEntry* ptr)
    {
        next = ptr;
    }
    inline void SetNextNE(CPfxEntry* ptr)
    {
        nextne = ptr;
    }
    inline void SetNextEQ(CPfxEntry* ptr)
    {
        nexteq = ptr;
    }

    void CheckWord(const char* text, int len,
                   const FLAG needflag, CWordPtr &word);

private:
    CPfxEntry*   next;
    CPfxEntry*   nexteq;
    CPfxEntry*   nextne;
};

#endif
