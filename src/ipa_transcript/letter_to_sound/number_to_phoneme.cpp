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
#include "number_to_phoneme.h"

#include <math.h>


#define MAX_YEKAN_DIGITS_PRONUNC        ARRAY_LENGTH(YekanDigits)
#define MAX_DAHGAN_DIGITS_PRONUNC       ARRAY_LENGTH(DahganDigits)
#define MAX_SADGAN_DIGITS_PRONUNC       ARRAY_LENGTH(SadganDigits)
#define MAX_SEG_SUFFIX_PRONUNC          ARRAY_LENGTH(SegSuffix)

// This array holds the suffix pronunciations of each segments
const char SegSuffix[][16] = {
    "hezAr",    //
    "melyon",    //
    "melyArd",    //
    "bilyon",    //
    "bilyArd",
    "trilyun",
    "trilyArd"
};

const char YekanDigits[][16] = {
    "yek",
    "do",
    "se",
    "CAhAr",
    "panj",
    "SeS",
    "haft",
    "haSt",
    "noh",
    "dah",
    "yAzdah",
    "davAzdah",
    "sizdah",
    "CAhArdah",
    "pAnzdah",
    "SAnzdah",
    "hefdah",
    "heJdah",
    "nuzdah",
};

const char DahganDigits[][16] = {
    "_",
    "bist",
    "si",
    "Cehel",
    "panjAh",
    "Sast",
    "haftAd",
    "haStAd",
    "navad",
};

char SadganDigits[][16] = {
    "sad",
    "devist",
    "sisad",
    "CAhArsad",
    "pAnsad",
    "SeSsad",
    "haftsad",
    "haStsad",
    "nohsad",
};


CNumberToPhoneme::CNumberToPhoneme()
{
}

CNumberToPhoneme::~CNumberToPhoneme()
{
}

std::string CNumberToPhoneme::Convert(const std::string &text)
{
#define SEG_APPENDER  "o"

    std::string pronunce, block_pronunce, prefix;
    std::string number_text(text);

    /*if(wtoi(number_text.c_str()) == 0)
    {
    return "sefr";
    }*/

    if(number_text[0] == '-') {
        prefix += "manfiye";
        number_text.erase(0, 1);
    } else if(number_text[0] == '+') {
        prefix += "mosbate";
        number_text.erase(0, 1);
    }

    while(number_text[0] == '0') {
        prefix += "sefr_";
        number_text.erase(0, 1);
    }

    // Output string (Pronunciation)
    int length = (int)number_text.length();

    // Calculates segments count, each segments has 3 letter and cut from right of text
    int seg_count = length / 3;
    int remainder = length % 3;
    if(remainder != 0) {
        seg_count++;
    }

    int            seg_start = 0;    // Index of start point of segment in number string
    int            seg_length = 0;    // Index of start point of segment in number string
    int            seg_value = 0;    // Segment number
    std::string    seg_text;    // null-terminated string

    for(int seg_no = seg_count - 1; seg_no >= 0; seg_no--) {
        // --- get segment string ---
        seg_start = length - ((seg_no + 1) * 3);
        seg_length = 3;
        if(seg_start < 0) {
            // exp. for '23' ==> seg_start = -1 --> seg_length = 2 --> seg_start = 0
            seg_length += seg_start;
            seg_start = 0;
        }

        seg_text = number_text.substr(seg_start, seg_length);
        //seg_length=seg_text.GetLength();
        seg_value = std::stoi(seg_text);

        // --- BlockNumber
        block_pronunce = GetBlockPronunce(seg_value, seg_length);

        /// if (pronunce.length() > 0 && block_pronunce.length() > 0)
        ///    pronunce = ConcatPronunciations(pronunce, SEG_APPENDER);

        pronunce += block_pronunce;
        if(seg_no == 0) {
            break;
        }

        if(block_pronunce != "" && seg_no > 0) {
            pronunce += SegSuffix[(seg_no - 1) % MAX_SEG_SUFFIX_PRONUNC];
        }
    }

    if(prefix.length() > 0) {
        pronunce.insert(0, prefix);
    }

    return pronunce;
}

std::string CNumberToPhoneme::GetBlockPronunce(int number, int seg_len)
{
    assert(!(number > 999 || seg_len > 3));

    int         remainder = 0;
    int         powered = 0;
    int         division = 0;
    int         index_in_array = 0;
    std::string pronunce = "";
    std::string digit_text = "";

    if(number == 0) {
        return "";
    }

    remainder = number;
    if(number > 0 && number < 20) {
        return GetDigitPronunce(0, number - 1);
    }

    for(int index = seg_len - 1; index >= 0; index--) {
        powered = (int)pow(10.0F, index);   // 0, 10, 100
        division = remainder / powered;
        remainder = remainder % powered;
        // exp. 412
        // index    | 2  | 1 | 0
        // division | 4  | 1 | 2
        // remainder | 12 | 2 | 0

        if(division == 0) { // It is not eqult to 0
            continue;
        }

        if(index == 1 && division == 1) { // Exception digits (between 10 to 19)
            index_in_array = (division * 10) + remainder - 1;
            index--;
        } else {
            index_in_array = division - 1;
        }

        digit_text = GetDigitPronunce(index, index_in_array);

        if(pronunce != "") {
            pronunce = ConcatPronunciations(pronunce, SEG_APPENDER);
        }

        pronunce += digit_text;
    }
    return pronunce;
}

const char* CNumberToPhoneme::GetDigitPronunce(int first_index, int sec_index)
{
    assert(first_index < 3);
    assert(sec_index < 19);

    switch(first_index) {
    case 0:
        return YekanDigits [sec_index];
    case 1:
        return DahganDigits[sec_index];
    case 2:
        return SadganDigits[sec_index];
    }

    return "";
};
