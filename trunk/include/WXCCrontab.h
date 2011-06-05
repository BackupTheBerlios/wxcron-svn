/**
 * Name:        WXCCrontab.h
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


#ifndef WXCCRONTAB_H
#define WXCCRONTAB_H

#include "WXCJob.h"

#include <wx/datetime.h>

///
class WXCCrontab
{
    private:
        ///
        static WXCCrontab       sCrontab_;

        /// modification time of last readed crontab file
        wxDateTime              timeMod_;

        /// the jobs
        WXCJobVector            vecJobs_;

        /// ctor
        WXCCrontab ();

    public:
        /// virtual dtor
        virtual ~WXCCrontab ();

        ///
        static WXCCrontab& Instance ()
        {
            return sCrontab_;
        }

        /** read and parse the crontab file
            it will create one if it doesn't exists */ 
        static void Read ();

        /// start all jobs
        void Start ();

        ///
        bool HasLine (const wxString& strLine);

        /** Check if the crontab file was modifide since the
            last read and read it again if needed.
            Return 'true' if there was a modification.
            Return 'false' if not. */
        static bool CheckModification ();

        /*void XXX (const wxString& str, short iFrom, short iTo);
        void YYY (const wxString& str);*/
};

#endif    // WXCCRONTAB_H
