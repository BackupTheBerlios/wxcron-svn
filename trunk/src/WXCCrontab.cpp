/**
 * Name:        WXCCrontab.cpp
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-08
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


#include "WXCCrontab.h"

#include <wx/textfile.h>
#include <wx/regex.h>
#include <wx/filename.h>

#include "WXCLog.h"
#include "WXCApp.h"
#include "wxCron.h"

/*static*/ WXCCrontab WXCCrontab::sCrontab_;

WXCCrontab::WXCCrontab ()
{
}


/*virtual*/ WXCCrontab::~WXCCrontab ()
{
}


/*static*/ bool WXCCrontab::Read ()
{
    wxTextFile file;

    // open the file
    if ( file.Open(WXC_CRONTAB) == false )
    {
        WXCLog::Do(wxString::Format("ERROR: The crontab file %s couldn't be loaded!", WXC_CRONTAB));
        return false;
    }

    // remember modification time
    Instance().timeMod_ = wxFileName(WXC_CRONTAB).GetModificationTime();

    // clear job vector
    Instance().vecJobs_.clear();

    // TODO XXX clear old timers!!!

    // line counter
    long lLine = 1;

    // regex for spaces
    wxRegEx regex("[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+");

    // read the file
    for (wxString strLine = file.GetFirstLine();
         !file.Eof();
         strLine = file.GetNextLine())
    {
        // remove white-spaces from the beginning and end of line
        strLine.Trim(true);
        strLine.Trim(false);

        // ignore blank lines and comments
        if ( !(strLine.IsEmpty())
          && !(strLine.StartsWith("#")) )
        {
            // check for enough parameters
            if ( regex.Matches(strLine) )
            {
                Instance().vecJobs_.push_back(WXCJob(strLine, lLine));
            }
            else
            {
                wxString strErr = wxString::Format("ERROR: Line %d in %s is not valid!\n\"%s\"",
                                                   lLine,
                                                   WXC_CRONTAB,
                                                   strLine);
                wxLogError(strErr);
                WXCLog::Do(strErr);
            }
        }

        // increment line counter
        ++lLine;
    }

    return true;
}

/*static*/ void WXCCrontab::CheckModification ()
{
    if ( Instance().timeMod_ != wxFileName(WXC_CRONTAB).GetModificationTime() )
    {
        WXCLog::Do("crontab was modified. Read it again...");

        if ( !(Read()) )
            wxGetApp().DoClose();
    }
}

#include "WXCTimeValue.h"

void WXCCrontab::XXX (const wxString& str, short iFrom, short iTo)
{
    WXCTimeValue tv(str, iFrom, iTo);
    bool bHas;
    short iNext;
    wxString strLog;

    for (short i = 0; i < 100; ++i)
    {
        bHas = tv.HasThisValue(i);
        iNext = tv.GetNextValue(i);

        WXCLog::Do(wxString::Format("\"%s\" %d - %d: %s;\tCURRENT: %d => NEXT: %d",
                               str,
                               iFrom,
                               iTo,
                               bHas ? "HAS" : "HAS NOT",
                               i,
                               iNext), false);

    }
    WXCLog::Do(wxEmptyString, false);
}

void WXCCrontab::Start ()
{
    for (WXCJobVectorIt it = vecJobs_.begin();
         it != vecJobs_.end();
         ++it)
    {
        (*it).Start();
    }
}
