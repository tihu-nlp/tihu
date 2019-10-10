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

#ifndef __TIHU__VERSION_H
#define __TIHU__VERSION_H

#include <string>

const int VersionMajor = 2;
const int VersionMinor = 1;
const int VersionPatch = 0;

const std::string GetVersion() {
    char buf[16];
    sprintf(buf, "%d.%d.%d",VersionMajor, VersionMinor,VersionPatch);
    return buf;
}

#endif
