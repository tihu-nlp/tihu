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
#include "mbrola_lib.h"

#include "../../helper.h"

#define DEFAULT_MBR_VOLUME 3.0F
#define DEFAULT_MBR_PITCH 1.0F
#define DEFAULT_MBR_SPEED 1.0F


#include "mbrowrap.h"

CMbrolaLib::CMbrolaLib() {}

CMbrolaLib::~CMbrolaLib() {
    // Release();
}

bool CMbrolaLib::Initialize(const std::string &data_path) {
    Finalize();

    // --- Load DataBase ---
    if (mbr.init_MBR((char *)data_path.c_str()) < 0) {
        Finalize();
        return false;
    }
    // ---------------------

    mbr.setNoError_MBR(1);

    return true;
}

void CMbrolaLib::Finalize() {
    mbr.close_MBR();
}

int CMbrolaLib::GetLastError() const {
    return 0; ///
}

void CMbrolaLib::GetLastErrorStr(char *buf, int length) {
    mbr.lastErrorStr_MBR(buf, length);
}

void CMbrolaLib::ApplyPitch(int pitch_adjust) {
    float fPitch = ((pitch_adjust + 10) * 1.6F / 20) + 0.2F;

    char buf[64];
    sprintf(buf, ";; F = %f\r\n", fPitch);

    Write(buf);
}

void CMbrolaLib::ApplyRate(int rate_adjust) {
    float fRate = 0.0F;

    if (rate_adjust >= 0) {
        fRate = (abs(rate_adjust - 10) * 0.06F) +
                0.2F; // Mbrola speed range must be between 2.2 and 0.2
                      // (reversed value)
    } else {
        fRate = (abs(rate_adjust) * 0.5F) +
                0.2F; // Mbrola speed range must be between 2.2 and 0.2
                      // (reversed value)
    }

    char buf[64];
    sprintf(buf, ";; T = %f\r\n", fRate);

    Write(buf);
}

void CMbrolaLib::ApplyVolume(int volume_adjust) {
    // The volume range must be between 0.2 and 3.0
    float fVolume = (volume_adjust * 2.8F / 100) + 0.2F; //

    mbr.setVolumeRatio_MBR(fVolume);
}

int CMbrolaLib::GetFrequency() const { return mbr.getFreq_MBR(); }

void CMbrolaLib::Write(const std::string &t) {
    mbr.write_MBR(t.c_str());
}

int CMbrolaLib::Read(short *samples, int length) {
    return mbr.read_MBR(samples, length);
}

void CMbrolaLib::Flush() { mbr.flush_MBR(); }

void CMbrolaLib::Clear() {
    Flush();
}

void CMbrolaLib::Reset() { mbr.reset_MBR(); }
