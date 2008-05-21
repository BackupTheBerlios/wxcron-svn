/**
 * Name:        WXCJob.h
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


#ifndef WXCJOB_H
#define WXCJOB_H

#include <vector>
#include <wx/string.h>
#include <wx/datetime.h>

// forewarde
class WXCTimer;

///
class WXCJob
{
    private:
        ///
        typedef std::vector<long>       longVector;
        ///
        typedef longVector::iterator    longVectorIt;

    private:
        /// to identify the crontab-line for the user on error messages
        long                    lLine_;
        ///
        WXCTimer*               pTimer_;
        /// XXX
        wxString                strOriginalLine_;

        /// minute
        longVector              minute_;
        /// hour
        longVector              hour_;
        /// day
        longVector              day_;
        /** month
            ATTENTION: Keep in mind, that crontab month can be values from 1 to 12
            but wxDateTime::Month store its month values from 0 to 11! */
        longVector              month_;
        /// day of week
        longVector              weekday_;

        /// the command to execute
        wxString                strCommand_;

        /// parse 'strOriginal_'
        void Parse (const wxString& strOriginalLine);

        /// parse the 'str' for crontab-like values and store them in a longVector
        void Parse (const wxString& str,
                    longVector& rVec,
                    int iRangeFrom,
                    int iRangeTo,
                    int iCurrValue);

        ///
        void FillValues (longVector& rVec,
                         int iRangeFrom,
                         int iRangeTo,
                         int iSteps);

        ///
        static long GetNextValue (longVector& rVec, long lCurrentValue);
        ///
        static bool HasThisValue (longVector& rVec, long lCurrentValue);

        /** Calculate based on the five time values-lists
            (minute_, hour_, day_, month_, weekday_) what is
            the next possible time and return it. */
        wxDateTime CalculateNextTime (const wxDateTime& dtNow = wxDateTime::Now());

        ///
        bool SetNextMinute (wxDateTime& dt);
        ///
        bool SetNextHour (wxDateTime& dt);
        ///
        bool SetNextDay (wxDateTime& dt);
        ///
        bool SetNextMonth (wxDateTime& dt);

    public:
        /// ctor
        WXCJob (const wxString& strOriginalLine, long lLine);

        /// virtual dtor
        virtual ~WXCJob ();

        ///
        void Start ();
};

typedef std::vector<WXCJob>         WXCJobVector;
typedef WXCJobVector::iterator      WXCJobVectorIt;

#endif    // WXCJOB_H
