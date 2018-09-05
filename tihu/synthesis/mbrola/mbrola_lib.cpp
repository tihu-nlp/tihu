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

#include "helper.h"


#define DEFAULT_MBR_VOLUME          3.0F
#define DEFAULT_MBR_PITCH           1.0F
#define DEFAULT_MBR_SPEED           1.0F

#ifdef WIN32

#include <windows.h>
typedef void (WINAPI* PROCVV)();
typedef void (WINAPI* PROCVI)(int);
typedef void (WINAPI* PROCVF)(float);
typedef int (WINAPI* PROCIV)();
typedef int (WINAPI* PROCIC)(char*);
typedef int (WINAPI* PROCISI)(short*,int);
typedef char* (WINAPI* PROCVCI)(char*,int);

PROCIC      init_MBR;
PROCIC      write_MBR;
PROCIV      flush_MBR;
PROCISI     read_MBR;
PROCVV      close_MBR;
PROCVV      reset_MBR;
PROCIV      lastError_MBR;
PROCVCI     lastErrorStr_MBR;
PROCVI      setNoError_MBR;
PROCIV      getFreq_MBR;
PROCVF      setVolumeRatio_MBR;



HINSTANCE   hinstDllMBR = NULL;


BOOL load_MBR(char* mbrola)
{
    if(hinstDllMBR != NULL) {
        return TRUE;    // already loaded
    }

    if((hinstDllMBR=LoadLibraryA(mbrola)) == 0) {
        return FALSE;
    }
    init_MBR            =(PROCIC) GetProcAddress(hinstDllMBR,"init_MBR");
    write_MBR           =(PROCIC) GetProcAddress(hinstDllMBR,"write_MBR");
    flush_MBR           =(PROCIV) GetProcAddress(hinstDllMBR,"flush_MBR");
    read_MBR            =(PROCISI) GetProcAddress(hinstDllMBR,"read_MBR");
    close_MBR           =(PROCVV) GetProcAddress(hinstDllMBR,"close_MBR");
    reset_MBR           =(PROCVV) GetProcAddress(hinstDllMBR,"reset_MBR");
    lastError_MBR       =(PROCIV) GetProcAddress(hinstDllMBR,"lastError_MBR");
    lastErrorStr_MBR    =(PROCVCI) GetProcAddress(hinstDllMBR,"lastErrorStr_MBR");
    setNoError_MBR      =(PROCVI) GetProcAddress(hinstDllMBR,"setNoError_MBR");
    setVolumeRatio_MBR  =(PROCVF) GetProcAddress(hinstDllMBR,"setVolumeRatio_MBR");
    getFreq_MBR         =(PROCIV) GetProcAddress(hinstDllMBR,"getFreq_MBR");

    return TRUE;
}


void unload_MBR()
{
    if(hinstDllMBR) {
        FreeLibrary(hinstDllMBR);
        hinstDllMBR=NULL;
    }
}

#else
#define INCLUDE_MBROLA

#include "mbrowrap.h"
#endif

CMbrolaLib::CMbrolaLib()
{
}

CMbrolaLib::~CMbrolaLib()
{
    //Release();
}

bool CMbrolaLib::Initialize(const std::string &data_path)
{
    Finalize();

#ifdef WIN32
    if(load_MBR("mbrola.dll") == FALSE) {    // load mbrola.dll
        fprintf(stderr, "Can't load mbrola.dll\n");
        return false;
    }
#endif

    // --- Load DataBase ---
    if(init_MBR((char*)data_path.c_str()) < 0) {
        Finalize();
        return false;
    }
    // ---------------------

    setNoError_MBR(1);

    return true;
}


void CMbrolaLib::Finalize()
{
#ifdef WIN32
    unload_MBR();
#else
    close_MBR();
#endif
}

int CMbrolaLib::GetLastError() const
{
#ifdef WIN32
    return lastError_MBR();
#else
    return 0;///
#endif
}

void CMbrolaLib::GetLastErrorStr(char* buf, int length) const
{
    lastErrorStr_MBR(buf,length);
}

void CMbrolaLib::ApplyPitch(int pitch_adjust)
{
    float fPitch = ((pitch_adjust+10) * 1.6F /20) + 0.2F;

    char buf[64];
    sprintf(buf, ";; F = %f\r\n", fPitch);

    Write(buf);
}

void CMbrolaLib::ApplyRate(int rate_adjust)
{
    float fRate = 0.0F;

    if(rate_adjust >= 0) {
        fRate = (abs(rate_adjust-10) * 0.06F) + 0.2F;    // Mbrola speed range must be between 2.2 and 0.2 (reversed value)
    } else {
        fRate = (abs(rate_adjust) * 0.5F) + 0.2F;    // Mbrola speed range must be between 2.2 and 0.2 (reversed value)
    }

    char buf[64];
    sprintf(buf, ";; T = %f\r\n", fRate);

    Write(buf);
}

void CMbrolaLib::ApplyVolume(int volume_adjust)
{
    // The volume range must be between 0.2 and 3.0
    float fVolume = (volume_adjust * 2.8F / 100) + 0.2F; //

    setVolumeRatio_MBR(fVolume);
}

int CMbrolaLib::GetFrequency() const
{
    return getFreq_MBR();
}

void CMbrolaLib::Write(const std::string &mbr)
{
    write_MBR((char*)mbr.c_str());
}

int CMbrolaLib::Read(short* samples, int length)
{
    return read_MBR(samples, length);
}

void CMbrolaLib::Flush()
{
    flush_MBR();
}

void CMbrolaLib::Clear()
{
    ///Flush();
}

void CMbrolaLib::Reset()
{
    reset_MBR();
}
