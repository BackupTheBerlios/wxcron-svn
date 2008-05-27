/**
 * Name:        WXCTime.cpp
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-21
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


#include "WXCTime.h"

WXCTime::WXCTime (const wxString& strCrontabLine)
{
    Parse(strCrontabLine);
}


/*virtual*/ WXCTime::~WXCTime ()
{
}

void WXCTime::Parse (const wxString& strCrontabLine)
{
    wxArrayString   arrStrings;
    wxString        str(strCrontabLine);
    wxString        strCurrent;
    size_t          idx;

    // extract the first five parameters
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
    }

    // enough parameters
    if (arrStrings.GetCount() < 5)
    {
        arrError_.Add(wxString::Format("Not enough parameters in this crontab line! \"%s\"", strCrontabLine));
        return;
    }

    // remember the unparsed rest
    strUnparsed_ = str;

    // minute
    minute_.Reset(arrStrings[0], 0, 59);

    // hour
    hour_.Reset(arrStrings[1], 0, 23);

    // day
    day_.Reset(arrStrings[2], 1, 31);

    // month
    month_.Reset(arrStrings[3], 1, 12);

    // day of week
    weekday_.Reset(arrStrings[4], 0, 7);
}

const wxArrayString& WXCTime::GetError ()
{
    return arrError_;
}

const wxString& WXCTime::GetUnparsed ()
{
    return strUnparsed_;
}

bool WXCTime::IsValid ()
{
    if ( minute_.IsValid()
      && hour_.IsValid()
      && day_.IsValid()
      && month_.IsValid()
      && weekday_.IsValid() )
      return true;

    return false;
}


wxDateTime WXCTime::GetNext (const wxDateTime& dtCurrent /*= wxDateTime::Now()*/)
{
    wxDateTime  dtNext(dtCurrent);
    short       iVal;
    short       iInitial, iInitialB;

    // seconds to 0
    dtNext.SetSecond(0);
    dtNext.SetMillisecond(0);


    // --- MINUTE ---
    iVal = minute_.GetNextValue(dtNext.GetMinute());

    // set minute
    dtNext.SetMinute(iVal);


    // --- HOUR ---
    iInitial = dtCurrent.GetHour();

    if ( dtNext <= dtCurrent || !(hour_.HasThisValue(dtNext.GetHour(), &iInitial)) )
    {
        iVal = hour_.GetNextValue(dtNext.GetHour());

        // set hour
        dtNext.SetHour(iVal);
    }

    // --- DAY OF MONTH / DAY OF WEEK ---
    /* For a better understanding here are the rules how "day of month" and
       "day of week" interact.
       If both fields are restricted (!= "*") only either field must match.
       If only one field is restricted the other field is ignored.
       Please read HELP or the crontab manpage to compare. */

    // set max-day depending on current month
    day_.SetValidRange(1, wxDateTime::GetNumberOfDays(dtNext.GetMonth(), dtNext.GetYear()));

    // day of month AND day of week are restricted (that means != "*")
    if (day_.IsRestricted() && weekday_.IsRestricted())
    {
        iInitialB = dtCurrent.GetDay();
        iInitial = dtCurrent.GetWeekDay();

        if ( dtNext <= dtCurrent )
        {
            short iValDay       = day_.GetNextValue(dtNext.GetDay());
            short iValWeekDay   = weekday_.GetNextValue(dtNext.GetWeekDay());

            wxDateTime dtNextDay(dtNext);
            wxDateTime dtNextWeekDay(dtNext);

            dtNextDay.SetDay(iValDay);

            // increment week
            if ( dtNextWeekDay.GetWeekDay() < iValWeekDay )
                dtNextWeekDay.Add(wxDateSpan(0, 0, 0, iValWeekDay-dtNextWeekDay.GetWeekDay()));
            else
                dtNextWeekDay.Add(wxDateSpan(0, 0, 0, 7+iValWeekDay-dtNextWeekDay.GetWeekDay()));

            // which do we use
            if (dtNextDay < dtNextWeekDay)
            {
                if (dtNextDay > dtNext)
                    dtNext = dtNextDay;
                else
                    dtNext = dtNextWeekDay;
            }
            else
            {
                if (dtNextWeekDay > dtNext)
                    dtNext = dtNextWeekDay;
                else
                    dtNext = dtNextDay;
            }
        }
    }
    else
    {
        // only the day of month restricted
        if ( day_.IsRestricted() )
        {
            if ( dtNext <= dtCurrent )
            {
                iVal = day_.GetNextValue(dtNext.GetDay());

                // set day
                dtNext.SetDay(iVal);
            }
        }

        // only the day of week restricted
        if ( weekday_.IsRestricted() )
        {
            if ( dtNext <= dtCurrent )
            {
                iVal = weekday_.GetNextValue(dtNext.GetWeekDay());

                // set day
                if (dtNext.GetWeekDay() < iVal)
                    dtNext.Add(wxDateSpan(0, 0, 0, iVal-dtNext.GetWeekDay()));
                else
                    dtNext.Add(wxDateSpan(0, 0, 0, 7+iVal-dtNext.GetWeekDay()));
            }
        }

        // nothing is restricted
        if ( !(day_.IsRestricted()) && !(weekday_.IsRestricted()) )
        {
            if ( dtNext <= dtCurrent )
            {
                iVal = day_.GetNextValue(dtNext.GetDay());

                // set day
                dtNext.SetDay(iVal);
            }
        }
    }

    // --- MONTH ---
    /* ATTENTION: Keep in mind, that crontab month can be values from 1 to 12
       but wxDateTime::Month store its month values from 0 to 11! */

    iInitial = dtCurrent.GetMonth() + 1;

    if ( dtNext <= dtCurrent )
    {
        iVal = month_.GetNextValue(dtNext.GetMonth()+1);

        // check if the next day is out of this months range
        if ( dtNext.GetDay() > wxDateTime::GetNumberOfDays((wxDateTime::Month)(iVal-1), dtNext.GetYear()) )
            dtNext.SetDay(1);

        // set month
        dtNext.SetMonth((wxDateTime::Month)(iVal-1));
    }


    // --- (YEAR) ---

    // increment year?
    if (dtNext <= dtCurrent)
        dtNext.Add(wxDateSpan(1, 0, 0, 0));

    return dtNext;
}
