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
#ifndef __TIHU__AFX_ENTRY_H
#define __TIHU__AFX_ENTRY_H

#include "hentry.h"

#include <string>

class CAfxManager;
class CAfxEntry
{
public:
    explicit CAfxEntry(CAfxManager* afx_manager)
        : Flag(0)
        , AfxManager(afx_manager)
    {
    }


public:
    int          Flag;
    std::string  Appnd;
    std::string  Pronunce;
    std::string  Label;

    CAfxManager* AfxManager;
};

#endif
