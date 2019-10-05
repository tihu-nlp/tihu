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

#include "../corpus/word.h"
#include "../helper.h"

#include <map>

struct hentry;

class CHashManager {
public:
  CHashManager();

  bool LoadTable(const std::string &filename, const std::string &key);
  struct hentry *Lookup(const char *text) const;
  int GetLemmaWeight(const std::string &lemma) const;

private:
  int Hash(const char *word) const;
  int DecodeFlags(unsigned short **result, const std::string &flags) const;
  int AddWord(const std::string &word, const std::string &lemma,
              const std::string &pos, const std::string &pron,
              unsigned short *flags, short flags_len, int weight);

private:
  struct hentry **TablePtr;
  int TableSize;
  std::map<const std::string, int> LemmaWeight;
};

#endif
