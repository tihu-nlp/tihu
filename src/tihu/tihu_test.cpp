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

#include "tihu.h"

#ifdef WIN32
#else
#include <dlfcn.h>
#endif

int main(int argc, char** argv)
{
    void* handle;
    TIHU_PROC_GET_VERSION tihu_GetVersion;
    char* error;

    handle = dlopen(argv[1], RTLD_LAZY);
    if(!handle) {
        printf("Error: Tihu module cannot be opened.\n");
        fputs(dlerror(), stderr);
        fputs("\n", stderr);
        exit(1);
        return 1;
    }

    tihu_GetVersion = (TIHU_PROC_GET_VERSION)dlsym(handle, "tihu_GetVersion");
    if((error = dlerror()) != NULL)  {
        printf("Error: Tihu module cannot be loaded.\n");
        fputs(error, stderr);
        fputs("\n", stderr);
        exit(1);
        return 1;
    }

    printf("Tihu loaded successfully. %s\n", (*tihu_GetVersion)());

    dlclose(handle);
    return 0;
}
