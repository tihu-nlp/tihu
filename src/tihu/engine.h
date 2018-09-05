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
#ifndef __TIHU__ENGINE_H
#define __TIHU__ENGINE_H

#pragma once


#include "helper.h"


class IParser;
class ISynthesizer;
class CCorpus;

class CEngine
{
public:
    CEngine();
    ~CEngine();

    int  LoadModules();
    int  LoadSynthesizer(TIHU_VOICE voice);
    void SetCallback(TIHU_CALLBACK callback, void* userData);

    void Speak(const std::string &text);
    void Diacritize(const std::string &text); //TODO:
    void AutoTag(const std::string &text); //TODO:

    void Dump(const std::string &filename);
    void Stop();

    bool SetParam(TIHU_PARAM param, int value);
    bool GetParam(TIHU_PARAM param, int &value);

private:
    void SetText(const std::string &text) const;
    void Process() const;

    void LogText(const std::string& filename) const;
    void LogCorpus(const std::string& filename) const;


private:
    IParser* Hazm;
    IParser* TihuDict;
    IParser* LetterToSound;
    IParser* TextTagger;

    ISynthesizer* Synthesizer;

    CCorpus* Corpus;
};

#endif
