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
#ifndef __TIHU__EVENT_H
#define __TIHU__EVENT_H

#pragma once

#include "../helper.h"

class CEvent {
  public:
    CEvent();
    CEvent(TIHU_EVENT_TYPE type, TIHU_EVENT_VALUE value);

    TIHU_EVENT_TYPE GetType() const;
    TIHU_EVENT_VALUE GetValue() const;

  public:
    TIHU_EVENT_TYPE Type;
    TIHU_EVENT_VALUE Value;
};

typedef std::unique_ptr<CEvent> CEventPtr;
typedef std::list<CEventPtr> CEventList;

#endif
