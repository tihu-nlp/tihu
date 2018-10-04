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
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include "../src/tihu.h"

std::string read_file(std::string fname) {
    std::ifstream infile(fname);
    std::string line, text;
    while (std::getline(infile, line))
    {
        if (line.front() != '#') {
            text += line;
        }
    }
    return text;
}


TIHU_CALLBACK_RETURN cb(TIHU_CALLBACK_MESSAGE message, long l_param, long w_param, void* user_data)
{

}

int main(int argc, char** argv)
{
    if (argc < 3) {
        printf("Error: Not enough arguments.\nex: ./tihu_test ./libtihu.so ../test/res\n");
        return 1;
    }

    void* handle;
    TIHU_PROC_INIT tihu_Init;
    TIHU_PROC_CLOSE tihu_Close;
    TIHU_PROC_SET_CALLBACK tihu_SetCallback;
    TIHU_PROC_SPEAK tihu_Tag;
    char* error;

    handle = dlopen(argv[1], RTLD_LAZY);
    if(!handle) {
        printf("Error: Tihu module cannot open.\n");
        return 1;
    }

    tihu_Init = (TIHU_PROC_INIT)dlsym(handle, "tihu_Init");
    tihu_Close = (TIHU_PROC_CLOSE)dlsym(handle, "tihu_Close");
    tihu_SetCallback = (TIHU_PROC_SET_CALLBACK)dlsym(handle, "tihu_SetCallback");
    tihu_Tag = (TIHU_PROC_TAG)dlsym(handle, "tihu_Tag");

    if( tihu_Init == NULL ||
        tihu_Close == NULL ||
        tihu_SetCallback == NULL ||
        tihu_Tag == NULL)  {
        printf("Error: Tihu module cannot load.\n");
        return 1;
    }

    if (!tihu_Init()) {
        printf("Error: Tihu module cannot init.\n");
        return 1;
    }

    std::string path = argv[2];
    for (auto & p : fs::directory_iterator(path)) {
        std::string fname = fs::path(p).filename();
        std::string ext = fs::path(p).extension();

        if (ext == ".txt") {
            std::string txt = read_file(p.path());
            printf("%s\n\n", txt.c_str());
            tihu_SetCallback(cb, 0);
            tihu_Tag(txt.c_str());
        }
    }

    tihu_Close();

    return 0;
}


