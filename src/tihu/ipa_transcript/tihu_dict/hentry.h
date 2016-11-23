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
#ifndef __TIHU__POS_TAGGER__HENTRY_H
#define __TIHU__POS_TAGGER__HENTRY_H


#include <string>
#include <algorithm>


#define ROTATE_LEN 5

#define ROTATE(v, q) \
  (v) = ((v) << (q)) | (((v) >> (32 - q)) & ((1 << (q)) - 1));

// approx. number  of user defined words
#define USERWORD 1000

#define FLAG unsigned short
#define FLAG_NULL 0


#define TESTAFF(a, b, c) (!b || std::binary_search(a, a + c, b))


/// hashed entry
struct hentry {
    unsigned char w_len;
    unsigned char l_len;
    short alen;            // length of affix flag vector
    FLAG* astr;  // affix flag vector
    struct hentry* next;   // next word with same hash code
    struct hentry* next_homonym;
    int freq;
    char text[1];  // variable-length word (8-bit or UTF-8 encoding)
};

#endif
