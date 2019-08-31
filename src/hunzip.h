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
/* hunzip: file decompression for sorted dictionaries with optional encryption,
 * algorithm: prefix-suffix encoding and 16-bit Huffman encoding */

#ifndef __TIHU__HUNZIP_H
#define __TIHU__HUNZIP_H


#include <stdio.h>
#include <fstream>
#include <vector>

#define BUFSIZE 65536
#define HZIP_EXTENSION ".hz"

#define MSG_OPEN "error: %s: cannot open\n"
#define MSG_FORMAT "error: %s: not in hzip format\n"
#define MSG_MEMORY "error: %s: missing memory\n"
#define MSG_KEY "error: %s: missing or bad password\n"

struct bit {
    unsigned char c[2];
    int v[2];
};

class Hunzip
{
private:
    Hunzip(const Hunzip &);
    Hunzip &operator=(const Hunzip &);

protected:
    std::string filename;
    std::ifstream fin;
    std::streamsize inbits;
    int bufsiz, lastbit, inc, outc;
    std::vector<bit> dec;     // code table
    char in[BUFSIZE];         // input buffer
    char out[BUFSIZE + 1];    // Huffman-decoded buffer
    char line[BUFSIZE + 50];  // decoded line
    int getcode(const char* key);
    int getbuf();
    int fail(const char* err, const char* par);

public:
    Hunzip(const char* filename, const char* key = NULL);
    ~Hunzip();
    bool is_open() {
        return fin.is_open();
    }
    bool getline(std::string &dest);
};

#endif
