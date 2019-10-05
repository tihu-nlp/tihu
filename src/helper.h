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
#ifndef __TIHU__HELPER_H
#define __TIHU__HELPER_H

#pragma once

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <assert.h>
#include <cassert>
#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <locale>
#include <memory>
#include <stddef.h>
#include <stdint.h>
#include <vector>

#define TIHU_WARNING_ON
#ifdef TIHU_WARNING_ON
#define TIHU_WARNING fprintf
#else
static inline void TIHU_WARNING(FILE *, const char *, ...) {}
#endif

#define CHR_U8_ZWNJ         "\xe2\x80\x8c"
#define CHR_U8_TANVIN_NASB  "\xd9\x8c"
#define CHR_U8_TANVIN_ZAM   "\xd9\x8c"
#define CHR_U8_TANCIN_JAR   "\xd9\x8d"
#define CHR_U8_FATHE        "\xd9\x8e"
#define CHR_U8_ZAME         "\xd9\x8f"
#define CHR_U8_KASRE        "\xd9\x90"
#define CHR_U8_TASHDID      "\xd9\x91"
#define CHR_U8_SUKUN        "\xd9\x92"
#define CHR_U8_YE           "\xdb\x8c"
#define CHR_U8_ALEF         "\xd8\xa7"
#define CHR_U8_HE           "\xd9\x87"
#define CHR_U8_WAW          "\xd9\x88"
#define CHR_U8_KAF          "\xda\xa9"
#define CHR_U8_GAF          "\xda\xaf"
#define CHR_U8_HAMZE        "\xd9\x94"
#define CHR_U8_LAM          "\xd9\x84"
#define CHR_U8_DAL          "\xd8\xaf"
#define CHR_U8_ZAL          "\xd8\xb0"
#define CHR_U8_RE           "\xd8\xb1"
#define CHR_U8_ZE           "\xd8\xb2"
#define CHR_U8_ZHE          "\xda\x98"
#define CHR_U8_MIM          "\xd9\x85"
#define CHR_U8_NON          "\xd9\x86"
#define STR_U8_MI           "\xd9\x85\xdb\x8c"
#define STR_U8_NEMI         "\xd9\x86\xd9\x85\xdb\x8c"

#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif

enum class TIHU_TOKEN_TYPE {
    LINE_BREAK = 'n',
    NON_PERSIAN = '@',
    PERSIAN = '!',
    NUMBER = '#',
    PUNCTUATION = '$',
    DELIMITER = '-',
    UNKNOWN = '?',
};

typedef std::string TIHU_EVENT_VALUE;

enum class TIHU_EVENT_TYPE {
    UNKNOWN = 0,
    BOOKMARK,
    SPEED_RATIO,
    PITCH_RATIO,
    VOLUME_RATIO,
    SILENCE,
    SPELL_OUT,
};

#define UNSET_FLAG(info, flag) (info = (info & ~flag))
#define SET_FLAG(info, flag) (info = ((info & ~flag) | flag))
#define IS_FLAG_SET(info, flag) ((info & flag) == flag)

#define GET_LOBYTE(w) ((uint8_t)((w)&0xff))
#define GET_HIBYTE(w) ((uint8_t)((w) >> 8))

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

#ifndef assert
#include <cassert>

#define assert(x) assert(x)
#endif

std::string::const_iterator mystrsep(const std::string &str,
                                     std::string::const_iterator &start);
void mychomp(std::string &s);

size_t u16len(const char16_t *wcs_);
char16_t *u16chr(const char16_t *wcs_, char16_t wch_);
char16_t *u16sep(char16_t **stringp, const char16_t delim);
int u16cmp(const char16_t *str1, const char16_t *str2, size_t n = -1);

bool IsVowelPhoneme(char c);
bool IsPronunciationEndsWithVowels(const std::string &pronunciation);
bool IsPronunciationStartsWithVowels(const std::string &pronunciation);
std::string ConcatPronunciations(const std::string &pronunciation_1,
                                 const std::string &pronunciation_2);

bool IsLeadingByte(char c);
bool EndsWithDetached(const std::string &value);
bool EndsWith(const std::string &value, std::string ending);
bool StartsWith(const std::string &value, std::string starting);
bool Replace(std::string &str, const std::string &search,
             const std::string &replace);
std::string Substr(const std::string &str, const std::string &search);
std::string &RemoveLast(std::string &value);
std::string &RemoveFirst(std::string &value);
std::string &ReplaceSubstring(std::string &value, const std::string &search,
                              const std::string &replace);

std::string &ToLower(std::string &value);
std::string &ToUpper(std::string &value);
std::u16string &ToLower(std::u16string &string);
std::u16string &ToUpper(std::u16string &string);
std::u16string UTF8ToUTF16(const std::string &utf8line);
std::string UTF16ToUTF8(const std::u16string &utf16line);

#endif
