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
#include "path_manager.h"

#include "helper.h"


#ifdef WIN32
#include <Windows.h>
#else
#include "tihu.h"
#include <dlfcn.h>
#endif


#define BUILD_FOLDER_KEY          "BUILD_PATH"
#define TEMP_FOLDER_KEY           "TEMP_PATH"
#define LOG_FOLDER_KEY            "LOG_PATH"
#define DATA_FOLDER_KEY           "DATA_PATH"



CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
}

void CPathManager::Initialize()
{
    Folders.clear();

#ifdef __ANDROID__

#endif

    std::string build_directory = GetCurrentModulePath();

    std::string build_path         = build_directory;
    std::string temp_path          = build_directory + "temp" + DIR_SEP;
    std::string log_path           = build_directory + "log" + DIR_SEP;
    std::string data_path          = build_directory + "data" + DIR_SEP;

    SetPath(BUILD_FOLDER_KEY, build_path);
    SetPath(TEMP_FOLDER_KEY, temp_path);
    SetPath(LOG_FOLDER_KEY, log_path);
    SetPath(DATA_FOLDER_KEY, data_path);
}

std::string CPathManager::GetBuildFolder() const
{
    return GetPath(BUILD_FOLDER_KEY);
}

std::string CPathManager::GetLogFolder() const
{
    return GetPath(LOG_FOLDER_KEY);
}

std::string CPathManager::GetDataFolder() const
{
    return GetPath(DATA_FOLDER_KEY);
}

std::string CPathManager::GetLogFile(const std::string& filename) const
{
	std::string path = GetLogFolder();
	path.append(filename);

	return path;
}

std::string CPathManager::GetPath(std::string dir_key) const
{
    dir_key = ToUpper(dir_key);

    if(Folders.find(dir_key) == Folders.end()) {
        return std::string();
    }

    return Folders.at(dir_key);
}

void CPathManager::SetPath(std::string dir_key, std::string dir_path)
{
    dir_key = ToUpper(dir_key);

    Folders.insert(std::pair<std::string, std::string>(dir_key, dir_path));
}


std::string CPathManager::GetCurrentModulePath()
{
    std::string path;

#ifdef WIN32
    char buffer[_MAX_PATH];
    buffer[0] = 0;

    HMODULE module = NULL;
    if(!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                           GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           "tihu.dll",
                           &module)) {
        int ret = GetLastError();
        fprintf(stderr, "GetModuleHandle returned %d\n", ret);
        return path;
    }

    if(!GetModuleFileNameA(module, buffer, _MAX_PATH)) {
        return path;
    }

    path = buffer;
#else
    Dl_info dl_info;
    if(!dladdr((void*)tihu_Init, &dl_info)) {
        return path;
    }

    path = dl_info.dli_fname;
#endif

    size_t found=path.find_last_of("/\\");

    return path.substr(0,found+1);
}
