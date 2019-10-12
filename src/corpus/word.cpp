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
#include "word.h"
#include "../utf8/source/utf8.h"
#include "entry.h"

#define WORD_FLAG_END_OF_SENTENCE 0x0001
#define WORD_FLAG_END_OF_PARAGRAPH 0x0002
#define WORD_FLAG_HAS_DIACRITIC 0x0004
#define WORD_FLAG_AUTO_PHONETICS 0x0008

CWord::CWord()
    : Type(TIHU_TOKEN_TYPE::UNKNOWN), Length(0), Offset(0), Flags(0) {}

CWord::CWord(CWord &word)
    : Text(word.Text), Type(word.Type), Length(word.Length),
      Offset(word.Offset), Flags(word.Flags) {}

CWord::~CWord() {}

void CWord::SetText(const std::string &text) {
  Text = text; //
}

void CWord::SetType(TIHU_TOKEN_TYPE type) {
  Type = type; //
}

void CWord::SetLength(size_t length) {
  Length = length; //
}

void CWord::SetOffset(size_t offset) {
  Offset = offset; //
}

std::string CWord::GetText() const {
  return Text; //
}

std::string CWord::GetTextWithoutDiacritics() const {
  if (!this->HasDiacritic()) {
    return Text;
  }
  std::u16string text = UTF8ToUTF16(Text);
  std::u16string striped;
  for (const char16_t &c : text) {
    if (!IsDiacriticChar(c)) {
      striped += c;
    }
  }

  return UTF16ToUTF8(striped);
}

TIHU_TOKEN_TYPE CWord::GetType() const {
  return Type; //
}

size_t CWord::GetLength() const {
  return Length; //
}

size_t CWord::GetOffset() const {
  return Offset; //
}

bool CWord::IsPersianWord() const {
  return (Type == TIHU_TOKEN_TYPE::PERSIAN); //
}

bool CWord::IsNonPersianWord() const {
  return (Type == TIHU_TOKEN_TYPE::NON_PERSIAN); //
}

bool CWord::IsPunctuation() const {
  return (Type == TIHU_TOKEN_TYPE::PUNCTUATION); //
}

bool CWord::IsNumber() const {
  return (Type == TIHU_TOKEN_TYPE::NUMBER); //
}

void CWord::SetIsEndOfParagraph(bool is_end_of_paragraph) {
  if (is_end_of_paragraph) {
    SET_FLAG(Flags, WORD_FLAG_END_OF_PARAGRAPH);
  } else {
    UNSET_FLAG(Flags, WORD_FLAG_END_OF_PARAGRAPH);
  }
}

void CWord::SetIsEndOfSentence(bool is_end_of_sentence) {
  if (is_end_of_sentence) {
    SET_FLAG(Flags, WORD_FLAG_END_OF_SENTENCE);
  } else {
    UNSET_FLAG(Flags, WORD_FLAG_END_OF_SENTENCE);
  }
}

void CWord::SetHasDiacritic(bool has_diacritic) {
  if (has_diacritic) {
    SET_FLAG(Flags, WORD_FLAG_HAS_DIACRITIC);
  } else {
    UNSET_FLAG(Flags, WORD_FLAG_HAS_DIACRITIC);
  }
}

bool CWord::IsEndOfParagraph() const {
  return IS_FLAG_SET(Flags, WORD_FLAG_END_OF_PARAGRAPH); //
}

bool CWord::IsEndOfSentence() const {
  return IS_FLAG_SET(Flags, WORD_FLAG_END_OF_SENTENCE); //
}

bool CWord::HasDiacritic() const {
  return IS_FLAG_SET(Flags, WORD_FLAG_HAS_DIACRITIC); //
}

CPhonemeList &CWord::GetPhonemeList() {
  return PhonemeList; //
}

CEventList &CWord::GetEventList() {
  return EventList; //
}

CEntryList &CWord::GetEntryList() {
  return EntryList; //
}

const CEntryPtr &CWord::GetFirstEntry() const {
  auto entry = EntryList.begin();

  if (entry != EntryList.end()) {
    return *entry;
  }

  TIHU_WARNING(stderr, "no entry for '%s'.\n", Text.c_str());
  return nop_entry;
}

void CWord::ParsPron() {
  std::string pron = GetFirstEntry()->GetPron();

  for (size_t index = 0; index < pron.length(); index++) {
    CPhonemePtr phoneme = std::make_unique<CPhoneme>();

    char prv_pho = (index > 0) ? pron[index - 1] : 0;
    char cur_pho = pron[index];
    char nxt_pho = pron[index + 1];

    if (cur_pho == '^') {
      continue;
    }

    phoneme->SetPhonetic(prv_pho, cur_pho, nxt_pho);

    PhonemeList.push_back(std::move(phoneme));
  }
}

void CWord::AddEvent(CEventPtr &event) {
  EventList.push_back(std::move(event)); //
}

void CWord::AddEntry(CEntryPtr &entry) {
  EntryList.push_back(std::move(entry)); //
}

void CWord::AddEvent(TIHU_EVENT_TYPE event_type,
                     TIHU_EVENT_VALUE &event_value) {
  EventList.push_back(std::make_unique<CEvent>(event_type, event_value));
}

bool CWord::IsEmpty() const {
  return EntryList.empty(); //
}

void CWord::SetIsAutoPhonetics(bool lts) {
  SET_FLAG(Flags, WORD_FLAG_AUTO_PHONETICS); //
}

bool CWord::IsAutoPhonetics() const {
  return IS_FLAG_SET(Flags, WORD_FLAG_AUTO_PHONETICS); //
}

bool CWord::EndsWithKasre() const { return EndsWith(Text, CHR_U8_KASRE); }