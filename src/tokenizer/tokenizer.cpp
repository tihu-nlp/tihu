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
#include "tokenizer.h"
#include "../file_manager.h"
#include "char_mapper.h"

CTokenizer::CTokenizer() { Offset = 0; }

bool CTokenizer::Load(std::string param) {
    CFileManager file_manager;

    if (file_manager.OpenFile("./data/tokens.txt")) {
        return false;
    }

    CCharMap char_map;
    while (file_manager.ReadLine()) {
        std::string code_l = file_manager.NextPiece();
        std::string letter = file_manager.NextPiece();
        std::string code_n = file_manager.NextPiece();
        std::string normed = file_manager.NextPiece();
        std::string type = file_manager.NextPiece();

        if (type.empty()) {
            TIHU_WARNING(stderr, "error: line %d: wrong token type.",
                         file_manager.GetLineNum());
            break;
        }

        char_map.Fill(normed, std::stoi(code_n), (TIHU_TOKEN_TYPE)type[0]);
        CharMapper.SetCharMap(std::stoi(code_l), char_map);
    }

    CharMapper.SetCharMap('\t',
                          CCharMap("\t", '\t', TIHU_TOKEN_TYPE::DELIMITER));
    CharMapper.SetCharMap(' ', CCharMap(" ", ' ', TIHU_TOKEN_TYPE::DELIMITER));

    return true;
}

void CTokenizer::ParsText(CCorpus *corpus) {
    std::string normed;
    TIHU_TOKEN_TYPE token_type = TIHU_TOKEN_TYPE::DELIMITER;
    int length = 0;
    CWordPtr word = std::make_unique<CWord>();
    std::u16string text_16 = UTF8ToUTF16(corpus->GetText());
    const char16_t *text = text_16.c_str();

    /// reset text offset
    Offset = corpus->GetOffset();

    while (1) {
        CCharMap char_map = CharMapper.GetCharMap(*text);

        ///
        if (token_type != char_map.GetType()) {
            if (token_type != TIHU_TOKEN_TYPE::DELIMITER) {

                if (EndsWith(normed, CHR_U8_ZWNJ)) { /// ignore znwj at the end
                    normed = RemoveLast(normed);
                }

                if (!normed.empty()) {
                    word->SetText(normed);
                    word->SetOffset(Offset);
                    word->SetLength(length);
                    word->SetType(token_type);

                    corpus->AddWord(word);

                    word = std::make_unique<CWord>();
                }
            }

            // ---
            normed.clear();
            Offset += length;
            length = 0;
            token_type = char_map.GetType();
        }

        if (!*text) {
            break;
        }

        //// -------------------------------------------------
        switch (token_type) {
        case TIHU_TOKEN_TYPE::PERSIAN: { // Persian

            if (char_map.IsDiacritic()) {
                /// don't add irab to normed string
                word->SetHasDiacritic(true);
            }

            switch (char_map.GetCode()) {
            case 0x200c: { // zwnj
                if (!(normed.empty() || EndsWith(normed, CHR_U8_ZWNJ))) {
                    normed += CHR_U8_ZWNJ;
                }
            } break;

            case 0x0640: { // ـ (Ignore Tatwell)
            } break;

            case 0x0621: { // ARABIC LETTER HAMZA
                if (EndsWith(normed, CHR_U8_HE)) {
                    normed += CHR_U8_HE;
                    normed += CHR_U8_HAMZE;
                } else {
                    normed + char_map.GetNormed();
                }
            } break;

            case 0x06C0: { // ARABIC LETTER HEH WITH YEH ABOVE
                normed += CHR_U8_HE;
                normed += CHR_U8_HAMZE;
            } break;

            case 0xFEFB: { /// ARABIC LIGATURE LAM WITH ALEF ISOLATED FORM
                normed += CHR_U8_LAM;
                normed += CHR_U8_ALEF;
            } break;

            case 0xFDF2: { // allah
                normed += CHR_U8_ALEF;
                normed += CHR_U8_LAM;
                normed += CHR_U8_LAM;
                normed += CHR_U8_HE;
            } break;

            default: { normed += char_map.GetNormed(); } break;
            }
        } break;

        case TIHU_TOKEN_TYPE::PUNCTUATION: {
            int event_size = ParsEvents(word, text);

            if (event_size == 1) {
                /// it's slash
                text += event_size;
            } else if (event_size > 1) {
                /// it's event, ignore them
                text += event_size;
                token_type = TIHU_TOKEN_TYPE::DELIMITER;
                continue;
            } else {
                normed += char_map.GetNormed();
            }
        } break;

        case TIHU_TOKEN_TYPE::DELIMITER: { //
            /// ignore delimiters
            /// White spaces (space, tab, new line, and carriage return) can be
            /// viewed as separators.
            if (char_map.IsLineBreak()) {
                const CWordPtr &last_token = corpus->GetLastWord();

                if (last_token) {
                    last_token->SetIsEndOfParagraph(true);
                }
            }

        } break;

        default: { normed += char_map.GetNormed(); } break;
        }

        ++length;
        ++text;
    }

    corpus->Dump("tokens.xml");
}

int CTokenizer::ParsEvents(const CWordPtr &word, const char16_t *text) {
    const char16_t *start_tag = text;

    /// it's an event!
    while (1) {

        if (*start_tag != '/') {
            break;
        }

        /// it's not an event if two slashes come together.
        /// it is a slash character: '/'.
        if (*(start_tag + 1) == '/') {
            start_tag++;
            break;
        }

        const char16_t *colon = u16chr(start_tag + 1, ':');
        if (!colon) {
            TIHU_WARNING(stderr, "Wrong event at offset %d", Offset);
            break;
        }

        const char16_t *end_tag = u16chr(colon + 1, '/');
        if (!end_tag) {
            TIHU_WARNING(stderr, "Wrong event at offset %d", Offset);
            break;
        }

        int event_length = (int)(end_tag - start_tag);

        std::string event_type =
            UTF16ToUTF8(std::u16string(start_tag + 1, colon));

        std::string event_value =
            UTF16ToUTF8(std::u16string(colon + 1, end_tag));

        CEventPtr event = CEventPtr(new CEvent());

        event_type = ToLower(event_type);

        if (event_type.compare("offset") == 0) {
            Offset = std::stoi(event_value.c_str());
        } else if (event_type.compare("mark") == 0) {
            word->AddEvent(TIHU_EVENT_TYPE::BOOKMARK, event_value);
        } else if (event_type.compare("volume") == 0) {
            word->AddEvent(TIHU_EVENT_TYPE::VOLUME_RATIO, event_value);
        } else if (event_type.compare("pitch") == 0) {
            word->AddEvent(TIHU_EVENT_TYPE::PITCH_RATIO, event_value);
        } else if (event_type.compare("speed") == 0) {
            word->AddEvent(TIHU_EVENT_TYPE::SPEED_RATIO, event_value);
        } else if (event_type.compare("silence") == 0) {
            word->AddEvent(TIHU_EVENT_TYPE::SILENCE, event_value);
        } else if (event_type.compare("spell") == 0) {
            word->AddEvent(TIHU_EVENT_TYPE::SPELL_OUT, event_value);
        } else {
            word->AddEvent(TIHU_EVENT_TYPE::UNKNOWN, event_value);
        }

        start_tag += event_length + 1;
    }

    return start_tag - text;
}