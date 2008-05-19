/**
 * Name:        WXCLog.h
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-18
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


#ifndef WXCLOG_H
#define WXCLOG_H

#include <wx/file.h>

///
class WXCLog
{
    private:
        ///
        static WXCLog           sLog_;

        /** max size of a file in bytes
            it is handled in CareSize()
            set it to '0' if the size does not matter */
        long                    lMaxSize_;

        /// direct interface to the log file
        wxFile                  fileLog_;

        /** take care that size of the logfile is not to big */
        void CareSize ();

        /// ctor
        WXCLog ();

    public:
        /// virtual dtor
        virtual ~WXCLog ();

        /// return the maximum size in bytes
        long GetMaxSize ();

        ///
        static WXCLog& Instance()
        {
            return sLog_;
        }

        /// log a message
        static void Do(const wxString& str);
};

#endif    // WXCLOG_H
