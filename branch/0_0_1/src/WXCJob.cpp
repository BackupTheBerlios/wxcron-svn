/**
 * Name:        WXCJob.cpp
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-07
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


#include "WXCJob.h"

#include <wx/tokenzr.h>
#include <wx/log.h>
#include "WXCTimer.h"
#include "WXCLog.h"
#include "WXCTimestampFile.h"


WXCJob::WXCJob (const wxString& strOriginalLine, long lLine)
      : lLine_(lLine),
        pTimer_(NULL),
        strOriginalLine_(strOriginalLine),
        time_(strOriginalLine)
{
    strCommand_ = time_.GetUnparsed();
    strCommand_.Trim(true);
    strCommand_.Trim(false);
}


/*virtual*/ WXCJob::~WXCJob ()
{
    if (pTimer_)
        delete pTimer_;
}


void WXCJob::Start ()
{
    // old timer there?
    if (pTimer_)
        pTimer_->Stop();
    else
        pTimer_ = new WXCTimer(this);

    // get last execution
    wxDateTime dt = WXCTimestampFile::Instance().GetLast(GetOriginalLine());

    if ( dt.IsValid() )
    {
        // catch up the job
        if ( time_.GetNext(dt) < wxDateTime::Now() )
        {
            Execute();
            return;
        }
    }

    // start the timer
    pTimer_->Start(time_.GetNext());
}


void WXCJob::Execute ()
{
    // log execution
    WXCLog::Do(wxString::Format("Execute Job. (\"%s\" crontab-line: %d)",
                                GetOriginalLine(),
                                GetLine()));

    // remember timestamp
    WXCTimestampFile::Instance().Set(GetOriginalLine());
    WXCTimestampFile::Instance().Save();

    // execute
    wxExecute(strCommand_, wxEXEC_ASYNC);

    // restart job
    Start();
}


const wxString& WXCJob::GetOriginalLine ()
{
    return strOriginalLine_;
}

long WXCJob::GetLine ()
{
    return lLine_;
}
