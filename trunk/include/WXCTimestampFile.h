/**
 * Name:        WXCTimestampFile.h
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-28
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


#ifndef WXCTIMESTAMPFILE_H
#define WXCTIMESTAMPFILE_H

#include <vector>
#include <map>
#include <wx/string.h>
#include <wx/datetime.h>

///
class WXCTimestampFile
{
    private:
        ///
        typedef std::pair<wxDateTime, wxString>     PairTimeString;
        ///
        typedef std::vector<PairTimeString>         VecPairTimeString;

    private:
        ///
        static WXCTimestampFile     sFile_;

        ///
        VecPairTimeString           vecTimestamps_;

    public:
        /// ctor
        WXCTimestampFile ();

        /// virtual dtor
        virtual ~WXCTimestampFile ();

        ///
        static WXCTimestampFile& Instance ();

        ///
        long Count ();

        ///
        void Set(const wxString& strOriginalLine, const wxDateTime& dt = wxDateTime::Now());

        /** Return the last timestmap for the givin crontabl-line. If there
            is no timestamp it return wxInvalidDateTime. */
        wxDateTime GetLast (const wxString& strOriginalLine);

        ///
        bool Read ();
        ///
        bool Save ();
};

#endif    // WXCTIMESTAMPFILE_H
