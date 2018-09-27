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
#include "mbrola_syn.h"
#include "tihu.h"


#define CHANGE_RATE_RANGE(rate)     (rate) //  Do not change rate range
#define CHANGE_PITCH_RANGE(pitch)   (pitch) //  Do not change pitch range
#define CHANGE_VOLUME_RANGE(volume) (volume) // Do not change volume range


CMbrolaSyn::CMbrolaSyn()
{

}

CMbrolaSyn::~CMbrolaSyn()
{
    MbrolaLib.Finalize();
}

bool CMbrolaSyn::Load(std::string name)
{
    Name = name;

    std::string data_path = "./data/" + name;
    if(!MbrolaLib.Initialize(data_path)) {
        return false;
    }

    return true;
}


int CMbrolaSyn::GetFrequency() const
{
    return MbrolaLib.GetFrequency();
}

void CMbrolaSyn::ParsText(CCorpus* corpus)
{
    if(corpus->IsEmpty()) {
        return;
    }

    std::string non_persian;
    std::string phoneme_line;

    const CWordList &token_list = corpus->GetWordList();

    IsStop = false;
    for(auto itt = token_list.begin(); itt != token_list.end(); ++itt) {
        if(IsStop) {
            break; /// External stop
        }

        const CWordPtr &word = *itt;

        FireEvents(word);


        phoneme_line.clear();

        word->ParsPronunciation();

        CPhonemeList &phoneme_list = word->GetPhonemeList();
        for(auto itp = phoneme_list.begin(); itp != phoneme_list.end(); ++itp) {
            CPhonemePtr &phoneme = *itp;

            std::string mbr = phoneme->GetMbrolString();

            phoneme_line.append(mbr);
        }

        if(!Synthesize((char*)phoneme_line.c_str())) {
            break;
        }
        /// -------------------------
    }
}

bool CMbrolaSyn::Synthesize(char* line)
{
    static const int sample_length = 2048;
    static short samples[sample_length];
    int length = 0;

    MbrolaLib.Write(line);

    if(MbrolaLib.GetLastError() != 0) {
        char bufa[255];
        MbrolaLib.GetLastErrorStr(bufa, 255);

        ReportMessage("Synthesizer Error: %s", bufa);
    }

    MbrolaLib.Flush();

    while((length = MbrolaLib.Read(samples, sample_length)) > 0) {

        if(!PlaySamples(samples, length)) {
            MbrolaLib.Clear();

            return false;
        }
    }

    return true;
}

void CMbrolaSyn::Stop()
{
    IsStop = true;
    MbrolaLib.Clear();
}

void CMbrolaSyn::ApplyChanges()
{
    int volume = Settings->GetVolume();
    int rate = Settings->GetRate();
    int pitch = Settings->GetPitch();

    MbrolaLib.ApplyVolume(volume);
    MbrolaLib.ApplyRate(rate);
    MbrolaLib.ApplyPitch(pitch);
}
