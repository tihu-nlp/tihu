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
#include "word2phoneme.h"

#include <algorithm>

#define LOG_UNKNOWN_WORDS

CWord2Phoneme::CWord2Phoneme() {
#ifdef LOG_UNKNOWN_WORDS
  LoadUnknownWords();
#endif
}

CWord2Phoneme::~CWord2Phoneme() {
#ifdef LOG_UNKNOWN_WORDS
  SaveUnknownWords();
#endif
}

bool CWord2Phoneme::LoadModel(const std::string &model) {
  return g2p.LoadModel(model);
}

void CWord2Phoneme::LoadUnknownWords() {
  std::string log_file = "./log/unknown_words.txt";
  FILE *file = fopen(log_file.c_str(), "r");

  if (!file) {
    return;
  }

  char buf[1024];
  while (fgets(buf, 1024, file)) {
    try {
      std::string word = strtok(buf, "\t");
      int weight = std::stoi(strtok(NULL, "\n"));

      UnknownWords[word] = weight;
    } catch (...) {
    }
  }

  fclose(file);
}

void CWord2Phoneme::SaveUnknownWords() {
  std::string log_file = "./log/unknown_words.txt";
  FILE *file = fopen(log_file.c_str(), "w");

  if (!file) {
    return;
  }

  /// make it sorted
  std::vector<std::pair<std::string, int>> sorted;
  for (auto const &i : UnknownWords) {
    sorted.push_back(std::pair<std::string, int>(i.first, i.second));
  }

  struct {
    bool operator()(const std::pair<std::string, int> &a,
                    const std::pair<std::string, int> &b) {
      return a.second > b.second;
    }
  } sort_by_weight;

  std::sort(sorted.begin(), sorted.end(), sort_by_weight);

  char buf[1024];
  for (auto const &i : sorted) {
    std::string word = i.first;
    int weight = i.second;

    sprintf(buf, "%s\t%d\n", word.c_str(), weight);
    fwrite(buf, 1, strlen(buf), file);
  }

  fclose(file);
}

std::string CWord2Phoneme::Convert(std::string word) {
#ifdef LOG_UNKNOWN_WORDS
  ++UnknownWords[word];
#endif

  std::string pron = g2p.Convert(word);

  return pron;
}
