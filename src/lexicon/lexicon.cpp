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
#include "lexicon.h"
#include "afx_manager.h"
#include "hash_manager.h"

#define MAX_COMPOUND 4

CLexicon::CLexicon() {
  HashManager = new CHashManager();
  AfxManager = new CAfxManager(HashManager);
}

CLexicon::~CLexicon() {
  delete AfxManager;
  delete HashManager;
}

bool CLexicon::Load() {
  if (!AfxManager->Load("./data/lexicon.aff")) {
    return false;
  }

  if (!HashManager->LoadTable("./data/lexicon.dic", /*dic_key*/ "")) {
    return false;
  }

  return true;
}

const CEntry *CLexicon::FindBestEntry(const std::u16string &text,
                                      bool only_verb) const {
  std::string t = UTF16ToUTF8(text);
  CWordPtr word = std::make_unique<CWord>();
  if (TagWord(word, t)) {
    CEntryList &entry_list = word->GetEntryList();

    for (auto it = entry_list.begin(); it != entry_list.end(); ++it) {
      if (only_verb && (*it)->IsVerb()) {
        return (*it).get();
      }
      if (!only_verb && !(*it)->IsVerb()) {
        return (*it).get();
      }
    }
  }

  return nullptr;
}

bool CLexicon::TagWord(CWordPtr &word, const std::string &text) const {
  const char *t = text.c_str();
  int len = text.size();

  if (!len) {
    return false;
  }

  // look word in hash table
  struct hentry *he = HashManager->Lookup(t);

  while (he) {
    AfxManager->ParsEntry(he, NULL, NULL, word);
    he = he->next_homonym;
  }

  // try stripping off affixes
  AfxManager->AffixCheck(t, len, FLAG_NULL, word);

  return !word->IsEmpty();
}

void CLexicon::ParsText(CCorpus *corpus) {
  CWordList &word_list = corpus->GetWordList();
  for (auto it = word_list.begin(); it != word_list.end();) {
    if (IsStopped) {
      break;
    }

    CWordPtr &word = (*it);
    if (word->IsPersianWord()) {
      ///
      if (!TagCompound(word_list, it)) {
        if (TryToBreak(word_list, it)) {
          ///
          continue;
        }

        if (word->EndsWithKasre()) {
          std::string t = word->GetText();
          RemoveLast(t);
          if (TagWord(word, t)) {
            CEntryList &entry_list = word->GetEntryList();
            for (auto it = entry_list.cbegin(); it != entry_list.cend(); ++it) {
              (*it)->AddGenitive();
            }
          }
        }
      }
    }

    ++it;
  }

  corpus->Dump("lexicon.xml");
}

bool CLexicon::TagCompound(CWordList &word_list, CWordList::iterator &word_it) {
  int compound_count = MAX_COMPOUND;

  while (compound_count > 0) {
    std::vector<std::string> compound =
        MakeCompound(word_list, word_it, compound_count);

    if (compound.empty()) {
      return false;
    }

    std::string text = GetCompoundText(compound);

    if (TagWord(*word_it, text)) {
      if (compound.size() > 1) {
        /// --- update word -----
        (*word_it)->SetText(text);
        (*word_it)->SetLength(text.length());

        ++word_it;
        for (size_t index = 0; index < compound.size() - 1; ++index) {
          word_it = word_list.erase(word_it);
        }
        --word_it;
      }

      return true;
    }

    compound_count = compound.size() - 1;
  }

  return false;
}

std::string
CLexicon::GetCompoundText(const std::vector<std::string> &compound) {
  std::string text;
  auto iter = compound.cbegin();

  text = *iter;

  for (++iter; iter != compound.cend(); ++iter) {
    text.append(CHR_U8_ZWNJ);
    text.append(*iter);
  }

  return text;
}

std::vector<std::string>
CLexicon::MakeCompound(const CWordList &word_list,
                       CWordList::const_iterator word_it, int compound_count) {
  std::vector<std::string> compound;
  compound.reserve(compound_count);

  int index = 0;
  for (auto it = word_it; it != word_list.cend() && index < compound_count;
       ++it, ++index) {
    const CWordPtr &word = (*it);

    if (!word->IsPersianWord()) {
      break;
    }

    compound.push_back(word->GetText());
  }

  /// Check last word
  if (!CanBeCompoundWord(compound)) {
    compound = MakeCompound(word_list, word_it, index - 1);
  }

  return compound;
}

bool CLexicon::CanBeCompoundWord(
    const std::vector<std::string> &compound) const {
  if (compound.size() == 1) {
    return true;
  }

  std::string last = compound.back();
  /// می و نمی، در انتهای یک ترکیب نمیتوانند باشند
  if (last == "می" || last == "نمی")
    return false;

  /// the last word shouldn't be a prefix
  return !AfxManager->IsPrefix(last.c_str());
}

/// TODO: improve this by the weight...
///
/// try to decompose the word
/// وزیرکارواموراجتماعی --> وزیر + کار + و + امور + اجتماعی
///
bool CLexicon::TryToBreak(CWordList &word_list,
                          CWordList::iterator &word_it) const {
  const std::string text = (*word_it)->GetText();

  /// -----------------------------------------------------
  /// check verbs starts with می
  if (StartsWith(text, "می") || StartsWith(text, "نمی")) {
    std::string text2 = text;
    Replace(text2, "می", "می‌");
    auto entry = FindBestEntry(UTF8ToUTF16(text2), true);
    if (entry && entry->IsVerb()) {
      (*word_it)->SetText(text2);
      return true;
    }
  }
  /// -----------------------------------------------------

  const std::u16string text_16 = UTF8ToUTF16(text);
  std::list<std::pair<std::u16string, int>> r;
  std::list<std::list<std::pair<std::u16string /*words*/, int /*weight*/>>>
      partials;
  Break(r, text_16, partials);

  if (partials.empty()) {
    return false;
  }

  int max_weight = 0;
  size_t min_count = 100;
  std::list<std::list<std::pair<std::u16string, int>>>::const_iterator best;

  for (auto it1 = partials.cbegin(); it1 != partials.cend(); ++it1) {
    if (min_count > (*it1).size()) {
      min_count = (*it1).size();

      int total_weight = 0;
      for (auto it2 = (*it1).cbegin(); it2 != (*it1).cend(); ++it2) {
        // printf("s:%d", (*it2).first, (*it2).second);
        total_weight += (*it2).second;
      }

      if (total_weight > max_weight) {
        best = it1;
      }
    }
  }

  size_t offset = (*word_it)->GetOffset();

  for (auto &p : *best) {
    CWordPtr word = std::make_unique<CWord>();

    word->SetText(UTF16ToUTF8(p.first));
    word->SetType(TIHU_TOKEN_TYPE::PERSIAN);
    word->SetOffset(offset);
    word->SetLength(p.first.length());

    word_list.insert(word_it, std::move(word));

    offset += p.first.length();
  }

  word_it = word_list.erase(word_it);

  size_t s = (*best).size();
  while (s--) {
    --word_it;
  }

  return true;
}

void CLexicon::Break(
    const std::list<std::pair<std::u16string, int>> &parts,
    const std::u16string &text,
    std::list<std::list<std::pair<std::u16string /*words*/, int /*weight*/>>>
        &partials) const {

  if (text.empty()) {
    return;
  }

  auto entry = FindBestEntry(text, false);
  if (entry && !entry->IsVerb()) {
    auto p = parts;
    p.push_back(std::make_pair(text, entry->GetWeight()));
    partials.push_back(p);
  }
  std::u16string temp;
  const char16_t *c_str = text.c_str();

  while (*c_str) {
    temp += *c_str;

    if (CanBeDetached(temp.c_str()) || *(c_str + 1) == 0) {
      auto p = parts;

      auto entry = FindBestEntry(temp, false);
      if (entry && !entry->IsVerb()) {
        p.push_back(std::make_pair(temp, entry->GetWeight()));
        Break(p, c_str + 1, partials);
      }
    }

    c_str++;
  }
}

bool CLexicon::CanBeDetached(std::u16string str) const {
  char16_t last = str.back();

  switch (last) {
  case 0x0627: //ا
  case 0x062F: //د
  case 0x0630: //ذ
  case 0x0631: //ر
  case 0x0632: //ز
  case 0x0698: //ژ
  case 0x0648: //و
    return true;
  }

  return false;
}
