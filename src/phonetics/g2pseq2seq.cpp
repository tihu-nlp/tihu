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
#include "g2pseq2seq.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

Cg2pSeq2Seq::Cg2pSeq2Seq() {
  p_stdin[PIPE_READ] = 0;
  p_stdin[PIPE_WRITE] = 0;
  p_stdout[PIPE_READ] = 0;
  p_stdout[PIPE_WRITE] = 0;
  pid = 0;
}

Cg2pSeq2Seq::~Cg2pSeq2Seq() {
  if (p_stdin[PIPE_READ]) {
    int result = write(p_stdin[PIPE_WRITE], "\n", 1);
    (void)result;

    close(p_stdin[PIPE_WRITE]);
    close(p_stdout[PIPE_READ]);

    if (pid) {
      kill(pid, SIGTERM);
      waitpid(pid, NULL, 0);
      pid = 0;
    }
  }
}

bool Cg2pSeq2Seq::LoadModel(const std::string &model) {
  if (pipe(p_stdin) != 0) {
    perror("pipe() to");
    exit(255);
  }
  if (pipe(p_stdout) != 0) {
    perror("pipe() from");
    exit(255);
  }

  pid = fork();
  if (pid < 0) {
    perror("fork() 1");
    exit(255);
  } else if (pid == 0) {
    /* dup pipe read/write to stdin/stdout */
    dup2(p_stdin[PIPE_READ], STDIN_FILENO);
    dup2(p_stdout[PIPE_WRITE], STDOUT_FILENO);

    /* close unnecessary pipe descriptors for a clean environment */
    close(p_stdin[PIPE_READ]);
    close(p_stdin[PIPE_WRITE]);
    close(p_stdout[PIPE_READ]);
    close(p_stdout[PIPE_WRITE]);

    /// TODO:: check if model exists
    int err = execlp("g2p-seq2seq", "g2p-seq2seq", "--interactive",
                     "--model_dir", model.c_str(), "2>/dev/null", (char *)NULL);

    if (err != 0) {
      TIHU_WARNING(stderr, "error loading g2p model: %d", err);
      exit(255);
    }

    exit(1);
  } else {
    // close(p_stdin[PIPE_READ]);
    // close(p_stdout[PIPE_WRITE]);
    // close(p_stderr[PIPE_WRITE]);
  }

  return true;
}

std::string Cg2pSeq2Seq::Convert(const std::string &word) {
  std::string phonemes;

  ///
  if (!pid) {
    return phonemes;
  }

  if (word.length() == 0) {
    return phonemes;
  }

  char buf[256];
  int result = 0;
  buf[0] = 0;

  snprintf(buf, 255, "%s\n", word.c_str());
  result = write(p_stdin[PIPE_WRITE], buf, strlen(buf));
  if (result == -1) {
    TIHU_WARNING(stderr, "error on writting to g2p: %s", buf);
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

  buf[0] = 0;
  result = read(p_stdout[PIPE_READ], buf, 256);
  if (result == -1) {
    TIHU_WARNING(stderr, "error on reading from g2p: %s", buf);
    return phonemes;
  }
  buf[result] = 0;

  if (result > 3) {
    // hello
    // >HH EH L OW
    for (int i = 2; i < result - 1; ++i) {
      phonemes += buf[i];
      ++i; /// ignore space
    }
  }

  return phonemes;
}
