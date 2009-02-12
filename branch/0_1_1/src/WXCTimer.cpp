/**
 * Name:        WXCTimer.cpp
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-19
 * Copyright:   (c) 2008 Christian Buhtz <exsudat@gmx.de>
 * Licence:     GNU General Public License (Version 3)
 ***
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/


#include "WXCTimer.h"

#include <wx/utils.h>

#include "WXCLog.h"
#include "WXCJob.h"
#include "WXCTimestampFile.h"

WXCTimer::WXCTimer (WXCJob* pParentJob)
        : pParentJob_(pParentJob)
{
}


/*virtual*/ WXCTimer::~WXCTimer ()
{
}


bool WXCTimer::Start (const wxDateTime& timeNextWakeUp)
{
    timeNextWakeUp_ = wxInvalidDateTime;

    // valid?
    if (timeNextWakeUp < wxDateTime::Now())
        return false;

    // get intervall in milliseconds
    wxLongLong llMsec = timeNextWakeUp.Subtract(wxDateTime::Now()).GetMilliseconds();

    // take care of the maximum value for 'int'
    if (llMsec > wxLongLong(0, INT_MAX))
    {
        timeNextWakeUp_ = timeNextWakeUp;

        return wxTimer::Start(INT_MAX, wxTIMER_ONE_SHOT);
    }

    return wxTimer::Start(llMsec.ToLong(), wxTIMER_ONE_SHOT);
}

void WXCTimer::Notify ()
{
    if (timeNextWakeUp_.IsValid())
    {
        // restart timer
        Start (timeNextWakeUp_);
    }
    else
    {
        // execute job
        pParentJob_->Execute();
    }
}
