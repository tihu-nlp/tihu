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
#ifndef __TIHU__G2P_SEQ2SEQ_H
#define __TIHU__G2P_SEQ2SEQ_H

#pragma once

#include "../helper.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

class Cg2pSeq2Seq
{
public:
    Cg2pSeq2Seq();
    ~Cg2pSeq2Seq();

    bool LoadModel(const std::string& model);
    std::string Convert(const std::string &word);

private:
    int p_stdin[2];     /* pipe to feed the exec'ed program input */
    int p_stdout[2];    /* pipe to get the exec'ed program output */
    pid_t pid;
};

#endif
