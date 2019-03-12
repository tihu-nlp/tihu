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
#ifndef __TIHU__PARSER_H
#define __TIHU__PARSER_H

#pragma once

#include "corpus/corpus.h"
#include "helper.h"
#include "settings.h"

#include <functional>

class IParser {
  public:
    typedef std::function<int(int,long,long)> callback_t;

    IParser();
    virtual ~IParser();

    virtual bool Load(std::string param = "") = 0;
    virtual void ParsText(CCorpus *corpus) = 0;
    virtual void SetSettings(CSettings *settings);
    virtual void SetCallback(const callback_t& cb);

protected:
    void Message(const char *message) const;
    void MessageF(const char *message, ...) const;

  protected:
    CSettings *Settings;
    callback_t Callback;
};

#endif
