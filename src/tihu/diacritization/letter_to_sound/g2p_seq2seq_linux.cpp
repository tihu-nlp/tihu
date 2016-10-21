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
#include "g2p_seq2seq_linux.h"
//#include "path_manager.h"


#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ioctl.h>


#define PIPE_READ  0
#define PIPE_WRITE 1


Cg2pSeq2Seq::Cg2pSeq2Seq()
{
    p_stdin[PIPE_READ] = 0;
    p_stdin[PIPE_WRITE] = 0;
    p_stdout[PIPE_READ] = 0;
    p_stdout[PIPE_WRITE] = 0;
    pid = 0;
}

Cg2pSeq2Seq::~Cg2pSeq2Seq()
{
    if(p_stdin[PIPE_READ]) {
        int result = write(p_stdin[PIPE_WRITE], "\n", 1);
        (void)result;

        close(p_stdin[PIPE_WRITE]);
        close(p_stdout[PIPE_READ]);

        if (pid)
        {
            kill(pid, SIGTERM);
            waitpid(pid, NULL, 0);
            pid = 0;
        }
    }
}

bool Cg2pSeq2Seq::LoadModel(const std::string& model)
{
    if ( pipe( p_stdin ) != 0 )
    {
        perror( "pipe() to" );
        exit(255);
    }
    if ( pipe( p_stdout ) != 0 )
    {
        perror( "pipe() from" );
        exit(255);
    }

    pid = fork();
    if ( pid < 0 )
    {
        perror( "fork() 1" );
        exit(255);
    }
    else if ( pid == 0 )
    {
        /* dup pipe read/write to stdin/stdout */
        dup2( p_stdin [PIPE_READ], STDIN_FILENO );
        dup2( p_stdout[PIPE_WRITE], STDOUT_FILENO );
        //dup2( p_stdout[PIPE_WRITE], STDERR_FILENO );

        /* close unnecessary pipe descriptors for a clean environment */
        close( p_stdin[PIPE_READ] );
        close( p_stdin[PIPE_WRITE] );
        close( p_stdout[PIPE_READ] );
        close( p_stdout[PIPE_WRITE] );

        /// TODO:: check if model exists
        execlp("./g2p_seq2seq_pyinstaller_linux_64", "g2p_seq2seq_pyinstaller_linux_64",
               "--interactive", "--model", model.c_str(), (char *)NULL);

        exit(1);
    }
    else
    {
        close(p_stdin[PIPE_READ]);
        close(p_stdout[PIPE_WRITE]);
    }

    return true;
}

std::string Cg2pSeq2Seq::Convert(const std::string &word)
{
    std::string phonemes;

    ///
    if(!pid) {
        return phonemes;
    }

    if(word.length() == 0) {
        return phonemes;
    }

    char buf[256];
    buf[0]=0;

    snprintf(buf, 255, "%s\n", word.c_str());
    int result = write(p_stdin[PIPE_WRITE], buf, strlen(buf));
    if(result == -1) {
        TIHU_WARNING(stderr, "error on writting to g2p");
        return phonemes;
    }

    ///
    /// now try to read from stdout buffer
    /// HACK:
    ///   python has own buffering system for stdout.
    ///   you should flush it after print command
    ///
    ///   import sys
    ///
    ///   sys.stdout.flush()
    ///

    buf[0]=0;
    result = read(p_stdout[PIPE_READ], buf, 256);
    if (result == -1) {
        TIHU_WARNING(stderr, "error on reading from g2p");
        return phonemes;
    }
    buf[result]=0;

    ///
    /// find offset of the last line break (\n)
    ///
    int length = strlen(buf);
    int offset = 0;
    for(int i = 0; i < length-1; ++i) {
        if(buf[i] == '\n')
            offset = i+1;
    }

    offset++; //ignore '>' charcter
    for(int i = offset; i < length-1; ++i) {
        ++i;/// ignore space
        phonemes += buf[i];
    }

    return phonemes;
}
