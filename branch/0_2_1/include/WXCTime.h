/**
 * Name:        WXCTime.h
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


#ifndef WXCTIME_H
#define WXCTIME_H

#include "WXCTimeValue.h"

#include <wx/datetime.h>

/**
 A couple of possible date values depending on a parsed crontab
 line.
 */
class WXCTime
{
    private:
        /// minute
        WXCTimeValue    minute_;

        /// hour
        WXCTimeValue    hour_;

        /// day of month
        WXCTimeValue    day_;

        /** month
            ATTENTION: Keep in mind, that crontab month can be values from 1 to 12
            but wxDateTime::Month store its month values from 0 to 11! */
        WXCTimeValue    month_;

        /// day of week
        WXCTimeValue    weekday_;

        /** Errors while parsing are stored here and can be
            checked with IsValid() and read with GetError(). */
        wxArrayString   arrError_;

        /** The rest/unparsed part of the crontabline.
            Normaly it is the command to execute. */
        wxString        strUnparsed_;

        ///
        void Parse (const wxString& strCrontabLine);

    public:
        ///
        WXCTime (const wxString& strCrontabLine);

        /// virtual dtor
        virtual ~WXCTime ();

        ///
        const wxArrayString& GetError ();
        ///
        const wxString& GetUnparsed ();
        ///
        bool IsValid ();

        ///
        wxDateTime GetNext (const wxDateTime& dtCurrent = wxDateTime::Now());
};

#endif    // WXCTIME_H
