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
#include "entry.h"
#include "path_manager.h"



CCorpus::CCorpus()
{
}

void CCorpus::SetText(const std::string text)
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
        snprintf(buffer, 1024, "\t<word offset=\"%d\" length=\"%d\" text=\"%s\" %s %s>",
                 word->GetOffset(),
                 word->GetLength(),
                 word->GetText().c_str(),
                 word->IsEndOfParagraph() ? "eop=\"true\"" : "",
                 word->IsEndOfSentence() ? "eos=\"true\"" : "");

        writer << buffer << std::endl;

        for(auto &entry : word->GetEntryList()) {

            snprintf(buffer, 1024, "\t\t<entry pronunciation=\"%s\" loabel=\"%s\" dictation=\"%s\" />",
                     entry->GetPronunciation().c_str(),
                     entry->GetLabel().c_str(),
                     entry->GetDictation().c_str());

            writer << buffer << std::endl;
        }

        writer << "\t</word>" << std::endl;
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
        snprintf(buffer, 1024, "%-30s", word->GetText().c_str());
        writer << buffer;

        bool first_entry = true;
        for(auto &entry : word->GetEntryList()) {

            if(first_entry) {
                snprintf(buffer, 1024, "%-12s %s",
                         entry->GetLabel().c_str(),
                         entry->GetPronunciation().c_str());
            } else {
                writer << std::endl;

                snprintf(buffer, 1024, "%-40s %s",
                         entry->GetLabel().c_str(),
                         entry->GetPronunciation().c_str());
            }

            writer << buffer;
            first_entry = false;
        }

        writer << std::endl;
    }

    writer.flush();
    writer.close();
}
