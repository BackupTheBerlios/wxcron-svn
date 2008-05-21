/**
 * Name:        WXCTimeValue.h
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


#ifndef WXCTIMEVALUE_H
#define WXCTIMEVALUE_H

#include <vector>
#include <wx/string.h>
#include <wx/arrstr.h>

/**
 A couple of possible values depending on a parsed crontab
 parameter.
 For example:
    "*"
    "* /5"
    "2,5,10"
    "5-23"
    "2-6,10-14"
    "5-20/3"
 */
class WXCTimeValue
{
    private:
        ///
        typedef std::pair<short, short>             PairShort;
        ///
        typedef std::vector<short>                  VectorShort;
        ///
        typedef std::vector<PairShort>              VectorPairShort;
        ///
        typedef std::pair<PairShort, short>         PairPairShort;

        /** Errors while parsing are stored here and can be
            checked with IsValid() and read with GetError(). */
        wxArrayString       arrError_;

        /// The range in where the values are valid.
        PairShort           pairValidRange_;

        /// Single values (e.g. "4,8,10,20").
        VectorShort         vecSingle_;

        /// Ranges (e.g. "4-9,14-20")
        VectorPairShort     vecpairRange_;

        /// Step without ranges (e.g. "*/5" or "*/2")
        short               iStepFree_;

        /// Steps in ranges (e.g. "5-20/3")
        PairPairShort       pairpairStepsInRanges_;

        ///
        void Parse (const wxString& strCrontabParameter);

        ///
        void Clear ();

    public:
        ///
        WXCTimeValue (const wxString& strCrontabParameter,
                      short iRangeValidFrom,
                      short iRangeValidTo);

        ///
        WXCTimeValue ();

        /// virtual dtor
        virtual ~WXCTimeValue ();

        ///
        void Reset (const wxString& strCrontabParameter,
                    short iRangeValidFrom,
                    short iRangeValidTo);

        /** Set a new range ('pairValidRange_') for valid values.
            Be carefull with use of this methode.
            Normaly it is used to modify the max-days (30 or 31)
            depending on a month. */
        void SetValidRange (short iFrom, short iTo);

        ///
        const wxArrayString& GetError ();
        ///
        bool IsValid ();

        /** If the parameter was "*" nothing is restricted. In this
            case the methode will return false. Otherwise it return
            true. */
        bool IsRestricted ();

        /// Check if the value 'iValue' is in the range specified by 'pairValidRange_'.
        bool IsInRange (const short iValue);
        /// Check if the range 'pairRange' is in the range specified by 'pairValidRange_'.
        bool IsInRange (PairShort pairRange);
        /// Check if the value 'iValue' is in the range specified by 'pairRange'.
        static bool IsInRange (const short iValue, PairShort pairRange);
        /// Check if the value 'iValue' is in some of the ranges specified by 'vecpairRanges'
        static bool IsInRange (const short iValue, VectorPairShort vecpairRanges);

        ///
        bool HasThisValue (const short iValue);
        /** Calculate the next possible value from 'iCurrent' depending on the crontab parameter.
            On errors it return 'iCurrent'. */
        short GetNextValue (const short iCurrent);
};

#endif    // WXCTIMEVALUE_H
