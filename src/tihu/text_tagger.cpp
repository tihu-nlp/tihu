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
#include "text_tagger.h"
#include "corpus.h"

#include <map>
#include <vector>
#include <algorithm>

template <typename T1, typename T2>
struct compare {
    typedef std::pair<T1, T2> type;
    bool operator()(type const &a, type const &b) const
    {
        return a.second > b.second;
    }
};

CTextTagger::CTextTagger()
{
}

CTextTagger::~CTextTagger()
{
}

void CTextTagger::ParsText(CCorpus* corpus)
{
    std::map<std::string, int> tags;

    const CWordList &word_list = corpus->GetWordList();
    for(auto &word : word_list) {
        if(!word->IsPersianWord()) {
            continue;
        }
        
        if (word->IsNoun()) {
            if (word->GetText().size() > 2) {
                tags[word->GetText()] =
                    tags[word->GetText()] + 1;
            }
        }
    }

    std::vector<std::pair<std::string, int> > taggs_copy(tags.begin(), tags.end());
    std::sort(taggs_copy.begin(), taggs_copy.end(), compare<std::string, int>());

    std::string tags_line;
    char buf[256];
    //int c = std::min<int>(20, taggs_copy.size());
    int c = taggs_copy.size();
    for(int i = 0; i < c; ++i) {
        sprintf(buf, "%s(x%d), ",
                taggs_copy[i].first.c_str(),
                taggs_copy[i].second);

        tags_line.append(buf);
    }

    ReportMessage((char*)tags_line.c_str());
};
