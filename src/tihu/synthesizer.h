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
#ifndef __TIHU__SYNTHESIZER_H
#define __TIHU__SYNTHESIZER_H

#pragma once

#include "parser.h"

class ISynthesizer
    : public IParser
{
public:
    ISynthesizer();
    virtual ~ISynthesizer();

    virtual bool InitializeVoice(const std::string &data_path) = 0;

    virtual void FinalizeVoice() = 0;

    virtual int  GetFrequency() const = 0;
    virtual void Stop() = 0;

    virtual void ApplyPitch(int pitch_adjust) = 0;
    virtual void ApplyRate(int rate_adjust) = 0;
    virtual void ApplyVolume(int volume_adjust) = 0;

protected:
    void FireEvents(const CWordPtr& word) const;
    bool PlaySamples(short* samples, int length) const;

protected:
    bool IsStop;
    int  SampleCounter;
};

#endif
