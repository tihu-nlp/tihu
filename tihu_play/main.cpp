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
#include <stdlib.h>
#include <stdio.h>

#include "../tihu/tihu.h"

#ifdef WIN32
#else
#include <dlfcn.h>
#endif

TIHU_CALLBACK_RETURN cb(TIHU_CALLBACK_MESSAGE message, long l_param, long w_param, void* user_data)
{
    FILE* fp = (FILE*)user_data;

    if (message == TIHU_WAVE_BUFFER) {
        fwrite((char*)l_param, sizeof(char), w_param, fp);
    }
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        printf("Error: Not enough arguments.\nex: ./tihu_play ./libtihu.so \"سلام\" /tmp/raw1\n");
        return 1;
    }

    void* handle;
    TIHU_PROC_INIT tihu_Init;
    TIHU_PROC_CLOSE tihu_Close;
    TIHU_PROC_SET_CALLBACK tihu_SetCallback;
    TIHU_PROC_LOAD_VOICE tihu_LoadVoice;
    TIHU_PROC_SPEAK tihu_Speak;
    char* error;

    handle = dlopen(argv[1], RTLD_LAZY);
    if(!handle) {
        printf("Error: Tihu module cannot open.\n");
        return 1;
    }

    tihu_Init = (TIHU_PROC_INIT)dlsym(handle, "tihu_Init");
    tihu_Close = (TIHU_PROC_CLOSE)dlsym(handle, "tihu_Close");
    tihu_SetCallback = (TIHU_PROC_SET_CALLBACK)dlsym(handle, "tihu_SetCallback");
    tihu_LoadVoice = (TIHU_PROC_LOAD_VOICE)dlsym(handle, "tihu_LoadVoice");
    tihu_Speak = (TIHU_PROC_SPEAK)dlsym(handle, "tihu_Speak");

    if( tihu_Init == NULL ||
        tihu_Close == NULL ||
        tihu_SetCallback == NULL ||
        tihu_LoadVoice == NULL ||
        tihu_Speak == NULL)  {
        printf("Error: Tihu module cannot load.\n");
        return 1;
    }

    if (!tihu_Init()) {
        printf("Error: Tihu module cannot init.\n");
        return 1;
    }

    FILE* fp = fopen(argv[3], "wb");
    tihu_LoadVoice(TIHU_VOICE_MBROLA_MALE);
    tihu_SetCallback(cb, fp);
    tihu_Speak(argv[2]);
    tihu_Close();
    fclose(fp);

    dlclose(handle);
    return 0;
}
