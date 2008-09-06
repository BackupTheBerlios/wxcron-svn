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

#include <wx/string.h>
#include <wx/datetime.h>
#include "WXCTime.h"

// foreward
class WXCTimer;

///
class WXCJob
{
    private:
        /// to identify the crontab-line for the user on error messages
        long                    lLine_;
        ///
        WXCTimer*               pTimer_;
        ///
        wxString                strOriginalLine_;

        ///
        WXCTime                 time_;

        /// the command to execute
        wxString                strCommand_;

        /// option "@nocatchup"
        bool                    bOption_nocatchup_;

        ///
        void SetOptions(const wxArrayString& arrOptions);

    public:
        /// ctor
        WXCJob (const wxString& strOriginalLine,
                long lLine,
                const wxArrayString& arrOptions);

        /// virtual dtor
        virtual ~WXCJob ();

        ///
        void Start ();

        ///
        void Execute ();

        ///
        const wxString& GetOriginalLine ();
        ///
        long GetLine ();
};

typedef std::vector<WXCJob>         WXCJobVector;
typedef WXCJobVector::iterator      WXCJobVectorIt;

#endif    // WXCJOB_H
