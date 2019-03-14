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
#include "punctuation_to_phonetic.h"
#include "../file_manager.h"


#include <algorithm>


CPunctuationToPhoneme::CPunctuationToPhoneme()
{
}

CPunctuationToPhoneme::~CPunctuationToPhoneme()
{
}

bool CPunctuationToPhoneme::Load(std::string filename)
{
    CFileManager file_manager;

    if(file_manager.OpenFile(filename)) {
        return false;
    }

    while(file_manager.ReadLine()) {

        std::string text = file_manager.NextPiece();
        std::string status = file_manager.NextPiece();
        std::string pron = file_manager.NextPiece();

        CPunctuationPtr punctuation = std::make_unique<CPunctuation>();

        punctuation->Text = text;
        punctuation->Pronunciation = pron;
        punctuation->ReadStatus = (CPunctuation::READ_STATUS)std::stoi(status);

        Punctuations.insert(
            std::pair<std::string, CPunctuationPtr>(
                text, std::move(punctuation)));
    }

    return true;
}

std::string CPunctuationToPhoneme::Convert(const std::string &text)
{
    std::string pronunciation;

    std::map<std::string, CPunctuationPtr>
        ::const_iterator iter = Punctuations.find(text);

    if(iter != Punctuations.cend()) {
        pronunciation = iter->second->Pronunciation;
    }

    return pronunciation;
}
