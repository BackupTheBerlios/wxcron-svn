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


WXCJob::WXCJob (const wxString& strOriginalLine,
                long lLine,
                const wxArrayString& arrOptions)
      : lLine_(lLine),
        pTimer_(NULL),
        strOriginalLine_(strOriginalLine),
        time_(strOriginalLine),
        bOption_nocatchup_(false)
{
    // the command
    strCommand_ = time_.GetUnparsed();
    strCommand_.Trim(true);
    strCommand_.Trim(false);

    // options
    SetOptions(arrOptions);
}


/*virtual*/ WXCJob::~WXCJob ()
{
    if (pTimer_)
        delete pTimer_;
}

void WXCJob::Start ()
{
    wxDateTime dtNextExec;

    // old timer there?
    if (pTimer_)
        pTimer_->Stop();
    else
        pTimer_ = new WXCTimer(this);

    // get last execution
    wxDateTime dtLastExec = WXCTimestampFile::Instance().GetLast(GetOriginalLine());

    // there is a last execution timestamp
    if ( dtLastExec.IsValid() )
    {
        // last execution in the past
        if ( dtLastExec <= wxDateTime::Now() )
        {
            // catchup ?
            if ( !bOption_nocatchup_ )
            {
                // catch up the job
                if ( time_.GetNext(dtLastExec) < wxDateTime::Now() )
                {
                    Execute();
                    return;
                }
                else
                {
                    dtNextExec = time_.GetNext(dtLastExec);
                }
            }
            else
            {
                dtNextExec = time_.GetNext();
            }
        }
        else
        // its in the future
        {
            dtNextExec = dtLastExec;
        }
    }

    if ( !(dtNextExec.IsValid()) )
        dtNextExec = time_.GetNext();

    // start the timer
    pTimer_->Start(dtNextExec);

    // remember timestamp for future execution
    if ( !(dtLastExec.IsValid()) )
    {
        WXCTimestampFile::Instance().Set(GetOriginalLine(), dtNextExec);
        WXCTimestampFile::Instance().Save();
    }
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

void WXCJob::SetOptions(const wxArrayString& arrOptions)
{
    for ( size_t i = 0; i < arrOptions.GetCount(); ++i )
    {
        if  ( arrOptions[i] == "@nocatchup" )
        {
            bOption_nocatchup_ = true;
            break;
        }
        else
        {
            WXCLog::Do(wxString::Format("ERROR: Unknown option \"%s\" in crontab-line: %d)",
                                        arrOptions[i],
                                        GetLine()));
            break;
        }
    }
}
