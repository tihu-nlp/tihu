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
#ifndef __TIHU__PATH_MANAGER_H
#define __TIHU__PATH_MANAGER_H


#include "singleton.h"

#include <map>

#ifdef WIN32
#define DIR_SEP     '\\'
#else
#define DIR_SEP     '/'
#endif


class CPathManager
    : public CSingleton<CPathManager>
{
public:
    CPathManager();
    virtual ~CPathManager();

    void Initialize();

    std::string GetBuildFolder() const;
    std::string GetLogFolder() const;
    std::string GetDataFolder() const;    
    std::string GetTokensPath() const;

	std::string GetLogFile(const std::string& filename) const;

private:
    std::string GetPath(std::string dir_key) const;
    void SetPath(std::string dir_key, std::string dir_path);

private:
    std::string GetCurrentModulePath();

protected:
    std::map<std::string, std::string> Folders;
};

#endif
