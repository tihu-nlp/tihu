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
#include "../tihu.h"

#include <stdarg.h>
#include <math.h>

ISynthesizer::ISynthesizer() {
    SampleCounter = 0;
    int error = 0;
    SRCState = src_new(SRC_SINC_BEST_QUALITY, 1, &error);
	if(SRCState == nullptr) {
		printf ("Error : src_new() failed : %s.\n", src_strerror(error)) ;
	}
}

ISynthesizer::~ISynthesizer() {
    if(SRCState) {
		src_delete(SRCState);
	}
}

void ISynthesizer::FireEvents(const CWordPtr &word) const {
    if (Callback) {
        /// word boundary
        (Callback)(TIHU_EVENT_WORD_BOUNDARY, word->GetOffset(),
                   word->GetLength());
    }
}

bool ISynthesizer::PlaySamples(short *samples, int length) const {
    if (!Callback) {
        return false;
    }

    if (this->GetFrequency() == Settings->GetFrequency() ||
        SRCState == nullptr) {

        if ((Callback)(TIHU_WAVE_BUFFER, reinterpret_cast<long>(samples),
                    length * 2) == TIHU_DATA_ABORT) {
            return false;
        }
    } else {
        const int in_channels = 1;
        const int out_channels = 1;
        const int buf_size = 4096;
        SRC_DATA src_data;
        int error = 0;

        float input [buf_size];
        float output[buf_size];
        short output_s[buf_size];

        src_short_to_float_array(samples, input, length);

        src_data.end_of_input	= 0;
        src_data.input_frames	= length / in_channels;
        src_data.data_in		= input;
        src_data.data_out		= output;
        src_data.src_ratio		= (1.0F * Settings->GetFrequency()) / this->GetFrequency();
        src_data.output_frames	= buf_size / out_channels;
        // if each frame was 16 bits per sample, each sample has two chanel, them each frame has 2 * 16 bits length

        while (1) {
            error = src_process (SRCState, &src_data);
            if (error != 0) {
                printf ("\nError : %s\n", src_strerror (error)) ;
            };

            // Terminate if done.
            if(src_data.end_of_input || src_data.output_frames_gen == 0)
                break;

            src_float_to_short_array(output, output_s, src_data.output_frames_gen);

            if ((Callback)(TIHU_WAVE_BUFFER, reinterpret_cast<long>(output_s),
                    src_data.output_frames_gen*2) == TIHU_DATA_ABORT) {
                return false;
            }

            src_data.data_in += src_data.input_frames_used * in_channels;
            src_data.input_frames -= src_data.input_frames_used ;
        }
    }

    return true;
}
