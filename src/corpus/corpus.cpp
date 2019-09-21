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

#include <fstream>
#include <sstream>

CCorpus::CCorpus(const std::string &text, int offset) : Text(text), Offset(offset) {}

CCorpus::CCorpus() {}

void CCorpus::SetText(const std::string &text, int offset) {
    Text = text;
    Offset = offset;
}

const std::string &CCorpus::GetText() const {
    return Text; //
}

int CCorpus::GetOffset() const {
    return Offset; //
}

CWordList &CCorpus::GetWordList() {
    return WordList; //
}

bool CCorpus::IsEmpty() const {
    return WordList.empty(); //
}

size_t CCorpus::Size() const {
    return WordList.size(); //
}

void CCorpus::Clear() {
    Text.clear();
    WordList.clear();
}

void CCorpus::AddWord(CWordPtr &word) { WordList.push_back(std::move(word)); }

const CWordPtr &CCorpus::GetLastWord() const {
    if (WordList.empty()) {
        return null_word;
    }

    return WordList.back();
}

const CWordPtr &CCorpus::GetFirstWord() const {
    if (WordList.empty()) {
        return null_word;
    }

    return WordList.front();
}

std::string CCorpus::ToXml() const {
    std::ostringstream writer;

    writer << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << std::endl;
    writer << "<corpus>" << std::endl;

    char buffer[1024];
    for (auto &word : WordList) {
        snprintf(buffer, 1024,
                 "\t<word offset=\"%zd\" length=\"%zd\" weight=\"%zd\" "
                 "text=\"%s\"%s%s%s>",
                 word->GetOffset(), word->GetLength(), word->GetWeight(),
                 word->GetText().c_str(),
                 word->IsAutoPhonetics() ? " g2p=\"1\"" : "",
                 word->IsEndOfParagraph() ? " eop=\"1\"" : "",
                 word->IsEndOfSentence() ? " eos=\"1\"" : "");

        writer << buffer << std::endl;

        for (auto &entry : word->GetEntryList()) {

            snprintf(
                buffer, 1024,
                "\t\t<entry pron=\"%s\" pos=\"%s\" stem=\"%s\" lemma=\"%s\" />",
                entry->GetPron().c_str(), entry->GetPOS().c_str(),
                entry->GetStem().c_str(), entry->GetLemma().c_str());

            writer << buffer << std::endl;
        }

        writer << "\t</word>" << std::endl;
    }

    writer << "</corpus>";
    writer.flush();

    return writer.str();
}

std::string CCorpus::ToTxt() const {
    std::ostringstream writer;

    char buffer[1024];
    for (auto &word : WordList) {

        snprintf(buffer, 1024, "%s\t%s\t%c%s", word->GetText().c_str(),
                 word->GetBestEntry()->GetPOS().c_str(),
                 word->IsAutoPhonetics() ? '*' : ' ',
                 word->GetBestEntry()->GetPron().c_str());

        writer << buffer << std::endl;
    }

    writer.flush();

    return writer.str();
}

void CCorpus::Dump(const std::string &filename) const {
    std::string extention;
    size_t pos = filename.rfind('.');
    if (pos != std::string::npos) {
        extention = filename.substr(pos + 1);
    }
    std::string content;
    if (extention == "xml") {
        content = ToXml();
    } else {
        content = ToTxt();
    }

    std::ofstream f;
    f.open("log/" + filename);
    f << content;
    f.close();
}