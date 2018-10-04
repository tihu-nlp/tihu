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
#include "hash_manager.h"
#include "hentry.h"
#include "../file_manager.h"
#include "../corpus/word.h"

#include <algorithm>

// lookup a root word in the hashtable

CHashManager::CHashManager()
{
    TablePtr = nullptr;
    TableSize = 0;
}

struct hentry* CHashManager::Lookup(const char* text) const {
    struct hentry* dp;
    if(TablePtr)
    {
        dp = TablePtr[Hash(text)];
        if(!dp) {
            return NULL;
        }
        for(; dp != NULL; dp = dp->next) {
            if(strcmp(text, dp->text) == 0) {
                return dp;
            }
        }
    }
    return NULL;
}

// the hash function is a simple load and rotate
// algorithm borrowed
int CHashManager::Hash(const char* word) const
{
    unsigned long hv = 0;
    for(int i = 0; i < 4 && *word != 0; i++) {
        hv = (hv << 8) | (*word++);
    }
    while(*word != 0) {
        ROTATE(hv, ROTATE_LEN);
        hv ^= (*word++);
    }
    return (unsigned long)hv % TableSize;
}

// load a munched word list and build a hash table on the fly
bool CHashManager::LoadTable(const std::string  &filename, const std::string  &key)
{
    // open dictionary file
    CFileManager file_manager;

    if(file_manager.OpenFile(filename, key)) {
        return false;
    }

    file_manager.ReadLine();

    TableSize = std::stoi(file_manager.NextPiece());

    if(TableSize == 0) {
        TIHU_WARNING(stderr, "error: empty dic file %s\n", filename.c_str());
        return false;
    }

    TableSize += USERWORD + 5;

    if((TableSize % 2) == 0) {
        TableSize++;
    }

    if(TableSize <= 0 ||
       (TableSize >= (std::numeric_limits<int>::max() - 1) /
        int(sizeof(struct hentry*)))) {
        TIHU_WARNING(
            stderr, "error: line 1: missing or bad word count in the dic file\n");
        return false;
    }



    // allocate the hash table
    TablePtr = (struct hentry**)calloc(TableSize, sizeof(struct hentry*));
    if(!TablePtr) {
        return false;
    }

    std::string word, label, pron, freq;

    while(file_manager.ReadLine()) {
        word  = file_manager.NextPiece();
        label = file_manager.NextPiece();
        pron  = file_manager.NextPiece();
        freq  = file_manager.NextPiece();

        // split each token into word and affix char strings
        // "\/" signs slash in words (not affix separator)
        size_t ap_pos = word.find('/');
        unsigned short* flags;
        int al;
        if(ap_pos != std::string::npos && ap_pos != word.size()) {
            std::string ap(word.substr(ap_pos + 1));
            word.resize(ap_pos);

            al = DecodeFlags(&flags, ap.c_str());
            if(al == -1) {
                TIHU_WARNING(stderr, "Can't allocate memory.\n");
                return false;
            }
            std::sort(flags, flags + al);
        } else {
            al = 0;
            flags = NULL;
        }

        if(AddWord(word, label, pron, flags, al, std::stoi(freq))) {
            return false;
        }
    }

    return true;
}

int CHashManager::DecodeFlags(unsigned short** result, const std::string &flags) const
{
    int len;
    if(flags.empty()) {
        *result = NULL;
        return 0;
    }

    len = flags.size();
    if(len % 2 == 1) {
        TIHU_WARNING(stderr, "error: bad flag vector: %s\n", flags.c_str());
    }
    len /= 2;
    *result = (unsigned short*)malloc(len * sizeof(unsigned short));
    if(!*result) {
        return -1;
    }
    for(int i = 0; i < len; i++) {
        (*result)[i] = (((unsigned short)flags[i * 2]) << 8) +
                       (unsigned short)flags[i * 2 + 1];
    }
    return len;
}

// add a word to the hash table (private)
int CHashManager::AddWord(
    const std::string &word,
    const std::string &label,
    const std::string &pron,
    unsigned short* flags,
    short flags_len,
    int freq)
{
    int w_len = word.size();
    int l_len = label.size();
    int p_len = pron.size();

    // variable-length hash record with word and optional fields
    struct hentry* hp =
        (struct hentry*)malloc(sizeof(struct hentry) + w_len + l_len + p_len + 3);
    if(!hp) {
        return 1;
    }

    strcpy(hp->text, word.c_str());
    strcpy(hp->text + w_len + 1, label.c_str());
    strcpy(hp->text + w_len + l_len + 2, pron.c_str());

    int hash = Hash(hp->text);

    hp->w_len = w_len;
    hp->l_len = l_len;
    hp->alen = flags_len;
    hp->astr = flags;
    hp->next = NULL;
    hp->next_homonym = NULL;

    struct hentry* dp = TablePtr[hash];
    if(!dp) {
        TablePtr[hash] = hp;
    } else {
        while(dp->next != NULL &&
              strcmp(hp->text, dp->text) != 0) {

            dp = dp->next;
        }

        if(strcmp(hp->text, dp->text) == 0) {
            while(dp->next_homonym != NULL) {
                dp = dp->next_homonym;
            }
            dp->next_homonym = hp;
        } else {
            dp->next = hp;
        }
    }

    return 0;
}
