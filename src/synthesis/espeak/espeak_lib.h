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
#ifndef __TIHU__ESPEAK_LIB_H
#define __TIHU__ESPEAK_LIB_H

#pragma once

#include <espeak-ng/espeak_ng.h>

#include <string>

typedef int (*ESPEAK_PROC_INITIALIZE)(espeak_AUDIO_OUTPUT output, int buflength,
                                      const char *path, int options);
typedef void (*ESPEAK_PROC_SETSYNTHCALLBACK)(t_espeak_callback *SynthCallback);
typedef void (*ESPEAK_PROC_SETSYNTHCALLBACK)(t_espeak_callback *SynthCallback);
typedef void (*ESPEAK_PROC_SETURICALLBACK)(int (*UriCallback)(int, const char *,
                                                              const char *));
typedef espeak_ERROR (*ESPEAK_PROC_SYNTH)(
    const void *text, size_t size, unsigned int position,
    espeak_POSITION_TYPE position_type, unsigned int end_position,
    unsigned int flags, unsigned int *unique_identifier, void *user_data);
typedef espeak_ERROR (*ESPEAK_PROC_SYNTH_MARK)(const void *text, size_t size,
                                               const char *index_mark,
                                               unsigned int end_position,
                                               unsigned int flags,
                                               unsigned int *unique_identifier,
                                               void *user_data);
typedef espeak_ERROR (*ESPEAK_PROC_KEY)(const char *key_name);
typedef espeak_ERROR (*ESPEAK_PROC_CHAR)(wchar_t character);
typedef espeak_ERROR (*ESPEAK_PROC_SETPARAMETER)(espeak_PARAMETER parameter,
                                                 int value, int relative);
typedef int (*ESPEAK_PROC_GETPARAMETER)(espeak_PARAMETER parameter,
                                        int current);
typedef espeak_ERROR (*ESPEAK_PROC_SETPUNCTUATIONLIST)(
    const wchar_t *punctlist);
typedef void (*ESPEAK_PROC_SETPHONEMETRACE)(int value, FILE *stream);
typedef void (*ESPEAK_PROC_COMPILEDICTIONARY)(const char *path, FILE *log,
                                              int flags);
typedef const espeak_VOICE **(*ESPEAK_PROC_LISTVOICES)(
    espeak_VOICE *voice_spec);
typedef espeak_ERROR (*ESPEAK_PROC_SETVOICEBYNAME)(const char *name);
typedef espeak_ERROR (*ESPEAK_PROC_SETVOICEBYPROPERTIES)(
    espeak_VOICE *voice_spec);
typedef espeak_VOICE *(*ESPEAK_PROC_GETCURRENTVOICE)(void);
typedef espeak_ERROR (*ESPEAK_PROC_CANCEL)(void);
typedef int (*ESPEAK_PROC_ISPLAYING)(void);
typedef espeak_ERROR (*ESPEAK_PROC_SYNCHRONIZE)(void);
typedef espeak_ERROR (*ESPEAK_PROC_TERMINATE)(void);
typedef const char *(*ESPEAK_PROC_INFO)(const char **path_data);

class CeSpeakLib {
public:
  CeSpeakLib();
  virtual ~CeSpeakLib();

  bool Initialize();
  void Finalize();

  void SetCallback(t_espeak_callback callback);
  void SetVoice(espeak_VOICE* voice);

  bool Synthesize(const char *text, void *user_data);

  void Stop();

  int GetFrequency() const;

protected:
private:
  void *Module;

  ESPEAK_PROC_INITIALIZE procInitialize;
  ESPEAK_PROC_SETSYNTHCALLBACK procSetSynthCallback;
  ESPEAK_PROC_SETURICALLBACK procSetUriCallback;
  ESPEAK_PROC_SYNTH procSynth;
  ESPEAK_PROC_SYNTH_MARK procSynth_Mark;
  ESPEAK_PROC_KEY procKey;
  ESPEAK_PROC_CHAR procChar;
  ESPEAK_PROC_SETPARAMETER procSetParameter;
  ESPEAK_PROC_GETPARAMETER procGetParameter;
  ESPEAK_PROC_SETPUNCTUATIONLIST procSetPunctuationList;
  ESPEAK_PROC_SETPHONEMETRACE procSetPhonemeTrace;
  ESPEAK_PROC_COMPILEDICTIONARY procCompileDictionary;
  ESPEAK_PROC_LISTVOICES procListVoices;
  ESPEAK_PROC_SETVOICEBYNAME procSetVoiceByName;
  ESPEAK_PROC_SETVOICEBYPROPERTIES procSetVoiceByProperties;
  ESPEAK_PROC_GETCURRENTVOICE procGetCurrentVoice;
  ESPEAK_PROC_CANCEL procCancel;
  ESPEAK_PROC_ISPLAYING procIsPlaying;
  ESPEAK_PROC_SYNCHRONIZE procSynchronize;
  ESPEAK_PROC_TERMINATE procTerminate;
  ESPEAK_PROC_INFO procInfo;

  int Frequency;
};

#endif
