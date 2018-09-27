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
#include "corpus.h"
#include "word.h"


CCorpus::CCorpus(const std::string &text)
    : Text(text)
{
}

CCorpus::CCorpus()
{
}

void CCorpus::SetText(const std::string &text)
{
    Text = text;
}

const std::string &CCorpus::GetText() const
{
    return Text;
}

CWordList &CCorpus::GetWordList()
{
    return WordList;
}

bool CCorpus::IsEmpty() const
{
    return WordList.empty();
}

void CCorpus::Clear()
{
    Text.clear();
    WordList.clear();
}

void CCorpus::AddWord(CWordPtr &word)
{
    WordList.push_back(std::move(word));
}

const CWordPtr &CCorpus::GetLastWord() const
{
    if (WordList.empty()) {
        return null_word;
    }

    return WordList.back();
}

const CWordPtr &CCorpus::GetFirstWord() const
{
    if (WordList.empty()) {
        return null_word;
    }

    return WordList.front();
}

void CCorpus::Dump(const std::string &filename) const
{
    std::string extention;
    size_t pos = filename.rfind('.');
    if(pos != std::string::npos) {
        extention = filename.substr(pos + 1);
    }

    if(extention == "xml") {
        DumpToXml(filename);
    } else {
        DumpToTxt(filename);
    }
}

void CCorpus::DumpToXml(const std::string &path) const
{
    std::ofstream writer(path);

    if(!writer.is_open()) {
        return;
    }

    writer << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << std::endl;
    writer << "<corpus>" << std::endl;

    char buffer[1024];

    for(auto &word : WordList) {
        snprintf(buffer, 1024, "\t<word text=\"%s\" pronunciation=\"%s\" label=\"%s\"/>",
                word->GetText().c_str(),
                word->GetPron().c_str(),
                word->GetPOSTag().c_str());

        writer << buffer << std::endl;
    }

    writer << "</corpus>";
    writer.flush();
    writer.close();
}

void CCorpus::DumpToTxt(const std::string &path) const
{
    std::ofstream writer(path);

    if(!writer.is_open()) {
        return;
    }

    char buffer[1024];

    for(auto &word : WordList) {

        snprintf(buffer, 1024, "%-30s%-12s %s",
            word->GetText().c_str(),
            word->GetPOSTag().c_str(),
            word->GetPron().c_str());

        writer << buffer << std::endl;
    }

    writer.flush();
    writer.close();
}
