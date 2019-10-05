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
#ifndef __TIHU__FILE_MANAGER_H
#define __TIHU__FILE_MANAGER_H

#include "helper.h"

#define BUFSIZE 65536

class CFileManager {
public:
  CFileManager();
  ~CFileManager();

  int OpenFile(const std::string &filename,
               const std::string &key = std::string());

  bool ReadLine();

  int GetLineNum() const;
  std::string GetLine() const;
  std::string NextPiece();

protected:
  int fail(const char *err, const char *par);

protected:
  std::ifstream fin;
  int LineNum;
  std::string Line;
  std::string::const_iterator Iter;
};
#endif
