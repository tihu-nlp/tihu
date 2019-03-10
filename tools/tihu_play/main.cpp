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

#include "../../src/tihu.h"

#ifdef WIN32
#else
#include <dlfcn.h>
#endif


unsigned char WAVE_HEADER_WAV[44] = {
    0x52, 0x49, 0x46, 0x46, 0x24, 0x76, 0x6B, 0x00,
    0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
    0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00,
    0x40, 0x1F, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00,
    0x04, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61,
    0x00, 0x00, 0x00, 0x00
};

TIHU_CALLBACK_RETURN cb(TIHU_CALLBACK_MESSAGE message, long l_param, long w_param, void* user_data) {
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
    TIHU_PROC_CALLBACK tihu_Callback;
    TIHU_PROC_SPEAK tihu_Speak;
    char* error;

    handle = dlopen(argv[1], RTLD_LAZY);
    if(!handle) {
        printf("Error: Tihu module cannot open.\n");
        return 1;
    }

    tihu_Init = (TIHU_PROC_INIT)dlsym(handle, "tihu_Init");
    tihu_Close = (TIHU_PROC_CLOSE)dlsym(handle, "tihu_Close");
    tihu_Callback = (TIHU_PROC_CALLBACK)dlsym(handle, "tihu_Callback");
    tihu_Speak = (TIHU_PROC_SPEAK)dlsym(handle, "tihu_Speak");

    if( tihu_Init == NULL ||
        tihu_Close == NULL ||
        tihu_Callback == NULL ||
        tihu_Speak == NULL)  {
        printf("Error: Tihu module cannot load.\n");
        return 1;
    }

    if (!tihu_Init()) {
        printf("Error: Tihu module cannot init.\n");
        return 1;
    }

    FILE* fp = fopen(argv[3], "wb");
    fwrite(WAVE_HEADER_WAV, sizeof(WAVE_HEADER_WAV), 1, fp);

    tihu_Callback(cb, fp);
    tihu_Speak(argv[2], TIHU_VOICE_MBROLA_MALE);
    tihu_Close();

    int file_size	= ftell(fp);
	int chunk_size	= file_size - 44;
	int wave_size	= file_size - 8;
    int format = 1;// pcm wave

	if(chunk_size % 4 != 0) {
		chunk_size -= 2;
	}

	fseek(fp, 20, SEEK_SET);
	fwrite(&format, 2, 1, fp);

	fseek(fp, 4, SEEK_SET);
	fwrite(&wave_size, 4, 1, fp);

	fseek(fp, 40, SEEK_SET);
	fwrite(&chunk_size, 4, 1, fp);
    fclose(fp);

    dlclose(handle);
    return 0;
}
