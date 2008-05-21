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
// XXX
#include <wx/msgdlg.h>

#include "WXCTimer.h"
#include "WXCLog.h"


WXCJob::WXCJob (const wxString& strOriginalLine, long lLine)
      : lLine_(lLine),
        pTimer_(NULL),
        strOriginalLine_(strOriginalLine)
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
    // XXX
    wxDateTime dtNow(20, wxDateTime::May, 2008, 12, 54, 37);
    wxDateTime dtNext;

    WXCLog::Do(wxString::Format("%s (Line: %d)", strOriginalLine_, lLine_));
    for (int i = 0;
         i < 5000;
         ++i)
    {
        dtNext = CalculateNextTime(dtNow);
        WXCLog::Do(wxString::Format("NOW:  %s\nNEXT: %s\n", dtNow.Format(), dtNext.Format()), false);
        dtNow = dtNext;
    }
    return;

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

/*static*/ long WXCJob::GetNextValue (longVector& rVec, long lCurrentValue)
{
    long lNextValue = *(rVec.begin());

    for (longVectorIt it = rVec.begin();
         it != rVec.end();
         ++it)
    {
        if ( (*it) > lCurrentValue )
        {
            lNextValue = (*it);
            break;
        }
    }

    return lNextValue;
}


/*static*/ bool WXCJob::HasThisValue (longVector& rVec, long lCurrentValue)
{
    for (longVectorIt it = rVec.begin();
         it != rVec.end();
         ++it)
    {
        if ( (*it) == lCurrentValue )
            return true;
    }

    return false;
}

wxDateTime WXCJob::CalculateNextTime (const wxDateTime& dtNow /*= wxDateTime::Now()*/)
{
    wxDateTime dtWakeUp(dtNow);
    //wxDateTime dtWakeUp(20, wxDateTime::May, 2008, 12, 54, 37);

    // second to 0
    dtWakeUp.SetSecond(0);
    dtWakeUp.SetMillisecond(0);

    // *** minute ***
    //wxMessageBox(dtWakeUp.Format());
    SetNextMinute(dtWakeUp);
    //wxMessageBox(dtWakeUp.Format());

    // *** hour ***
    //wxMessageBox(dtWakeUp.Format());
    if ( !(HasThisValue(hour_, dtWakeUp.GetHour())) )
        SetNextHour(dtWakeUp);
    //wxMessageBox(dtWakeUp.Format());

    // *** day || weekday ***
    //wxMessageBox(dtWakeUp.Format());
    while ( !(HasThisValue(day_, dtWakeUp.GetDay()))
      && !(HasThisValue(weekday_, dtWakeUp.GetWeekDay())) )
    {
        // increment day
        dtWakeUp.Add(wxDateSpan(0, 0, 0, 1));
    }
    //wxMessageBox(dtWakeUp.Format());

    // month
    //wxMessageBox(dtWakeUp.Format());
    if ( !(HasThisValue(month_, (dtWakeUp.GetMonth()+1))) )
        SetNextMonth(dtWakeUp);
    //wxMessageBox(dtWakeUp.Format());

    return dtWakeUp;
}

bool WXCJob::SetNextMinute (wxDateTime& dt)
{
    bool bIncremented = false;
    long lNextVal = GetNextValue(minute_, dt.GetMinute());

    // increment hour
    if (lNextVal <= dt.GetMinute())
    {
        dt.Add(wxTimeSpan(1, 0, 0, 0));
        bIncremented = true;
    }

    // set new minute
    dt.SetMinute(lNextVal);

    return bIncremented;
}

bool WXCJob::SetNextHour (wxDateTime& dt)
{
    bool bIncremented = false;
    long lNextVal = GetNextValue(hour_, dt.GetHour());

    // increment day?
    if (lNextVal < dt.GetHour())
    {
        dt.Add(wxDateSpan(0, 0, 0, 1));
        bIncremented = true;
    }

    // set new hour
    dt.SetHour(lNextVal);

    return bIncremented;
}

bool WXCJob::SetNextDay (wxDateTime& dt)
{
    bool bIncremented = false;
    long lNextVal = GetNextValue(day_, dt.GetDay());

    // end of month?
    if ( lNextVal > wxDateTime::GetNumberOfDays(dt.GetMonth(), dt.GetYear()) )
        lNextVal = GetNextValue(day_, lNextVal);

    // increment month?
    if (lNextVal < dt.GetDay())
    {
        dt.Add(wxDateSpan(0, 1, 0, 0));
        bIncremented = true;
    }

    // set new day
    dt.SetDay(lNextVal);

    return bIncremented;
}

bool WXCJob::SetNextMonth (wxDateTime& dt)
{
    bool bIncremented = false;
    long lNextVal = GetNextValue(month_, dt.GetMonth()+1);

    // increment year?
    if (lNextVal < (dt.GetMonth()+1) )
    {
        dt.Add(wxDateSpan(1, 0, 0, 0));
        bIncremented = true;
    }

    // set new month
    dt.SetMonth((wxDateTime::Month)(lNextVal-1));

    return bIncremented;
}

void WXCJob::Parse (const wxString& strOriginalLine)
{
    wxArrayString   arrStrings;
    wxString        str(strOriginalLine);
    wxString        strCurrent;
    size_t          idx;

    // extract the parameters
    while ( !(str.IsEmpty()) && arrStrings.GetCount() < 5 )
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

        // XXX        WXCLog::Do(wxString::Format("\"%s\" count: %d", strCurrent, arrStrings.GetCount()));
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
    strCommand_ = str.Trim(false);

    // XXX
    WXCLog::Do(wxString::Format("Line: %d => %s (%s)", lLine_, wxJoin(arrStrings, ';'), strCommand_), false);

    // XXX
    wxDateTime dt(20, wxDateTime::May, 2008, 12, 54, 37);

    // minutes
    Parse(arrStrings[0], minute_, 0, 59, dt.GetMinute());

    // hours
    Parse(arrStrings[1], hour_, 0, 23, dt.GetHour());

    // only use day of month; ignore day of week
    if ( arrStrings[4] == "*" )
    {
        Parse(arrStrings[2], day_, 1, 31, dt.GetDay());
    }
    // only use day of week; ignore day of month
    else
    {
        // day of month OR day of week
        if ( arrStrings[2] != "*" )
            Parse(arrStrings[2], day_, 1, 31, dt.GetDay());

        Parse(arrStrings[4], weekday_, 0, 7, dt.GetWeekDay());
    }

    // month
    Parse(arrStrings[3], month_, 1, 12, dt.GetMonth()+1);
}

void WXCJob::Parse (const wxString& str,
                    longVector& rVec,
                    int iRangeFrom,
                    int iRangeTo,
                    int iCurrValue)
{
    // XXX    WXCLog::Do(str);

    // just (*)
    if (str == '*')
    {
        FillValues(rVec, iRangeFrom, iRangeTo, 1);
        return;
    }

    // number-steps (*/n)
    if (str.Find('/') != wxNOT_FOUND)
    {
        long lStep;

        if ( str.AfterFirst('/').ToLong(&lStep) )
        {
            while (iCurrValue > iRangeFrom)
                iCurrValue = iCurrValue - lStep;

            iCurrValue = iCurrValue + lStep;

            FillValues(rVec, iRangeFrom + iCurrValue, iRangeTo, lStep);
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
    if (str.Find(',') != wxNOT_FOUND)
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
    if (str.Find('-') != wxNOT_FOUND)
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
