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
#include "synthesizer.h"

#include <stdarg.h>

ISynthesizer::ISynthesizer()
{
    SampleCounter = 0;
    IsStop = false;
}

ISynthesizer::~ISynthesizer()
{
}

void ISynthesizer::FireEvents(const CWordPtr& word) const
{
    if (Callback) {

        /// word boundary
        Callback(TIHU_EVENT_WORD_BOUNDRY,
            word->GetOffset(),
            word->GetLength(),
            UserData);
    }
}

bool ISynthesizer::PlaySamples(short* samples, int length) const
{
    if (Callback) {
        if (Callback(TIHU_WAVE_BUFFER,
            reinterpret_cast<uintptr_t>(samples),
            length * 2,
            UserData) == TIHU_DATA_ABORT) {
            return false;
        }
    }

    return true;
}
