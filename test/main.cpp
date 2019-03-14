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
#include <algorithm>
#include <cstring>
#include <dlfcn.h>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace fs = std::experimental::filesystem;

#include "../src/tihu.h"

bool compareFiles(const std::string &p1, const std::string &p2) {
    std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; // file problem
    }

    if (f1.tellg() != f2.tellg()) {
        return false; // size mismatch
    }

    // seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

std::string read_file(std::string fname) {
    std::ifstream infile(fname);
    std::string line, text;
    while (std::getline(infile, line)) {
        if (line.front() != '#') {
            text += line;
            text += "\n";
        }
    }
    return text;
}


TIHU_CALLBACK_RETURN cb(TIHU_CALLBACK_MESSAGE message, long l_param,
                        long w_param, void *data) {
    char *str = (char *)l_param;
    FILE *fp = (FILE *)data;

    fwrite(str, 1, strlen(str), fp);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Error: Not enough arguments.\nex: ./tihu_test ./libtihu.so "
               "../test/res\n");
        return 1;
    }

    void *handle;
    TIHU_PROC_INIT tihu_Init;
    TIHU_PROC_CLOSE tihu_Close;
    TIHU_PROC_CALLBACK tihu_Callback;
    TIHU_PROC_TAG tihu_Tag;
    char *error;

    handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        printf("Error: Tihu module cannot open.\n");
        return 1;
    }

    tihu_Init = (TIHU_PROC_INIT)dlsym(handle, "tihu_Init");
    tihu_Close = (TIHU_PROC_CLOSE)dlsym(handle, "tihu_Close");
    tihu_Callback = (TIHU_PROC_CALLBACK)dlsym(handle, "tihu_Callback");
    tihu_Tag = (TIHU_PROC_TAG)dlsym(handle, "tihu_Tag");

    if (tihu_Init == NULL || tihu_Close == NULL || tihu_Callback == NULL ||
        tihu_Tag == NULL) {
        printf("Error: Tihu module cannot load.\n");
        return 1;
    }

    if (!tihu_Init()) {
        printf("Error: Tihu module cannot init.\n");
        return 1;
    }

    int ret = 0;
    std::string path = argv[2];
    for (auto &p : fs::directory_iterator(path)) {
        std::string fname = fs::path(p).filename();
        std::string ext = fs::path(p).extension();

        if (ext == ".txt") {
            std::string txt = read_file(p.path());
            std::string lbl1 = fs::path(p).replace_extension(".lbl");
            std::string lbl2 = "/tmp/" + fname + ".lbl";

            FILE *fp = fopen(lbl2.c_str(), "w");

            printf("Testing %s ... ", fname.c_str());

            tihu_Callback(cb, (void *)fp);
            tihu_Tag(txt.c_str());

            fclose(fp);

            bool equal = compareFiles(lbl1, lbl2);
            if (!equal) {
                printf("FAILED\n");
                ret = 1;
            } else {
                printf("OK\n");
            }
        }
    }

    tihu_Close();

    return ret;
}
