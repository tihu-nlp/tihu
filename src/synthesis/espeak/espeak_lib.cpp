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
#include "espeak_lib.h"
#include "../../helper.h"

#include <cstring>

#ifndef WIN32
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#endif

CeSpeakLib::CeSpeakLib()
{
    Module                  = nullptr;
    procInitialize          = nullptr;
    procSetSynthCallback    = nullptr;
    procSetUriCallback      = nullptr;
    procSynth               = nullptr;
    procSynth_Mark          = nullptr;
    procKey                 = nullptr;
    procChar                = nullptr;
    procSetParameter        = nullptr;
    procGetParameter        = nullptr;
    procSetPunctuationList  = nullptr;
    procSetPhonemeTrace     = nullptr;
    procCompileDictionary   = nullptr;
    procListVoices          = nullptr;
    procSetVoiceByName      = nullptr;
    procSetVoiceByProperties= nullptr;
    procGetCurrentVoice     = nullptr;
    procCancel              = nullptr;
    procIsPlaying           = nullptr;
    procSynchronize         = nullptr;
    procTerminate           = nullptr;
    procInfo                = nullptr;

    Frequency               = -1;
}

CeSpeakLib::~CeSpeakLib()
{
    Finalize();
}

bool CeSpeakLib::Initialize(const char* data_path)
{
    Finalize();
#ifdef WIN32
    Module = LoadLibraryA("espeak.dll");

    if(!Module) {
        return false;
    }

    procInitialize          = (ESPEAK_PROC_INITIALIZE) GetProcAddress(Module, "espeak_Initialize");
    procSetSynthCallback    = (ESPEAK_PROC_SETSYNTHCALLBACK) GetProcAddress(Module, "espeak_SetSynthCallback");
    procSetUriCallback      = (ESPEAK_PROC_SETURICALLBACK) GetProcAddress(Module, "espeak_SetUriCallback");
    procSynth               = (ESPEAK_PROC_SYNTH) GetProcAddress(Module, "espeak_Synth");
    procSynth_Mark          = (ESPEAK_PROC_SYNTH_MARK) GetProcAddress(Module, "espeak_Synth_Mark");
    procKey                 = (ESPEAK_PROC_KEY) GetProcAddress(Module, "espeak_Key");
    procChar                = (ESPEAK_PROC_CHAR) GetProcAddress(Module, "espeak_Char");
    procSetParameter        = (ESPEAK_PROC_SETPARAMETER) GetProcAddress(Module, "espeak_SetParameter");
    procGetParameter        = (ESPEAK_PROC_GETPARAMETER) GetProcAddress(Module, "espeak_GetParameter");
    procSetPunctuationList  = (ESPEAK_PROC_SETPUNCTUATIONLIST) GetProcAddress(Module, "espeak_SetPunctuationList");
    procSetPhonemeTrace     = (ESPEAK_PROC_SETPHONEMETRACE) GetProcAddress(Module, "espeak_SetPhonemeTrace");
    procCompileDictionary   = (ESPEAK_PROC_COMPILEDICTIONARY) GetProcAddress(Module, "espeak_CompileDictionary");
    procListVoices          = (ESPEAK_PROC_LISTVOICES) GetProcAddress(Module, "espeak_ListVoices");
    procSetVoiceByName      = (ESPEAK_PROC_SETVOICEBYNAME) GetProcAddress(Module, "espeak_SetVoiceByName");
    procSetVoiceByProperties= (ESPEAK_PROC_SETVOICEBYPROPERTIES) GetProcAddress(Module, "espeak_SetVoiceByProperties");
    procGetCurrentVoice     = (ESPEAK_PROC_GETCURRENTVOICE) GetProcAddress(Module, "espeak_GetCurrentVoice");
    procCancel              = (ESPEAK_PROC_CANCEL) GetProcAddress(Module, "espeak_Cancel");
    procIsPlaying           = (ESPEAK_PROC_ISPLAYING) GetProcAddress(Module, "espeak_IsPlaying");
    procSynchronize         = (ESPEAK_PROC_SYNCHRONIZE) GetProcAddress(Module, "espeak_Synchronize");
    procTerminate           = (ESPEAK_PROC_TERMINATE) GetProcAddress(Module, "espeak_Terminate");
    procInfo                = (ESPEAK_PROC_INFO) GetProcAddress(Module, "espeak_Info");
#else

    Module = dlopen("libespeak.so", RTLD_LAZY);

    if(!Module) {
        fputs(dlerror(), stderr);
        return false;
    }

    procInitialize          = (ESPEAK_PROC_INITIALIZE) dlsym(Module, "espeak_Initialize");
    procSetSynthCallback    = (ESPEAK_PROC_SETSYNTHCALLBACK) dlsym(Module, "espeak_SetSynthCallback");
    procSetUriCallback      = (ESPEAK_PROC_SETURICALLBACK) dlsym(Module, "espeak_SetUriCallback");
    procSynth               = (ESPEAK_PROC_SYNTH) dlsym(Module, "espeak_Synth");
    procSynth_Mark          = (ESPEAK_PROC_SYNTH_MARK) dlsym(Module, "espeak_Synth_Mark");
    procKey                 = (ESPEAK_PROC_KEY) dlsym(Module, "espeak_Key");
    procChar                = (ESPEAK_PROC_CHAR) dlsym(Module, "espeak_Char");
    procSetParameter        = (ESPEAK_PROC_SETPARAMETER) dlsym(Module, "espeak_SetParameter");
    procGetParameter        = (ESPEAK_PROC_GETPARAMETER) dlsym(Module, "espeak_GetParameter");
    procSetPunctuationList  = (ESPEAK_PROC_SETPUNCTUATIONLIST) dlsym(Module, "espeak_SetPunctuationList");
    procSetPhonemeTrace     = (ESPEAK_PROC_SETPHONEMETRACE) dlsym(Module, "espeak_SetPhonemeTrace");
    procCompileDictionary   = (ESPEAK_PROC_COMPILEDICTIONARY) dlsym(Module, "espeak_CompileDictionary");
    procListVoices          = (ESPEAK_PROC_LISTVOICES) dlsym(Module, "espeak_ListVoices");
    procSetVoiceByName      = (ESPEAK_PROC_SETVOICEBYNAME) dlsym(Module, "espeak_SetVoiceByName");
    procSetVoiceByProperties= (ESPEAK_PROC_SETVOICEBYPROPERTIES) dlsym(Module, "espeak_SetVoiceByProperties");
    procGetCurrentVoice     = (ESPEAK_PROC_GETCURRENTVOICE) dlsym(Module, "espeak_GetCurrentVoice");
    procCancel              = (ESPEAK_PROC_CANCEL) dlsym(Module, "espeak_Cancel");
    procIsPlaying           = (ESPEAK_PROC_ISPLAYING) dlsym(Module, "espeak_IsPlaying");
    procSynchronize         = (ESPEAK_PROC_SYNCHRONIZE) dlsym(Module, "espeak_Synchronize");
    procTerminate           = (ESPEAK_PROC_TERMINATE) dlsym(Module, "espeak_Terminate");
    procInfo                = (ESPEAK_PROC_INFO) dlsym(Module, "espeak_Info");
#endif




    if(!procInitialize             ||
       !procInitialize             ||
       !procSetSynthCallback       ||
       !procSetUriCallback         ||
       !procSynth                  ||
       !procSynth_Mark             ||
       !procKey                    ||
       !procChar                   ||
       !procSetParameter           ||
       !procGetParameter           ||
       !procSetPunctuationList     ||
       !procSetPhonemeTrace        ||
       !procCompileDictionary      ||
       !procListVoices             ||
       !procSetVoiceByName         ||
       !procSetVoiceByProperties   ||
       !procGetCurrentVoice        ||
       !procCancel                 ||
       !procIsPlaying              ||
       !procSynchronize            ||
       !procTerminate              ||
       !procInfo) {
        Finalize();
        return false;
    }

    Frequency = procInitialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, data_path, 0);

    if(Frequency == -1) {
        return false;
    }

    return true;
}

void CeSpeakLib::Finalize()
{
    if(Module) {
        procTerminate();
#ifdef WIN32
        FreeLibrary(Module);
#else
        dlclose(Module);
#endif
        Module =  nullptr;
    }
}

void CeSpeakLib::SetCallback(t_espeak_callback callback)
{
    procSetSynthCallback(callback);
}

bool CeSpeakLib::Synthesize(const char* text, void* user_data)
{
    if(!procSynth){
        TIHU_WARNING(stderr, "eSpeak hasn't' loaded properly.");
        return false;
    }

    int error =
        procSynth(text, strlen(text),
                  0, POS_CHARACTER, 0, espeakPHONEMES, 0, user_data);

    return (error == EE_OK);
}

void CeSpeakLib::Stop()
{
    if(procCancel){
        procCancel();
    }
}

int CeSpeakLib::GetFrequency() const
{
    return Frequency;
}
