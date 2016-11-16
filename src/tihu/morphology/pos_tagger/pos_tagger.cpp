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
#include "pos_tagger.h"
#include "afx_manager.h"
#include "hash_manager.h"

#define MAX_COMPOUND      4



CPOSTagger::CPOSTagger()
{
    HashManager = new CHashManager();
    AfxManager = new CAfxManager(HashManager);
}

CPOSTagger::~CPOSTagger()
{
    delete AfxManager;
    delete HashManager;
}

bool CPOSTagger::Load(const std::string &aff_file,
                      const std::string &dic_file, const std::string &dic_key)
{
    if(!AfxManager->Load(aff_file)) {
        return false;
    }

    if(!HashManager->LoadTable(dic_file, dic_key)) {
        return false;
    }

    return true;
}

bool CPOSTagger::CheckWord(const std::string &_text)
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

bool CPOSTagger::TagWord(CWordPtr &word, const std::string &_text)
{
    std::string w2;

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

    return !word->IsEmpty();
}

void CPOSTagger::ParsText(CCorpus* corpus)
{
    CWordList &word_list = corpus->GetWordList();
    for(auto itr = word_list.begin();
            itr != word_list.end();) {

        CWordPtr &word = (*itr);

        if(word->IsPersianWord()) {

            ///
            if(!TagCompound(word_list, itr)) {
                if(Breakdown(word_list, itr)) {
                    ///
                    continue;
                }
            }
        }

        ++itr;
    }
}



bool CPOSTagger::TagCompound(CWordList &word_list, CWordList::iterator &itr)
{
    int compound_count = MAX_COMPOUND;

    while(compound_count > 0) {
        std::vector<std::string> compound =
            MakeCompound(word_list, itr, compound_count);

        if(compound.empty()) {
            break;
        }

        std::string text = GetCompoundText(compound);

        if(TagWord(*itr, text)) {
            if(compound.size() > 1) {
                /// --- update word -----
                (*itr)->SetText(text);
                (*itr)->SetLength(text.length());

                ++itr;
                for(size_t index = 0; index < compound.size()-1; ++index) {
                    itr = word_list.erase(itr);
                }
                --itr;
            }

            return true;
        }

        compound_count = compound.size()-1;
    }

    return false;
}

std::string CPOSTagger::GetCompoundText(const std::vector<std::string> &compound)
{
    std::string text;
    std::vector<std::string>::const_iterator iter = compound.begin();

    text = *iter;

    for(++iter; iter != compound.end(); ++iter) {
        if(!EndsWithDetached(text))
            text.append(CHR_U8_ZWNJ);

        text.append(*iter);
    }

    return text;
}

std::vector<std::string> CPOSTagger::MakeCompound(const CWordList &word_list, CWordList::const_iterator itr, int compound_count)
{
    std::vector<std::string> compound;
    compound.reserve(compound_count);

    int index = 0;
    for(; itr != word_list.end() &&
        index < compound_count; ++itr, ++index) {
        const CWordPtr &word = (*itr);

        if(!word->IsPersianWord()) {
            break;
        }

        compound.push_back(word->GetText());

        ///TODO:::::::::::::::
        //// if (word->IsEndOfParagraph())
        //// {
        ////     ++index;
        ////     break;
        //// }

        ///if (word->IsEndOfSentence())
        ///    break;
    }

    /// Check last word
    if(!CanBeCompoundWord(compound)) {
        compound.clear();
    }

    return compound;
}


bool CPOSTagger::CanBeCompoundWord(const std::vector<std::string> &compound) const
{
    if(compound.size() == 1) {
        return true;
    }

    std::string last = compound.back();
	/// می و نمی نمی‌توانند در انتهای یک ترکیب ظاهر شوند
	if(last == "می" || last == "نمی")
		return false;

    /// the last word shouldn't be a prefix
    return !AfxManager->IsPrefix(last.c_str());
}


///
/// try to decompose the word
/// وزیرکارواموراجتماعی --> وزیر + کار + و + امور + اجتماعی
///
bool CPOSTagger::Breakdown(CWordList &word_list, CWordList::iterator &itr)
{
    std::string text = (*itr)->GetText();
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

    size_t offset = (*itr)->GetOffset();

    for(auto &partial : partials) {
        CWordPtr word = std::make_unique<CWord>();

        word->SetText(UTF16ToUTF8(partial));
        word->SetOffset(offset);
        word->SetLength(partial.length());
        word->SetType((*itr)->GetType());

        word_list.insert(itr, std::move(word));

        offset += partial.length();
    }

    itr = word_list.erase(itr);

    size_t s = partials.size();
    while(s--) {
        --itr;
    }

    return true;
}

bool CPOSTagger::CanBeDetached(std::u16string str)
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
