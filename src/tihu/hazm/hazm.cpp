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
#include "path_manager.h"

#include <Python.h>


CHazm::CHazm()
{    
    TokenizerObj = nullptr;
    NormalizerObj = nullptr;
    POSTaggerObj = nullptr;

    Py_Initialize();
}

CHazm::~CHazm()
{
    if(TokenizerObj)  Py_DECREF(TokenizerObj);
    if(NormalizerObj) Py_DECREF(NormalizerObj);
    if(POSTaggerObj)  Py_DECREF(POSTaggerObj);
    
    Py_Finalize();
}

bool CHazm::Load(const std::string &hazm_model)
{
    //std::string hazm_folder = CPathManager::GetInstance()->GetHazmFolder();
    std::string hazm_folder = "/home/mostafa/Projects/tihu-nlp/hazm/hazm/";
    
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\"/home/mostafa/Projects/tihu-nlp/hazm/hazm/\")");
    
    PyObject* tokenizer_name  = PyUnicode_FromString("WordTokenizer");
    PyObject* normalizer_name = PyUnicode_FromString("Normalizer");
    PyObject* postagger_name  = PyUnicode_FromString("POSTagger");

    PyObject* tokenizer_module  = PyImport_Import(tokenizer_name);
    PyObject* normalizer_module = PyImport_Import(normalizer_name);
    PyObject* postagger_module  = PyImport_Import(postagger_name);

    Py_DECREF(tokenizer_name);
    Py_DECREF(normalizer_name);
    Py_DECREF(postagger_name);

    if (tokenizer_module  == NULL ||
        normalizer_module == NULL ||
        postagger_module == NULL) {
        PyErr_Print();

        return false;
    }
    return true;

    PyObject* tokenizer_dict  = PyModule_GetDict(tokenizer_module);
    PyObject* normalizer_dict = PyModule_GetDict(normalizer_name);
    PyObject* postagger_dict  = PyModule_GetDict(postagger_name);

    // Build the name of a callable class 
    PyObject* tokenizer_class  = PyDict_GetItemString(tokenizer_dict,  "WordTokenizer.py");
    PyObject* normalizer_class = PyDict_GetItemString(normalizer_dict, "Normalizer.py");
    PyObject* postagger_class  = PyDict_GetItemString(postagger_dict,  "POSTagger.py");

    // Create an instance of the class
    if (!PyCallable_Check(tokenizer_class) ||
        !PyCallable_Check(normalizer_class) ||
        !PyCallable_Check(postagger_class))    {
        PyErr_Print();
        return false;
    }

    //char args[1024];
    //sprintf(args, "model='%s'", hazm_model.c_str());
    //PyObject* postagger_args = PyUnicode_DecodeFSDefault(args);
    //
    //PyObject *TokenizerObj  = PyObject_CallObject(tokenizer_class, NULL);
    //PyObject *NormalizerObj = PyObject_CallObject(normalizer_class, NULL);
    //PyObject *POSTaggerObj  = PyObject_CallObject(postagger_class, postagger_args);
    //
    //if (TokenizerObj  == NULL ||
    //    NormalizerObj == NULL ||
    //    POSTaggerObj  == NULL) {
    //    PyErr_Print();
    //    return false;
    //}

    return true;
}

void CHazm::ParsText(CCorpus* corpus)
{
}
