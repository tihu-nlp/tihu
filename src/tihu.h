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
#ifndef TIHU_H
#define TIHU_H

#pragma once

#define TIHU_FN_DECLARE

enum TIHU_ERROR {
    TIHU_ERR_NONE                 = 0,
    TIHU_ERR_LOADING                 ,
    TIHU_ERR_LOAD_USER_DIC           ,
    TIHU_ERR_NO_PYTHON               ,
};

enum TIHU_PARAM {
    TIHU_PARAM_PITCH                 = 0,
    TIHU_PARAM_VOLUME                   ,
    TIHU_PARAM_RATE                     ,
    TIHU_PARAM_READ_PUNCS               ,
    TIHU_PARAM_FREQUENCY                ,
};

enum TIHU_CALLBACK_RETURN {
    TIHU_DATA_NOT_PROCESSED         = 0,
    TIHU_DATA_PROCESSED                ,
    TIHU_DATA_ABORT                    ,
};

enum TIHU_CALLBACK_MESSAGE {
    TIHU_WAVE_BUFFER                  = 0,
    TIHU_TEXT_MESSAGE                    ,
    TIHU_TEXT_TAGS                       ,
    TIHU_EVENT_WORD_BOUNDARY             ,
    TIHU_EVENT_SENTENCE_BOUNDARY         ,
    TIHU_EVENT_BOOKMARK                  ,
};

enum TIHU_VOICE {
    TIHU_VOICE_MBROLA_MALE               ,
    TIHU_VOICE_MBROLA_FEMALE             ,
    TIHU_VOICE_ESPEAK_MALE               ,
    TIHU_VOICE_ESPEAK_FEMALE             ,

    TIHU_VOICE_COUNT                     ,
};

typedef TIHU_CALLBACK_RETURN(*TIHU_CALLBACK)(TIHU_CALLBACK_MESSAGE message, long l_param, long w_param, void* user_data);


typedef bool (*TIHU_PROC_INIT)();
typedef void (*TIHU_PROC_CLOSE)();
typedef void (*TIHU_PROC_CALLBACK)(TIHU_CALLBACK, void*);
typedef void (*TIHU_PROC_TAG)(const char*);
typedef void (*TIHU_PROC_SPEAK)(const char*, TIHU_VOICE);
typedef void (*TIHU_PROC_STOP)();
typedef bool (*TIHU_PROC_SET_PARAM)(TIHU_PARAM, int);
typedef bool (*TIHU_PROC_GET_PARAM)(TIHU_PARAM, int &);
typedef int (*TIHU_PROC_LAST_ERROR)();
typedef const char* (*TIHU_PROC_ERROR_STRING)(int);
typedef const char* (*TIHU_PROC_VERSION)();
typedef void (*TIHU_PROC_ENABLE_DEBUG_MODE)(bool);

#ifdef __cplusplus
extern "C" {
#endif

TIHU_FN_DECLARE bool         tihu_Init();
TIHU_FN_DECLARE void         tihu_Close();
TIHU_FN_DECLARE bool         tihu_LoadUserDic(const char* user_dic);
TIHU_FN_DECLARE void         tihu_Callback(TIHU_CALLBACK callback, void* user_data);
TIHU_FN_DECLARE void         tihu_Speak(const char* text, TIHU_VOICE voice);
TIHU_FN_DECLARE void         tihu_Tag(const char* text);
TIHU_FN_DECLARE void         tihu_Stop();
TIHU_FN_DECLARE bool         tihu_SetParam(TIHU_PARAM param, int value);
TIHU_FN_DECLARE bool         tihu_GetParam(TIHU_PARAM param, int &value);
TIHU_FN_DECLARE int          tihu_LastError();
TIHU_FN_DECLARE const char*  tihu_ErrorString(int error_code);
TIHU_FN_DECLARE const char*  tihu_Version();
TIHU_FN_DECLARE void         tihu_EnableDebugMode(bool enable);


#ifdef __cplusplus
}
#endif

#endif
