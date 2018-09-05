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
#include "tihu_dict.h"
#include "afx_manager.h"
#include "hash_manager.h"

#define MAX_COMPOUND      4



CTihuDict::CTihuDict()
{
    HashManager = new CHashManager();
    AfxManager = new CAfxManager(HashManager);
}

CTihuDict::~CTihuDict()
{
    delete AfxManager;
    delete HashManager;
}

bool CTihuDict::Load(std::string name)
{
    Name = name;

    if(!AfxManager->Load("./data/lexicon.aff")) {
        return false;
    }

    if(!HashManager->LoadTable("./data/lexicon.dic", /*dic_key*/"")) {
        return false;
    }

    return true;
}

bool CTihuDict::CheckWord(const std::string &_text) const
{
    const char* text = _text.c_str();
    int len = _text.size();

    if(!len) {
        return false;
    }

    // look word in hash table
    struct hentry* he =
        HashManager->Lookup(text);

    if(he) {
        return true;
    }

    return false;
}

bool CTihuDict::TagWord(CWordPtr &word) const
{
    auto _text = word->GetText(); /// create a copy
    const char* text = _text.c_str();
    int len = _text.size();

    if(!len) {
        return false;
    }

    // look word in hash table
    struct hentry* he =
        HashManager->Lookup(text);

    while(he) {
        AfxManager->ParsEntry(he, NULL, NULL, word);
        he = he->next_homonym;
    }

    // try stripping off affixes
    AfxManager->AffixCheck(text, len, FLAG_NULL, word);

    return !word->GetPron().empty();
}

void CTihuDict::ParsText(CCorpus* corpus)
{
    CWordList &word_list = corpus->GetWordList();
    for(auto itr = word_list.begin();
            itr != word_list.end();) {

        CWordPtr &word = (*itr);

        if(word->IsPersianWord()) {

            ///
            if (!TagWord(word)) {
                if (Breakdown(word_list, itr)) {
                    ///
                    continue;
                }
            }
        }

        ++itr;
    }
}

///
/// try to decompose the word
/// وزیرکارواموراجتماعی --> وزیر + کار + و + امور + اجتماعی
///
bool CTihuDict::Breakdown(CWordList &word_list, CWordList::iterator &word_itr) const
{
    std::string text = (*word_itr)->GetText();
    std::u16string text_16 = UTF8ToUTF16(text);
    const char16_t* c_str_16 = text_16.c_str();
    std::list<std::u16string> partials;
    const char16_t* p = 0;

    while(*c_str_16) {
        p = c_str_16;

        std::u16string partial;
        std::u16string temp;

        while(*p) {
            temp += *p;

            if(CanBeDetached(temp.c_str()) || *(p+1)==0) {
               // همیشه «و» را یک کلمه جدا در نظر میگیریم
                if (temp == u"\u0648") {
                    partial = temp;
                    break;
                }

                std::string temp_u8 = UTF16ToUTF8(temp);
                if(CheckWord(temp_u8)) {
                    partial = temp;
                }
            }

            p++;
        }

        if(partial.length() == 0) {
            return false;
        }

        c_str_16 += partial.length();
        partials.push_back(partial);
        partial.clear();
        temp.clear();
    }

    if(partials.size()<=1) {
        return false;
    }

    size_t offset = (*word_itr)->GetOffset();

    for(auto &partial : partials) {
        CWordPtr word = std::make_unique<CWord>();

        word->SetText(UTF16ToUTF8(partial));
        word->SetOffset(offset);
        word->SetLength(partial.length());
        if(partial == u"\u0648") {
            word->SetPOSTag("CONJ");
        } else {
            word->SetPOSTag((*word_itr)->GetPOSTag());
        }

        word_list.insert(word_itr, std::move(word));

        offset += partial.length();
    }

    word_itr = word_list.erase(word_itr);

    size_t s = partials.size();
    while(s--) {
        --word_itr;
    }

    return true;
}

bool CTihuDict::CanBeDetached(std::u16string str) const
{
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

    if (str == u"\u0645\u06CC" ||   /// می
        str == u"\u0646\u0645\u06CC") {   /// نمی
        return true;
    }

    return false;
}
