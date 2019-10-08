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
#include "espeak_syn.h"
#include "../../tihu.h"

int espeak_callback(short *samples, int length, espeak_EVENT *espeak_event) {
  CeSpeakSyn *synth = (CeSpeakSyn *)espeak_event->user_data;

  return synth->ParsEvent(samples, length, espeak_event);
}

CeSpeakSyn::CeSpeakSyn(CeSpeakLib *lib, std::string gender) : eSpeakLib(lib) {
  VoiceParam = gender;
}

CeSpeakSyn::~CeSpeakSyn() {}

bool CeSpeakSyn::Load() { return true; }

void CeSpeakSyn::ParsText(CCorpus *corpus) {
  if (corpus->IsEmpty()) {
    return;
  }

  espeak_VOICE voice;
  voice.languages = "fa-ir";
  voice.gender = (VoiceParam=="male")?1:2;

  eSpeakLib->SetCallback(espeak_callback);
  eSpeakLib->SetVoice(&voice);

  std::string phonetic;
  const CWordList &token_list = corpus->GetWordList();
  for (auto itt = token_list.begin(); itt != token_list.end(); ++itt) {
    if (IsStopped) {
      eSpeakLib->Stop();
      break; /// External stop
    }

    const CWordPtr &word = *itt;

    FireEvents(word);

    phonetic.clear();

    if (word->IsNonPersianWord()) {
      phonetic.append(word->GetText());
    } else {
      word->ParsPron();

      phonetic.append("[[");

      CPhonemeList &phoneme_list = word->GetPhonemeList();
      for (auto itp = phoneme_list.begin(); itp != phoneme_list.end(); ++itp) {
        CPhonemePtr &phoneme = *itp;
        // if(phoneme->GetIPAName() != "?")
        { phonetic.append(phoneme->GetIPAName()); }
      }

      phonetic.append("]]");
    }

    eSpeakLib->Synthesize(phonetic.c_str(), this);
  }
}

void CeSpeakSyn::ApplyChanges() {
  /// TODO
}

int CeSpeakSyn::GetFrequency() const {
  return eSpeakLib->GetFrequency(); //
}

int CeSpeakSyn::ParsEvent(short *samples, int length,
                          espeak_EVENT *espeak_event) {

  if (espeak_event->type != espeakEVENT_MSG_TERMINATED) {
    PlaySamples(samples, length);

    switch (espeak_event->type) {
    case espeakEVENT_PLAY: {

    } break;

    default:
      break;
    }
  }

  return 0;
}
