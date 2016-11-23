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
#ifndef __TIHU__POS_TAGGER__TIHU_DICT_H
#define __TIHU__POS_TAGGER__TIHU_DICT_H

#pragma once

#include "parser.h"

class CAfxManager;
class CHashManager;

class CTihuDict
    : public IParser
{
public:
    CTihuDict();
    ~CTihuDict();

    bool Load(const std::string &aff_file,
              const std::string &dic_file, const std::string &dic_key);

    void ParsText(CCorpus* corpus) override;

protected:
    
    bool CheckWord(const std::string &text);
    bool TagWord(CWordPtr &word, const std::string &text);
    bool Breakdown(CWordList &word_list, CWordList::iterator &word_itr);
    bool CanBeDetached(std::u16string str);

private:
    CAfxManager*  AfxManager;
    CHashManager* HashManager;

};

#endif
