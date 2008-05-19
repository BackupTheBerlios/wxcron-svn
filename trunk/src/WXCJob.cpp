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


WXCJob::WXCJob (const wxString& strOriginalLine, long lLine)
      : lLine_(lLine),
        pTimer_(NULL)
{
    Parse(strOriginalLine);
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
    {
        pTimer_->Stop();
        pTimer_->SetCommand(strCommand_);
    }
    else
    {
        pTimer_ = new WXCTimer(this, strCommand_);
    }

    // start the timer
    pTimer_->Start(CalculateNextTime());
}


wxDateTime WXCJob::CalculateNextTime ()
{
    wxDateTime dtNow(wxDateTime::Now());

    // XXX if (
}

void WXCJob::Parse (const wxString& strOriginalLine)
{
    wxArrayString   arrStrings;
    wxString        str(strOriginalLine);
    wxString        strCurrent;
    size_t          idx;

    // extract the parameters
    while ( !(str.IsEmpty()) || arrStrings.GetCount() < 5 )
    {
        // find white spaces at the beginning
        idx = 0;
        while (str[idx] == ' ' || idx == str.Length())
            ++idx;

        // erase white spaces at the beginning
        str = str.Mid(idx);

        //
        strCurrent = str.BeforeFirst(' ');
        str = str.Mid(strCurrent.Length());

        //
        arrStrings.Add(strCurrent);
    }

    // enough parameters
    if (arrStrings.GetCount() < 5)
    {
        wxString strErr(wxString::Format("ERROR: Not enough parameters in this line! %s (line %d)", strOriginalLine, lLine_));
        wxLogError(strErr);
        WXCLog::Do(strErr);
        return;
    }

    // command
    strCommand_ = str;

    // minutes
    Parse(arrStrings[0], minute_, 0, 59);

    // hours
    Parse(arrStrings[1], hour_, 0, 23);

    // days
    Parse(arrStrings[2], day_, 1, 31);

    // month
    Parse(arrStrings[3], month_, 1, 12);

    // day of week
    Parse(arrStrings[4], weekday_, 1, 7);
}

void WXCJob::Parse (const wxString& str,
                    longVector& rVec,
                    int iRangeFrom,
                    int iRangeTo)
{
    // just (*)
    if (str == '*')
    {
        FillValues(rVec, iRangeFrom, iRangeTo, 1);
        return;
    }

    // number-steps (*/n)
    if (str.Find('/'))
    {
        long lStep;

        if ( str.AfterFirst('/').ToLong(&lStep) )
        {
            FillValues(rVec, iRangeFrom, iRangeTo, lStep);
        }
        else
        {
            wxString strErr(wxString::Format("ERROR: The step parameter (%s) couldn't be parsed. (line %d)", str, lLine_));
            wxLogError(strErr);
            WXCLog::Do(strErr);
        }

        return;
    }

    // number-list (n,m)
    if (str.Find(','))
    {
        wxStringTokenizer tkn(str, ',');
        long              lVal;

        while ( tkn.HasMoreTokens() )
        {
            if ( tkn.GetNextToken().ToLong(&lVal) )
            {
                if ((int)lVal >= iRangeFrom && (int)lVal <= iRangeTo)
                {
                    rVec.push_back(lVal);
                }
                else
                {
                    wxString strErr(wxString::Format("ERROR: The value (%d) is out of range in line %d. Needed range is %d - %d.", lVal, lLine_, iRangeFrom, iRangeTo));
                    wxLogError(strErr);
                    WXCLog::Do(strErr);
                }
            }
            else
            {
                wxString strErr(wxString::Format("ERROR: The list parameter (%s) couldn't be parsed. (line %d)", str, lLine_));
                wxLogError(strErr);
                WXCLog::Do(strErr);
            }
        }
        return;
    }

    // number-range (n-m)
    if (str.Find('-'))
    {
        long lFrom, lTo;

        if ( str.BeforeFirst('-').ToLong(&lFrom)
          && str.AfterLast('-').ToLong(&lTo) )
        {
            if ( (int)lFrom >= iRangeFrom && (int)lTo <= iRangeTo )
            {
                FillValues(rVec, lFrom, lTo, 1);
            }
            else
            {
                wxString strErr(wxString::Format("ERROR: The wrong range (%s) in line %d. Needed range is %d - %d.", str, lLine_, iRangeFrom, iRangeTo));
                wxLogError(strErr);
                WXCLog::Do(strErr);
            }
        }
        else
        {
            wxString strErr(wxString::Format("ERROR: The range parameter (%s) in line %d couldn't be parsed.", str, lLine_));
            wxLogError(strErr);
            WXCLog::Do(strErr);
        }

        return;
    }

    // number (n)
    long lNumber;
    if ( str.ToLong(&lNumber) )
    {
        if ( (int)lNumber >= iRangeFrom && (int)lNumber <= iRangeTo )
        {
            rVec.push_back(lNumber);
        }
        else
        {
            wxString strErr(wxString::Format("ERROR: The value (%d) is out of range in line %d. Needed range is %d - %d.", lNumber, lLine_, iRangeFrom, iRangeTo));
            wxLogError(strErr);
            WXCLog::Do(strErr);
        }

    }
    else
    {
        wxString strErr(wxString::Format("ERROR: The number parameter (%s) in line %d couldn't be parsed.", str, lLine_));
        wxLogError(strErr);
        WXCLog::Do(strErr);
    }
}

void WXCJob::FillValues (longVector& rVec,
                         int iRangeFrom,
                         int iRangeTo,
                         int iSteps)
{
    for (int i = iRangeFrom;
         i <= iRangeTo;
         i = i + iSteps)
    {
        rVec.push_back(i);
    }
}
