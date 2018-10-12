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
    TokenzierFunc = nullptr;
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
    if (TokenzierFunc)
        Py_DecRef(TokenzierFunc);
    if (NormalizeFunc)
        Py_DecRef(NormalizeFunc);
    if (TagFunc)
        Py_DecRef(TagFunc);
}

bool CHazm::Load(std::string param) {
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

    TaggerObj =
        PyEval_CallObjectWithKeywords(tagger_class, nullptr, tagger_args);
    Py_DECREF(tagger_class);
    Py_DECREF(tagger_args);
    if (TaggerObj == NULL) {
        PyErr_Print();
        return false;
    }

    TokenzierFunc = PyObject_GetAttrString(HazmObj, "word_tokenize");
    if (!TokenzierFunc || !PyCallable_Check(TokenzierFunc)) {
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
    PyObject *text = PyUnicode_FromString(corpus->GetText().c_str());
    PyObject *args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, text);
    //

    PyObject *normalized = PyObject_CallObject(NormalizeFunc, args);
    if (normalized == nullptr) {
        Py_DECREF(text);
        Py_DECREF(args);
        PyErr_Print();
        return;
    }

    PyTuple_SetItem(args, 0, normalized);
    PyObject *tokens = PyObject_CallObject(TokenzierFunc, args);
    if (tokens == nullptr) {
        Py_DECREF(text);
        Py_DECREF(args);
        PyErr_Print();
        return;
    }

    PyTuple_SetItem(args, 0, tokens);
    PyObject *tags = PyObject_CallObject(TagFunc, args);
    if (tags == nullptr) {
        Py_DECREF(tokens);
        Py_DECREF(text);
        Py_DECREF(args);
        PyErr_Print();
        return;
    }

    int count = (int)PyList_Size(tags);
    PyObject *item, *obj0, *obj1, *wrd, *tag;
    for (int i = 0; i < count; i++) {
        item = PyList_GetItem(tags, i);

        obj0 = PyTuple_GetItem(item, 0);
        obj1 = PyTuple_GetItem(item, 1);

        wrd = PyUnicode_AsUTF8String(obj0);
        tag = PyUnicode_AsUTF8String(obj1);

        CWordPtr word = std::make_unique<CWord>();
        word->SetText(PyString_AsString(wrd));
        // word->SetPOSTag(PyString_AsString(tag));
        corpus->AddWord(word);

        Py_DECREF(wrd);
        Py_DECREF(tag);
        Py_DECREF(item);
    }

    Py_DECREF(tags);
    Py_DECREF(tokens);
}
