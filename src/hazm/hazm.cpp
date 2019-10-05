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
#include "hazm.h"

#include <Python.h>

CHazm::CHazm() {
  HazmObj = nullptr;
  TaggerObj = nullptr;
  NormalizerObj = nullptr;
  TokenizerFunc = nullptr;
  NormalizeFunc = nullptr;
  TagFunc = nullptr;
}

CHazm::~CHazm() {
  if (HazmObj)
    Py_DecRef(HazmObj);
  if (TaggerObj)
    Py_DecRef(TaggerObj);
  if (NormalizerObj)
    Py_DecRef(NormalizerObj);
  if (TokenizerFunc)
    Py_DecRef(TokenizerFunc);
  if (NormalizeFunc)
    Py_DecRef(NormalizeFunc);
  if (TagFunc)
    Py_DecRef(TagFunc);
}

bool CHazm::Load() {
  PyObject *hazm_name = PyUnicode_FromString("hazm");
  HazmObj = PyImport_Import(hazm_name);
  Py_DECREF(hazm_name);
  if (HazmObj == NULL) {
    PyErr_Print();
    return false;
  }

  PyObject *normalizer_class = PyObject_GetAttrString(HazmObj, "Normalizer");
  if (normalizer_class == NULL) {
    PyErr_Print();
    return false;
  }

  PyObject *normalizer_args = Py_BuildValue("()");
  if (normalizer_args == NULL) {
    PyErr_Print();
    return false;
  }

  NormalizerObj = PyEval_CallObject(normalizer_class, normalizer_args);
  Py_DECREF(normalizer_class);
  Py_DECREF(normalizer_args);
  if (NormalizerObj == NULL) {
    PyErr_Print();
    return false;
  }

  PyObject *tagger_class = PyObject_GetAttrString(HazmObj, "POSTagger");
  if (tagger_class == NULL) {
    PyErr_Print();
    return false;
  }

  PyObject *tagger_args =
      Py_BuildValue("{s:s}", "model", "./data/hazm/postagger.model");
  if (tagger_args == NULL) {
    PyErr_Print();
    return false;
  }

  TaggerObj = PyEval_CallObjectWithKeywords(tagger_class, nullptr, tagger_args);
  Py_DECREF(tagger_class);
  Py_DECREF(tagger_args);
  if (TaggerObj == NULL) {
    PyErr_Print();
    return false;
  }

  TokenizerFunc = PyObject_GetAttrString(HazmObj, "word_tokenize");
  if (!TokenizerFunc || !PyCallable_Check(TokenizerFunc)) {
    PyErr_Print();
    return false;
  }

  NormalizeFunc = PyObject_GetAttrString(NormalizerObj, "normalize");
  if (!NormalizeFunc || !PyCallable_Check(NormalizeFunc)) {
    PyErr_Print();
    return false;
  }

  TagFunc = PyObject_GetAttrString(TaggerObj, "tag");
  if (!TagFunc || !PyCallable_Check(TagFunc)) {
    PyErr_Print();
    return false;
  }

  return true;
}

void CHazm::ParsText(CCorpus *corpus) {
  PyObject *tokens = PyList_New(corpus->Size());
  CWordList &word_list = corpus->GetWordList();
  int index = 0;
  for (auto &word : word_list) {
    std::string txt = word->GetTextWithoutDiacritics();
    PyObject *token = PyUnicode_FromString(txt.c_str());
    PyList_SetItem(tokens, index++, token);
  }

  PyObject *args = PyTuple_New(1);
  PyTuple_SetItem(args, 0, tokens);
  PyObject *tags = PyObject_CallObject(TagFunc, args);
  if (tags == nullptr) {
    Py_DECREF(tokens);
    Py_DECREF(args);
    PyErr_Print();
    return;
  }

  index = 0;
  for (auto &word : word_list) {
    if (IsStopped) {
      break;
    }

    PyObject *item = PyList_GetItem(tags, index++);

    // PyObject * obj0 = PyTuple_GetItem(item, 0);
    PyObject *obj1 = PyTuple_GetItem(item, 1);

    // PyObject *wrd = PyUnicode_AsUTF8String(obj0);
    PyObject *obj1str = PyUnicode_AsUTF8String(obj1);
    std::string pos_hazm = PyBytes_AsString(obj1str);

    bool is_genitive = EndsWith(pos_hazm, "e");
    if (is_genitive) {
      RemoveLast(pos_hazm);
    }
    CEntryList &entry_list = word->GetEntryList();
    CEntryList::iterator it = entry_list.begin();
    int heaviest = 0;
    bool heaviest_has_genitive = "";
    std::string heaviest_pron = "";

    while (it != entry_list.end()) {
      CEntryPtr &entry = (*it);

      if (heaviest <= entry->GetWeight()) {
        heaviest = entry->GetWeight();
        heaviest_pron = entry->GetPron();
        heaviest_has_genitive = entry->HasGenitive();
      }

      std::string pos_lexicon = entry->GetPOS();
      if (pos_lexicon == ".") {
        // words from g2p
        entry->SetPOS(pos_hazm);
        if (is_genitive) {
          entry->AddGenitive();
        }
      } else {

        // N_GEN -> N
        std::string pos_lexicon = Substr(entry->GetPOS(), "_");
        bool has_genitive = entry->HasGenitive();

        if (pos_hazm == "N" || pos_hazm == "AJ" || pos_hazm == "V") {
          if (pos_lexicon != pos_hazm) {
            entry_list.erase(it--);
          } else if (entry_list.front()->GetWeight() > entry->GetWeight()) {
            entry_list.erase(it--);
          } else {
            if (it != entry_list.begin()) {
              entry_list.erase(entry_list.begin());
            }
            if (is_genitive && !has_genitive) {
              entry->AddGenitive();
            }
          }
        } else {
          if (pos_lexicon != "*") {
            entry_list.erase(it--);
          } else if (entry_list.front()->GetWeight() > entry->GetWeight()) {
            it = entry_list.erase(it--);
          } else {
            if (it != entry_list.begin()) {
              entry_list.erase(entry_list.begin());
            }
            entry->SetPOS(pos_hazm);
            if (is_genitive && !has_genitive) {
              entry->AddGenitive();
            }
          }
        }
      }

      ++it;
    }

    if (word->IsEmpty()) {
      if (!heaviest_pron.empty()) {
        TIHU_WARNING(stderr,
                     "Unable to find '%s' at lexicon with label '%s'.\n",
                     word->GetText().c_str(), pos_hazm.c_str());
      }
      CEntryPtr entry = std::make_unique<CEntry>();
      entry->SetPOS(pos_hazm);
      entry->SetPron(heaviest_pron);
      if (is_genitive && !heaviest_has_genitive) {
        entry->AddGenitive();
      }
      word->AddEntry(entry);
    }
  }

  corpus->Dump("hazm.xml");
}
