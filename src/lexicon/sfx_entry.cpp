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
#include "sfx_entry.h"
#include "afx_manager.h"
#include "pfx_entry.h"

CSfxEntry::CSfxEntry(CAfxManager *afx_manager)
    : CAfxEntry(afx_manager), next(nullptr), nexteq(nullptr), nextne(nullptr) {}

void CSfxEntry::initReverseWord() {
  rAppnd = Appnd;

  std::reverse(rAppnd.begin(), rAppnd.end());
}

// see if this suffix is present in the word
void CSfxEntry::CheckWord(const char *text, int len, CPfxEntry *pfx,
                          const FLAG needflag, CWordPtr &word) {
  struct hentry *he; // hash entry pointer

  // upon entry suffix is 0 length or already matches the end of the word.
  // So if the remaining root word has positive length
  // and if there are enough chars in root word and added back strip chars
  // to meet the number of characters conditions, then test it

  int tmpl = len - Appnd.size(); // length of tmpword
  // the second condition is not enough for UTF-8 strings
  // it checked in test_condition()

  std::string tmpstring(text, tmpl);

  const char *tmpword = tmpstring.c_str();
  if ((he = AfxManager->Lookup(tmpword)) != NULL) {
    do {
      // check conditional suffix (enabled by prefix)
      if (TESTAFF(he->astr, Flag, he->alen) &&
          TESTAFF(he->astr, needflag, he->alen)) {

        AfxManager->ParsEntry(he, pfx, this, word);
      }
      he = he->next_homonym; // check homonyms
    } while (he);
  }
}
