/**
 * Name:        WXCTimeValue.cpp
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


#include "WXCTimeValue.h"
#include <wx/tokenzr.h>

WXCTimeValue::WXCTimeValue (const wxString& strCrontabParameter,
                            short iRangeValidFrom,
                            short iRangeValidTo)
            : pairValidRange_(-1, -1),
              iStepFree_(-1),
              pairpairStepsInRanges_(PairShort(-1, -1), -1)
{
    Reset (strCrontabParameter, iRangeValidFrom, iRangeValidTo);
}

WXCTimeValue::WXCTimeValue ()
            : pairValidRange_(-1, -1),
              iStepFree_(-1),
              pairpairStepsInRanges_(PairShort(-1, -1), -1)
{
}

void WXCTimeValue::Reset (const wxString& strCrontabParameter,
                          short iRangeValidFrom,
                          short iRangeValidTo)
{
    Clear();
    pairValidRange_.first = iRangeValidFrom;
    pairValidRange_.second = iRangeValidTo;
    Parse(strCrontabParameter);
}

void WXCTimeValue::SetValidRange (short iFrom, short iTo)
{
    pairValidRange_ = PairShort(iFrom, iTo);
}

void WXCTimeValue::Clear ()
{
    arrError_.Clear();
    pairValidRange_.first = -1;
    pairValidRange_.second = -1;
    vecSingle_.clear();
    vecpairRange_.clear();
    iStepFree_ = -1;
    pairpairStepsInRanges_.first = PairShort(-1, -1);
    pairpairStepsInRanges_.second = -1;
}

void WXCTimeValue::Parse (const wxString& strCrontabParameter)
{
    long lOne, lTwo, lThree;
    wxString strOne;

    // --- unrestricted (*) ---
    if (strCrontabParameter == '*')
    {
        // nothing to be done
        return;
    }

    // --- steps (*/s) (n-m/s) ---
    if (strCrontabParameter.Find('/') != wxNOT_FOUND)
    {
        if ( strCrontabParameter.AfterFirst('/').ToLong(&lOne) )
        {
            strOne = strCrontabParameter.BeforeFirst('/');

            // free steps
            if ( strOne == "*" )
            {
                iStepFree_ = (short)lOne;
            }
            // steps in a range
            else
            {
                if ( strOne.BeforeFirst('-').ToLong(&lTwo)
                  && strOne.AfterFirst('-').ToLong(&lThree) )
                {
                    if ( IsInRange(PairShort((short)lTwo, (short)lThree)) )
                    {
                        pairpairStepsInRanges_.first.first  = (short)lTwo;      // range from
                        pairpairStepsInRanges_.first.second = (short)lThree;    // range to
                        pairpairStepsInRanges_.second       = (short)lOne;      // steps in the range
                    }
                    else
                    {
                        arrError_.Add
                        (
                            wxString::Format("The step-in-range parameter (%s) couldn't be parsed. Out of valid range (%d - %d)!",
                                             strCrontabParameter,
                                             pairValidRange_.first,
                                             pairValidRange_.second)
                        );
                        return;
                    }
                }
                else
                {
                    arrError_.Add
                    (
                        wxString::Format("The step-in-range parameter (%s) couldn't be parsed.",
                                         strCrontabParameter)
                    );
                    return;
                }
            }
        }
        else
        {
            arrError_.Add
            (
                wxString::Format("The step parameter (%s) couldn't be parsed.",
                                 strCrontabParameter)
            );
            return;
        }

        return;
    }

    // --- value-list (m, n) AND range-list (m-n,o-p,x) ---
    if (strCrontabParameter.Find(',') != wxNOT_FOUND)
    {
        wxStringTokenizer tkn(strCrontabParameter, ',');

        while ( tkn.HasMoreTokens() )
        {
            strOne = tkn.GetNextToken();

            // a range
            if (strOne.Find('-') != wxNOT_FOUND)
            {
                if ( strOne.BeforeFirst('-').ToLong(&lOne)
                  && strOne.AfterLast('-').ToLong(&lTwo) )
                {
                    if ( IsInRange(PairShort((short)lOne, (short)lTwo)) )
                    {
                        vecpairRange_.push_back(PairShort((short)lOne, (short)lTwo));
                    }
                    else
                    {
                        arrError_.Add
                        (
                            wxString::Format("The range-list parameter (%s) couldn't be parsed. Token (%s) out of valid range (%d - %d)!",
                                             strCrontabParameter,
                                             strOne,
                                             pairValidRange_.first,
                                             pairValidRange_.second)
                        );
                        return;
                    }
                }
                else
                {
                    arrError_.Add
                    (
                        wxString::Format("The range-list parameter (%s) couldn't be parsed. Can not convert token (%s) to long!",
                                         strCrontabParameter,
                                         strOne)
                    );
                    return;
                }
            }
            // just a value
            else
            {
                if ( strOne.ToLong(&lOne) )
                {
                    if ( IsInRange((short)lOne) )
                    {
                        vecSingle_.push_back((short)lOne);
                    }
                    else
                    {
                        arrError_.Add
                        (
                            wxString::Format("The value-list parameter (%s) couldn't be parsed. Token (%s) out of valid range (%d - %d)!",
                                             strCrontabParameter,
                                             strOne,
                                             pairValidRange_.first,
                                             pairValidRange_.second)
                        );
                        return;
                    }
                }
                else
                {
                    arrError_.Add
                    (
                        wxString::Format("The value-list parameter (%s) couldn't be parsed. Can not convert token (%s) to long!",
                                         strCrontabParameter,
                                         strOne)
                    );
                    return;
                }
            }
        }

        return;
    }

    // --- range (n-m) ---
    if (strCrontabParameter.Find('-') != wxNOT_FOUND)
    {
        if ( strCrontabParameter.BeforeFirst('-').ToLong(&lOne)
          && strCrontabParameter.AfterLast('-').ToLong(&lTwo) )
        {
            if ( IsInRange(PairShort((short)lOne, (short)lTwo)) )
            {
                vecpairRange_.push_back(PairShort((short)lOne, (short)lTwo));
            }
            else
            {
                arrError_.Add
                (
                    wxString::Format("The range parameter (%s) couldn't be parsed. Out of valid range (%d - %d)!",
                                     strCrontabParameter,
                                     pairValidRange_.first,
                                     pairValidRange_.second)
                );
                return;
            }
        }
        else
        {
            arrError_.Add
            (
                wxString::Format("The range parameter (%s) couldn't be parsed. Can not convert to long!",
                                 strCrontabParameter)
            );
            return;
        }

        return;
    }

    // --- number (n) ---
    if ( strCrontabParameter.ToLong(&lOne) )
    {
        if ( IsInRange((short)lOne) )
        {
            vecSingle_.push_back((short)lOne);
        }
        else
        {
            arrError_.Add
            (
                wxString::Format("The number parameter (%s) couldn't be parsed. Out of valid range (%d - %d)!",
                                 strCrontabParameter,
                                 pairValidRange_.first,
                                 pairValidRange_.second)
            );
            return;
        }

        return;
    }
    else
    {
        arrError_.Add
        (
            wxString::Format("The number parameter (%s) couldn't be parsed. Can not convert to long!",
                             strCrontabParameter)
        );
        return;
    }
}

//
/*virtual*/ WXCTimeValue::~WXCTimeValue ()
{
}


const wxArrayString& WXCTimeValue::GetError ()
{
    return arrError_;
}

bool WXCTimeValue::IsValid ()
{
    return arrError_.IsEmpty();
}

bool WXCTimeValue::IsRestricted ()
{
    if ( vecSingle_.empty()
      && vecpairRange_.empty()
      && iStepFree_ == -1
      && pairpairStepsInRanges_.second == -1 )
    {
        return false;
    }

    return true;
}

bool WXCTimeValue::IsInRange (const short iValue)
{
    return IsInRange(iValue, pairValidRange_);
}

bool WXCTimeValue::IsInRange (PairShort pairRange)
{
    if ( IsInRange(pairRange.first)
      && IsInRange(pairRange.second) )
      return true;

    return false;
}

/*static*/ bool WXCTimeValue::IsInRange (const short iValue, PairShort pairRange)
{
    if ( iValue >= pairRange.first
      && iValue <= pairRange.second )
      return true;

    return false;
}

/*static*/ bool WXCTimeValue::IsInRange (const short iValue, VectorPairShort vecpairRanges)
{
    for ( VectorPairShort::iterator it = vecpairRanges.begin();
          it != vecpairRanges.end();
          ++it)
    {
        if ( IsInRange(iValue, (*it)) )
            return true;
    }

    return false;
}

bool WXCTimeValue::HasThisValue (const short iValue)
{
    // --- unrestricted ---
    if ( !(IsRestricted()) && IsInRange(iValue) )
        return true;

    // --- free steps ---
    if ( iStepFree_ != -1 )
    {
        // free steps (that means without a restricted(!) range
        // match everytime if you count enough on them
        return true;
    }

    // --- single values ---
    if ( !(vecSingle_.empty()) )
    {
        for (VectorShort::iterator it = vecSingle_.begin();
             it != vecSingle_.end();
             ++it)
        {
            if ( (*it) == iValue )
                return true;
        }
    }

    // --- ranges ---
    if ( !(vecpairRange_.empty()) )
    {
        for (VectorPairShort::iterator it = vecpairRange_.begin();
             it != vecpairRange_.end();
             ++it)
        {
            if ( IsInRange(iValue, (*it)) )
                return true;
        }
    }

    // --- steps in ranges ---
    if ( pairpairStepsInRanges_.second != -1 )
    {
        if ( IsInRange(iValue, pairpairStepsInRanges_.first) )
        {
            if ( ( (iValue-pairpairStepsInRanges_.first.first) % pairpairStepsInRanges_.second ) == 0 )
                return true;
        }
    }

    return false;
}

short WXCTimeValue::GetNextValue (const short iCurrent)
{
    // in range
    if ( !(IsInRange(iCurrent)) )
        return iCurrent;

    //
    short iNext = iCurrent;

    // --- unrestricted ---
    if ( !(IsRestricted()) )
    {
        if (iCurrent == pairValidRange_.second)
            iNext = pairValidRange_.first;
        else
            iNext = iCurrent + 1;

        return iNext;
    }

    // --- free steps ---
    if ( iStepFree_ != -1 )
    {
        if ( iCurrent > (pairValidRange_.second-iStepFree_) )
            iNext = pairValidRange_.first + (iStepFree_ - (pairValidRange_.second - iCurrent));
        else
            iNext = iCurrent + iStepFree_;

        return iNext;
    }

    // --- single values ---
    if ( !(vecSingle_.empty()) )
    {
        for (VectorShort::iterator it = vecSingle_.begin();
             it != vecSingle_.end();
             ++it)
        {
            if ( (*it) > iCurrent )
                return (*it);
        }

        return (*(vecSingle_.begin()));
    }

    // --- ranges ---
    if ( !(vecpairRange_.empty()) )
    {
        for (VectorPairShort::iterator it = vecpairRange_.begin();
             it != vecpairRange_.end();
             ++it)
        {
            // use the next range in the list
            if ( iNext < (*it).first )
            {
                iNext = (*it).first;
                return iNext;
            }
            else
            {
                // step to beginning of current range
                // and wait if there is a higher range
                if ( iNext == (*it).second )
                {
                    iNext = iCurrent + 1;
                }
                // just increment
                else
                {
                    iNext = iCurrent + 1;

                    if ( IsInRange(iNext, vecpairRange_) )
                        return iNext;
                }
            }
        }

        if (iNext > (*(vecpairRange_.begin())).second)
            return (*(vecpairRange_.begin())).first;
    }

    // --- steps in ranges ---
    if ( pairpairStepsInRanges_.second != -1 )
    {
        short iStep = pairpairStepsInRanges_.second;
        short iFrom = pairpairStepsInRanges_.first.first;
        short iTo = pairpairStepsInRanges_.first.second;

        // over range
        if ( iCurrent > (iTo - iStep) )
        {
            if (iCurrent > iTo)
                iNext = iFrom;
            else
                iNext = iTo;
        }
        else
        {
            // under range
            if ( iCurrent < (iFrom) )
            {
                iNext = iFrom;
            }
            // inside range
            else
            {
                // only match the steps!
                iNext = iCurrent - ( (iCurrent-iFrom) % iStep ) + iStep;
            }
        }

        return iNext;
    }

    return iNext;
}

