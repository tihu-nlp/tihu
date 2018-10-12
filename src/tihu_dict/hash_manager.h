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
#ifndef __TIHU__HASH_MANAGER_H
#define __TIHU__HASH_MANAGER_H

#include "../helper.h"
#include "../corpus/word.h"

struct hentry;

class CHashManager
{
public:
    CHashManager();

    bool LoadTable(const std::string  &filename, const std::string  &key);
    struct hentry* Lookup(const char* text) const;

private:

    int Hash(const char* word) const;

    int DecodeFlags(unsigned short** result, const std::string &flags) const;

    int AddWord(const std::string &word, const std::string &lemma,
                const std::string &pos, const std::string &pron,
                unsigned short* flags, short flags_len, int freq);

private:
    struct hentry** TablePtr;
    int TableSize;
};

#endif
