/**
 * Name:        WXCApp.h
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-04
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


#ifndef WXCAPP_H
#define WXCAPP_H

// forward declarations
class WXCTaskBarIcon;

#include <wx/wx.h>
#include <wx/fswatcher.h>
#include "WXCConfig.h"

///
class WXCApp : public wxApp
{
    private:
        ///
        WXCTaskBarIcon*         pTaskBarIcon_;
        ///
        wxFileSystemWatcher*    pFSWatcher_;

		/** Return the application config dir for the current user.
			If it doesn't exists it will create it. */
		static wxString GetUserConfigDir ();

    public:
        /// ctor
        WXCApp ();
        /// virtual dtor
        virtual ~WXCApp ();

        ///
        static wxString GetFullApplicationName ();
        /// return major, minor, release number and extensions as string
        static wxString GetVersion ();

		// Return the crontab filename.
		static wxString GetCrontabFilename ();
		// Return the log filename.
		static wxString GetLogFilename ();
		// Return the timestamp filename.
		static wxString GetTimestampFilename ();
		// Return the config filename.
		static wxString GetConfigFilename ();

        /// close the application
        void DoClose ();

        /// start point like main()
        virtual bool OnInit();

        ///
        void OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop));

        ///
        void OnFileSystemEvent(wxFileSystemWatcherEvent& rEvent);

        ///
        virtual int OnExit();

        ///
        void OnTimer_CheckCrontab (wxTimerEvent& rEvent);

        //static long CountString (const wxString& strSource, const wxString& strToFind);
};

DECLARE_APP(WXCApp)

#endif    // WXCAPP_H
