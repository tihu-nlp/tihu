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
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/tihu.h"

#ifdef WIN32
#else
#include <dlfcn.h>
#endif

#define WAVE_HDR_SIZE 44

static const unsigned char wav_hdr[WAVE_HDR_SIZE] = {
    'R',  'I',  'F',
    'F', /* Chunk ID : "RIFF" */
    '\0', '\0', '\0',
    '\0', /* Chunk size = file size - 8 */
    'W',  'A',  'V',
    'E', /* Chunk format : "WAVE" */
    'f',  'm',  't',
    ' ', /*   Subchunk ID : "fmt " */
    0x10, 0x00, 0x00,
    0x00,       /*   Subchunk size : 16 for PCM format */
    0x01, 0x00, /*     Audio format : 1 means PCM linear */
    '\0', '\0', /*     Number of channels */
    '\0', '\0', '\0',
    '\0', /*     Sample rate */
    '\0', '\0', '\0',
    '\0',       /*     Byte rate = SampleRate * NumChannels * BitsPerSample/8 */
    '\0', '\0', /*     Blockalign = NumChannels * BitsPerSample/8 */
    '\0', '\0', /*     Bitpersample */
    'd',  'a',  't',
    'a', /*   Subchunk ID : "data" */
    '\0', '\0', '\0',
    '\0' /*   Subchunk size = NumSamples * NumChannels * BitsPerSample/8 */
};

void calc_wav_header(unsigned char header[WAVE_HDR_SIZE],
                     unsigned char nb_channels, unsigned long sample_rate,
                     unsigned short bits_per_sample, int data_size) {
    int byte_rate;
    int block_align;
    if (bits_per_sample == 8) {
        byte_rate = nb_channels * sizeof(char) * sample_rate;
        block_align = nb_channels * sizeof(char);
    } else {
        byte_rate = nb_channels * sizeof(short) * sample_rate;
        block_align = nb_channels * sizeof(short);
    }

    // int data_size = voice_count;
    int chunk_size = data_size + 36;

    /* Copy the standard header */
    memcpy(header, wav_hdr, WAVE_HDR_SIZE);
    // Update wav header section
    header[4] = (unsigned char)chunk_size;
    header[5] = (unsigned char)(chunk_size >> 8);
    header[6] = (unsigned char)(chunk_size >> 16);
    header[7] = (unsigned char)(chunk_size >> 24);

    header[22] = (unsigned char)nb_channels;
    header[23] = 0; /* nb_channels is coded on 1 byte only */

    header[24] = (unsigned char)sample_rate;
    header[25] = (unsigned char)(sample_rate >> 8);
    header[26] = (unsigned char)(sample_rate >> 16);
    header[27] = (unsigned char)(sample_rate >> 24);

    header[28] = (unsigned char)byte_rate;
    header[29] = (unsigned char)(byte_rate >> 8);
    header[30] = (unsigned char)(byte_rate >> 16);
    header[31] = (unsigned char)(byte_rate >> 24);

    header[32] = (unsigned char)block_align;
    header[33] = (unsigned char)(block_align >> 8);

    header[34] = (unsigned char)bits_per_sample;
    header[35] = (unsigned char)(bits_per_sample >> 8);

    header[40] = (unsigned char)data_size;
    header[41] = (unsigned char)(data_size >> 8);
    header[42] = (unsigned char)(data_size >> 16);
    header[43] = (unsigned char)(data_size >> 24);
}

TIHU_CALLBACK_RETURN cb(TIHU_CALLBACK_MESSAGE message, long l_param,
                        long w_param, void *user_data) {
    FILE *fp = (FILE *)user_data;

    if (message == TIHU_WAVE_BUFFER) {
        fwrite((char *)l_param, sizeof(char), w_param, fp);
    }
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Error: Not enough arguments.\nex: ./tihu_play ./libtihu.so "
               "\"سلام\" /tmp/raw1\n");
        return 1;
    }

    void *handle;
    TIHU_PROC_INIT tihu_Init;
    TIHU_PROC_CLOSE tihu_Close;
    TIHU_PROC_CALLBACK tihu_Callback;
    TIHU_PROC_SPEAK tihu_Speak;
    char *error;

    handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        printf("Error: Tihu module cannot open.\n");
        return 1;
    }

    tihu_Init = (TIHU_PROC_INIT)dlsym(handle, "tihu_Init");
    tihu_Close = (TIHU_PROC_CLOSE)dlsym(handle, "tihu_Close");
    tihu_Callback = (TIHU_PROC_CALLBACK)dlsym(handle, "tihu_Callback");
    tihu_Speak = (TIHU_PROC_SPEAK)dlsym(handle, "tihu_Speak");

    if (tihu_Init == NULL || tihu_Close == NULL || tihu_Callback == NULL ||
        tihu_Speak == NULL) {
        printf("Error: Tihu module cannot load.\n");
        return 1;
    }

    if (!tihu_Init()) {
        printf("Error: Tihu module cannot init.\n");
        return 1;
    }

    unsigned char wav_hdr_1[WAVE_HDR_SIZE];
    FILE *fp = fopen(argv[3], "wb");
    fwrite(wav_hdr_1, WAVE_HDR_SIZE, 1, fp);

    tihu_Callback(cb, fp);
    tihu_Speak(argv[2], TIHU_VOICE_MBROLA_FEMALE);
    tihu_Close();

    int data_size = ftell(fp) - 44;

    // mbrola male
    //calc_wav_header(wav_hdr_1, 1, 16000, 16, data_size);
    // mbrola female
    calc_wav_header(wav_hdr_1, 1, 22050, 16, data_size);

    fseek(fp, 0, SEEK_SET);
    fwrite(wav_hdr_1, WAVE_HDR_SIZE, 1, fp);

    dlclose(handle);
    return 0;
}
