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
#include "phoneme.h"

enum class ConsonantType {
    CONSONANT_TYPE_NOT_SET,
    CONSONANT_TYPE_VOWEL,
    CONSONANT_TYPE_STOP,
    CONSONANT_TYPE_FRICATIVE,
    CONSONANT_TYPE_AFFRICATIVE,
    CONSONANT_TYPE_NASAL,
    CONSONANT_TYPE_LIQUID,
    CONSONANT_TYPE_APPROXIMANT,
};

typedef struct {
    char phoneme;
    char MbrName[4];
    char IPAName[4];
    int Duration;
    ConsonantType Consonant;
} PHONEME;

/// This table was set by Mostafa.
/// My main resource was WikiPedia:
///    https://en.wikipedia.org/wiki/Persian_phonology
/// Never trust it!
const static PHONEME PhonemeTable[] = {
    { '!', "_" , "_" , 100, ConsonantType::CONSONANT_TYPE_NOT_SET       } ,/// unknown, error
    { '_', "_" , "_" , 100, ConsonantType::CONSONANT_TYPE_NOT_SET       } ,
    { 'h', "h" , "h" ,  83, ConsonantType::CONSONANT_TYPE_FRICATIVE     } ,
    { 'C', "c:", "tS", 120, ConsonantType::CONSONANT_TYPE_AFFRICATIVE   } ,
    { '?', "?" , "?" ,  50, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'p', "p" , "p" , 112, ConsonantType::CONSONANT_TYPE_STOP          },
    { 't', "t" , "t" ,  81, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'c', "c" , "k" , 100, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'k', "k" , "k" , 100, ConsonantType::CONSONANT_TYPE_STOP          },
    { 's', "s" , "s" , 123, ConsonantType::CONSONANT_TYPE_FRICATIVE     },
    { 'S', "s:", "S" , 111, ConsonantType::CONSONANT_TYPE_FRICATIVE     },
    { 'x', "x" , "x" , 109, ConsonantType::CONSONANT_TYPE_FRICATIVE     },
    { 'f', "f" , "f" ,  99, ConsonantType::CONSONANT_TYPE_FRICATIVE     },
    { 'b', "b" , "b" ,  70, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'd', "d" , "d" ,  66, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'g', "g:", "g" ,  78, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'G', "g" , "g" ,  78, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'q', "q" , "q" ,  87, ConsonantType::CONSONANT_TYPE_STOP          },
    { 'z', "z" , "z" ,  86, ConsonantType::CONSONANT_TYPE_FRICATIVE     },
    { 'Z', "z:", "Z" ,  96, ConsonantType::CONSONANT_TYPE_FRICATIVE     },
    { 'v', "v" , "v" ,  52, ConsonantType::CONSONANT_TYPE_FRICATIVE     },
    { 'j', "j:", "dZ",  92, ConsonantType::CONSONANT_TYPE_AFFRICATIVE   },
    { 'r', "r" , "R" ,  38, ConsonantType::CONSONANT_TYPE_APPROXIMANT   },
    { 'm', "m" , "m" ,  73, ConsonantType::CONSONANT_TYPE_NASAL         },
    { 'n', "n" , "n" ,  61, ConsonantType::CONSONANT_TYPE_NASAL         },
    { 'l', "l" , "l" ,  58, ConsonantType::CONSONANT_TYPE_APPROXIMANT   },
    { 'y', "y" , "j" ,  71, ConsonantType::CONSONANT_TYPE_APPROXIMANT   },
    { 'i', "i" , "i" , 101, ConsonantType::CONSONANT_TYPE_VOWEL         },
    { 'u', "u" , "u" , 112, ConsonantType::CONSONANT_TYPE_VOWEL         },
    { 'A', "a:", "A:", 138, ConsonantType::CONSONANT_TYPE_VOWEL         },
    { 'e', "e" , "e" ,  69, ConsonantType::CONSONANT_TYPE_VOWEL         },
    { 'o', "o" , "u" ,  83, ConsonantType::CONSONANT_TYPE_VOWEL         },
    { 'a', "a" , "a" ,  90, ConsonantType::CONSONANT_TYPE_VOWEL         },
};

#define PHONEME_TABLE_SIZE ARRAY_LENGTH(PhonemeTable)

/*
int main()
{
    int PhonemeTableHelper[256];
    memset((void*)PhonemeTableHelper,  0, sizeof(PhonemeTableHelper));
    for (size_t index = 0; index < PHONEME_TABLE_SIZE; ++index)
        PhonemeTableHelper[(int)PhonemeTable[index].phoneme] = index;

    for (int i = 0; i<256; i++) {
        if (i % 32 == 0) printf("\n");
        printf("%2d, ", PhonemeTableHelper[i]);
    }
    return 0;
}
*/

const static int PhonemeTableHelper[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,
    0, 29,  0,  3,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,  0,  0,  0,  0,  0,  0, 19,  0,  0,  0,  0,  1,
    0, 32, 13,  7, 14, 30, 12, 15,  2, 27, 21,  8, 25, 23, 24, 31,  5, 17, 22,  9,  6, 28, 20,  0, 11, 26, 18,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

#define PHO_DEFAULT_PITCH 200

CPhoneme::CPhoneme()
    : PhonemeIndex(0), PitchRange(0, 0), Duration(0), PrevVoweled(false) {}

std::string CPhoneme::GetMbrName() {
    return PhonemeTable[PhonemeIndex].MbrName; //
}

std::string CPhoneme::GetIPAName() {
    return PhonemeTable[PhonemeIndex].IPAName; //
}

/*void CPhoneme::operator =(Phoneme &pho)
{
}*/

void CPhoneme::SetPhonetic(char prev_phoneme, char phoneme, char next_phoneme) {
    if (phoneme == 'g' &&
        (next_phoneme == 'A' || next_phoneme == 'o' || next_phoneme == 'u')) {
        phoneme = 'G';
    } else if (phoneme == 'k' && !(next_phoneme == 'A' || next_phoneme == 'o' ||
                                   next_phoneme == 'u')) {
        phoneme = 'c';
    }

    if (IsVowelPhoneme(phoneme)) {
        if (prev_phoneme == 0 || IsVowelPhoneme(prev_phoneme)) {
            PrevVoweled = true;
        }
    }

    PhonemeIndex = PhonemeTableHelper[(int)phoneme];

    assert(PhonemeIndex != 0);
}

std::string CPhoneme::GetMbrolString() {
    std::string mbrola;
    //--------------------------------------------------------------
    // Format of MBrola phonemes is:
    // phonemeName-duration-pitchRangeArray(...)
    // that pitchRangeArray set in below format:
    // 1 First-Pitch-Value 100 Last-Pitch-Value
    // for example "a: 120 1 135 100 110"
    //--------------------------------------------------------------
    // SetPitch(1, m_PitchRange.FValue, 100, m_PitchRange.LValue, -1
    // /*terminator arg*/);

    static char temp[1024];
    snprintf
        (temp, 1024, "%s%s %d \n", (PrevVoweled) ? "? 50 \n" : "",
         PhonemeTable[PhonemeIndex].MbrName,
         PhonemeTable[PhonemeIndex].Duration);

    // sprintf_s(pszBuffer, length, "%s %d 1 %d 100 %d \n",
    //  mbrPhonemes[m_nPhoIndex].cName/*name*/,
    /////////// m_nDuration * fSpeedRatio/*duration*/,
    //  m_PitchRange.FValue,
    //      m_PitchRange.LValue);

    mbrola.assign(temp);

    return mbrola;
}
