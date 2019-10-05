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
#include "pfx_entry.h"
#include "afx_manager.h"
#include "sfx_entry.h"

CPfxEntry::CPfxEntry(CAfxManager *afx_manager)
    : CAfxEntry(afx_manager), next(nullptr), nexteq(nullptr), nextne(nullptr) {}

// check if this prefix entry matches
void CPfxEntry::CheckWord(const char *text, int len, const FLAG needflag,
                          CWordPtr &word) {
  struct hentry *he; // hash entry of root text or NULL

  int tmpl = len - Appnd.size(); // length of tmpword

  if (tmpl > 0) {
    // generate new root text by removing prefix and adding
    // back any characters that would have been stripped

    std::string tmptext(text + Appnd.size());

    // now make sure all of the conditions on characters
    // are met.  Please see the appendix at the end of
    // this file for more info on exactly what is being
    // tested

    // if all conditions are met then check if resulting
    // root text in the dictionary

    if ((he = AfxManager->Lookup(tmptext.c_str())) != NULL) {
      do {
        if (TESTAFF(he->astr, needflag, he->alen)) {
          AfxManager->ParsEntry(he, this, nullptr, word);
        }
        he = he->next_homonym; // check homonyms
      } while (he);
    }

    // prefix matched but no root text was found
    AfxManager->SuffixCheck(tmptext.c_str(), tmptext.length(), this, needflag,
                            word);
  }
}
