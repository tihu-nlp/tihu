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
#include "parser.h"
#include "settings.h"
#include "tihu.h"

#include <stdarg.h>

IParser::IParser()
{
    Callback = nullptr;
    UserData = nullptr;
    Settings = nullptr;
}

IParser::~IParser()
{

}

void IParser::SetCallBack(TIHU_CALLBACK callback, void* userdata)
{
    Callback = callback;
    UserData = userdata;
}

void IParser::SetSettings(CSettings* settings)
{
    Settings = settings;
}

void IParser::ReportMessage(const char* message, ...) const
{
    char temp[1024];

    va_list arglist;
    va_start(arglist, message);
    vsnprintf(temp, 1024, message, arglist);
    va_end(arglist);

    ////wcscat(temp, L"\r\n");

    if(Callback) {
        static_cast<TIHU_CALLBACK>(Callback)
            (TIHU_TEXT_MESSAGE, (long)temp, strlen(temp)-1, UserData);
    }
}
