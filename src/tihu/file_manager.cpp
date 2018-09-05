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
#include "file_manager.h"

#include "helper.h"


int CFileManager::fail(const char* err, const char* par)
{
    fprintf(stderr, err, par);
    return -1;
}

CFileManager::CFileManager()
    : hin(NULL), LineNum(0)
{
}

int CFileManager::OpenFile(const std::string &filename, const std::string &key)
{
    fin.open(filename, std::ios_base::in);
    if(!fin.is_open()) {
        // check hzipped file
        std::string st(filename);
        st.append(HZIP_EXTENSION);
        hin = new Hunzip(st.c_str(), key.c_str());
    }
    if(!fin.is_open() && !hin->is_open()) {
        fail(MSG_OPEN, filename.c_str());
        return 1;
    }

    return 0;
}

CFileManager::~CFileManager()
{
    delete hin;
}

bool CFileManager::ReadLine()
{
    bool ret = false;
    Line.clear();

    if(fin.is_open()) {
        ret = static_cast<bool>(std::getline(fin, Line));
    } else if(hin->is_open()) {
        ret = hin->getline(Line);
    }

    if(ret) {
        if(LineNum == 0) {
            if(Line.compare(0, 3, "\xEF\xBB\xBF", 3) == 0) {
                Line.erase(0, 3);
            }
        }
        ++LineNum;

        mychomp(Line);

        // ignore blanks
        if(Line.length() == 0) {
            return ReadLine();
        }

        // ignore comment lines
        if(Line.compare(0, 1, "#") == 0) {
            return ReadLine();
        }
    }

    Iter = Line.begin();

    return ret;
}

int CFileManager::GetLineNum() const
{
    return LineNum;
}

std::string CFileManager::GetLine() const
{
    return Line;
}

std::string CFileManager::NextPiece()
{
    std::string piece;
    std::string::const_iterator start_piece = mystrsep(Line, Iter);
    if(start_piece != Line.end()) {
        piece = std::string(start_piece, Iter);
    }

    return piece;
}
