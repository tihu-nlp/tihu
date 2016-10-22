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
#include "helper.h"

#include <algorithm>
#include <string>

#define USING_UTF8_LIB

#ifdef USING_UTF8_LIB
#include "../third_party/utf8/source/utf8.h"
#endif


namespace
{
class is_any_of
{
public:
    explicit is_any_of(const std::string &in) : chars(in) {}

    bool operator()(char c)
    {
        return chars.find(c) != std::string::npos;
    }

private:
    std::string chars;
};
}

std::string::const_iterator mystrsep(const std::string &str,
                                     std::string::const_iterator &start)
{
    std::string::const_iterator end = str.end();

    is_any_of op(" \t");
    // don't use isspace() here, the string can be in some random charset
    // that's way different than the locale's
    std::string::const_iterator sp = start;
    while(sp != end && op(*sp)) {
        ++sp;
    }

    std::string::const_iterator dp = sp;
    while(dp != end && !op(*dp)) {
        ++dp;
    }

    start = dp;
    return sp;
}

// remove cross-platform text line end characters
void mychomp(std::string &s)
{
    size_t k = s.size();
    size_t newsize = k;
    if((k > 0) && ((s[k - 1] == '\r') || (s[k - 1] == '\n'))) {
        --newsize;
    }
    if((k > 1) && (s[k - 2] == '\r')) {
        --newsize;
    }
    s.resize(newsize);
}

size_t u16len(const char16_t* wcs_)
{
#ifdef _NOPE
    return std::char_traits<char16_t>::length(string);
#endif

    const char16_t* _wcs = wcs_;

    while(*_wcs++);

    return (size_t)(_wcs - wcs_ - 1);
}

char16_t* u16chr(const char16_t* wcs_, char16_t wch_)
{
    while(*wcs_ && *wcs_ != wch_) {
        ++wcs_;
    }

    if(*wcs_ == wch_) {
        return (char16_t*)wcs_;
    }

    return NULL;
}

char16_t* u16sep(char16_t** stringp, const char16_t delim)
{
    char16_t* mp = *stringp;
    if (*mp != '\0') {
        char16_t* dp;
        if (delim) {
            dp = u16chr(mp, delim);
        }
        else {
            // don't use isspace() here, the string can be in some random charset
            // that's way different than the locale's
            for (dp = mp; (*dp && *dp != ' ' && *dp != '\t'); dp++);
            if (!*dp) dp = NULL;
        }
        if (dp) {
            *stringp = dp + 1;
            *dp = '\0';
        }
        else {
            *stringp = mp + u16len(mp);
        }
        return mp;
    }
    return NULL;
}

char GetLastChar(const std::string &value)
{
    if(value.empty()) {
        return 0;
    }

    return value.back();
}

char GetFirstChar(const std::string &value)
{
    if(value.empty()) {
        return 0;
    }

    return *value.begin();
}

std::u16string& ToLower(std::u16string &string)
{
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);

    return string;
}

std::u16string& ToUpper(std::u16string &string)
{
    std::transform(string.begin(), string.end(), string.begin(), ::toupper);

    return string;
}

std::string& ToLower(std::string &value)
{
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    return value;
}

std::string& ToUpper(std::string &value)
{
    std::transform(value.begin(), value.end(), value.begin(), ::toupper);

    return value;
}

std::u16string UTF8ToUTF16(const std::string &utf8line)
{
    std::u16string utf16line;
#ifdef USING_UTF8_LIB
    utf8::utf8to16(utf8line.begin(), utf8line.end(), back_inserter(utf16line));
#else
#endif

    return utf16line;
}

std::string UTF16ToUTF8(const std::u16string &utf16line)
{
    std::string utf8line;

#ifdef USING_UTF8_LIB
    utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));
#else
    /// link error in visual studio
    /// https://social.msdn.microsoft.com/Forums/vstudio/en-US/8f40dcd8-c67f-4eba-9134-a19b9178e481/vs-2015-rc-linker-stdcodecvt-error?forum=vcgeneral
    std::wstring_convert<
    std::codecvt_utf8_utf16<char16_t>, char16_t> convert;

    utf8line = convert.to_bytes(utf16line);
#endif

    return utf8line;
}

std::u16string UTF32ToUTF16(const std::u32string &utf32line)
{
    std::u16string result;

#ifndef _MSC_VER
    std::wstring_convert<
    std::codecvt_utf16<char32_t>, char32_t> convert;

    std::string bytes = convert.to_bytes(utf32line);

    result.reserve(bytes.size() / 2);

    for(size_t i = 0; i < bytes.size(); i += 2) {
        result.push_back(static_cast<char16_t>(static_cast<unsigned char>(bytes[i]) * 256 + static_cast<unsigned char>(bytes[i + 1])));
    }
#endif
    return result;
}



/// Is the pronunciation std::string ends with vowels phoneme
bool IsPronunciationEndsWithVowels(const std::string &pronunciation)
{
    return IsVowelPhoneme(GetLastChar(pronunciation));
}

bool IsVowelPhoneme(char phoneme)
{
    if(phoneme == 'a' ||  // a and a:
       phoneme == 'e' ||
       phoneme == 'o' || // o and ow
       phoneme == 'u' ||
       phoneme == 'i' ||
       phoneme == 'A') {
        return true;
    }

    return false;
}

/// Is the pronunciation std::string starts with vowels phoneme
bool IsPronunciationStartsWithVowels(const std::string &pronunciation)
{
    return IsVowelPhoneme(GetFirstChar(pronunciation));
}


/// Concat (Merge) two pronunciation
std::string ConcatPronunciations(const std::string &pronunciation_1, const std::string &pronunciation_2)
{
    std::string pronunciation = pronunciation_1;

    if(IsPronunciationStartsWithVowels(pronunciation_2)) {
        char last = GetLastChar(pronunciation_1);
        char first = GetFirstChar(pronunciation_2);

        if(IsVowelPhoneme(last)) {
            /// like تنباکوم کجاست
            if(last == 'u' && first == 'a') {
                /// tanbAkuaS -> tanbAkuS
                int offset = pronunciation.length();
                pronunciation += pronunciation_2;

                pronunciation.erase(offset, 1);

                return pronunciation;
            }  else if(first == 'o' || first == 'w' || first == 'u') {
                pronunciation += 'v';
            }
        }
    }

    pronunciation += pronunciation_2;

    return pronunciation;
}

bool EndsWith(const std::string &value, std::string ending)
{
    if(ending.size() > value.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool StartsWith(const std::string &value, std::string starting)
{
    if (starting.size() > value.size()) {
        return false;
    }
    return std::equal(starting.begin(), starting.end(), value.begin());
}

bool IsLeadingByte(char c)
{
    auto first_bit_set = (c & 0x80) != 0;
    auto second_bit_set = (c & 0X40) != 0;
    return !first_bit_set || second_bit_set;
}

std::string& RemoveFirst(std::string &value)
{
    while (!IsLeadingByte(value.front()))
        value.erase(0,1);
    value.erase(0, 1);

    return value;
}

std::string& RemoveLast(std::string &value)
{
    while (!IsLeadingByte(value.back()))
        value.pop_back();
    value.pop_back();

    return value;
}
