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
#include "tihu.h"
#include "engine.h"

#include <mutex>
#include <dlfcn.h>
#include <Python.h>

static CEngine* g_engine = nullptr;
static int g_refcount = 0;
static int g_errorcode = TIHU_ERROR_NONE;
static std::mutex io_mutex;


const char ERROR_STRING[][256] = {
    "No Error.",
    "Loading lexicon failed",
    "Loading voice failed",
    "Loading user lexicon failed",
    "Loading settings failed",
};
#define MUTEX_LOCK std::lock_guard<std::mutex> lk(io_mutex);

TIHU_FN_DECLARE bool tihu_Init()
{
    MUTEX_LOCK

    if(g_refcount == 0) {
        /// fix crazy error: undefined symbol: PyFloat_Type
        void* p = dlopen( "libpython2.7.so", RTLD_LAZY | RTLD_GLOBAL );
        if (p == nullptr) {
            g_errorcode = TIHU_ERROR_NO_PYTHON;
            return false;
        }

        Py_Initialize();

        g_engine = new CEngine();
        g_errorcode = g_engine->LoadModules();

        if(g_errorcode != TIHU_ERROR_NONE) {
            delete g_engine;
            g_engine = nullptr;

            return false;
        }
    }

    g_refcount++;

    return true;
}

TIHU_FN_DECLARE bool tihu_LoadVoice(TIHU_VOICE voice)
{
    MUTEX_LOCK

    g_errorcode = g_engine->LoadSynthesizer(voice);

    if(g_errorcode != TIHU_ERROR_NONE) {
        return false;
    }

    return true;
}

TIHU_FN_DECLARE void tihu_Close()
{
    MUTEX_LOCK

    g_refcount--;

    if(g_refcount <= 0) {
        delete g_engine;
        g_engine = 0;
        g_refcount = 0; /// to make sure it won't be negative

        Py_Finalize();
    }
}

TIHU_FN_DECLARE void tihu_Speak(const char* text)
{
    MUTEX_LOCK

    if(!g_engine) {
        return;
    }

    g_engine->Speak(text);
}

TIHU_FN_DECLARE void tihu_Stop()
{
    if (!g_engine) {
        return;
    }

    g_engine->Stop();
}

TIHU_FN_DECLARE int tihu_GetLastError()
{
    return g_errorcode;
}

TIHU_FN_DECLARE const char* tihu_GetErrorString(int error_code)
{
    return ERROR_STRING[error_code];
}

TIHU_FN_DECLARE bool tihu_SetParam(TIHU_PARAM param, int value)
{
    if(!g_engine) {
        return false;
    }

    return g_engine->SetParam(param, value);
}

TIHU_FN_DECLARE bool tihu_GetParam(TIHU_PARAM param, int &value)
{
    if(!g_engine) {
        return false;
    }

    return g_engine->GetParam(param, value);
}

TIHU_FN_DECLARE void tihu_SetCallback(TIHU_CALLBACK call_back, void* user_data)
{
    MUTEX_LOCK

    if(!g_engine) {
        return;
    }

    g_engine->SetCallback(call_back, user_data);
}

TIHU_FN_DECLARE const char* tihu_GetVersion()
{
    return "Version 0.1";
}

TIHU_FN_DECLARE  void tihu_Dump(const char* filename)
{
    if (!g_engine) {
        return;
    }

    g_engine->Dump(filename);
}
