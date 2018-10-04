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
#ifndef __TIHU__MBROLA_LIB_H
#define __TIHU__MBROLA_LIB_H

#pragma once

#include "../../synthesizer.h"



class CMbrolaLib
{
public:
    CMbrolaLib();
    virtual ~CMbrolaLib();

    bool Initialize(const std::string &data_path);
    void Finalize();

    void Write(const std::string &mbr);
    int  Read(short* samples, int length);

    int  GetLastError() const;
    void GetLastErrorStr(char* buffer, int length) const;

    void ApplyPitch(int pitch);
    void ApplyRate(int rate);
    void ApplyVolume(int volume);

    int  GetFrequency() const;

    void Flush();
    void Clear();
    void Reset();

private:
};

#endif
