/**
 * Name:        WXCApp.cpp
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


#include "WXCApp.h"

#include <wx/timer.h>
#include <wx/snglinst.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>

#include "WXCTaskBarIcon.h"
#include "WXCCrontab.h"
#include "WXCLog.h"
#include "WXCTimestampFile.h"
#include "wxCron.h"

#define WXC_APP_ID_TIMER_CRONTAB    1 + wxID_HIGHEST

IMPLEMENT_APP(WXCApp);

WXCApp::WXCApp ()
      : pTaskBarIcon_(NULL),
        pFSWatcher_(NULL)
{
}


/*virtual*/ WXCApp::~WXCApp ()
{
}

/*static*/ wxString WXCApp::GetFullApplicationName ()
{
    return wxString::Format(_T("%s %d.%d.%d %s"),
                               WXC_APP_NAME,
                               WXC_VERSION_MAJOR,
                               WXC_VERSION_MINOR,
                               WXC_VERSION_RELEASE,
                               WXC_VERSION_EXTENSION);
}

/*static*/ wxString WXCApp::GetVersion ()
{
    return wxString::Format("%d.%d.%d %s",
                            WXC_VERSION_MAJOR,
                            WXC_VERSION_MINOR,
                            WXC_VERSION_RELEASE,
                            WXC_VERSION_EXTENSION);
}

/*static*/ wxString WXCApp::GetUserConfigDir ()
{
	wxString str = wxStandardPaths::Get().GetUserConfigDir();

	if ( false == str.EndsWith(wxFILE_SEP_PATH) )
		str << wxFILE_SEP_PATH;

	str << WXC_APP_NAME;

	if ( false == wxDirExists(str) )
		wxMkDir(str);

	return str;
}

/*static*/ wxString WXCApp::GetCrontabFilename ()
{
	wxString str = GetUserConfigDir();

	str << wxFILE_SEP_PATH
		<< "crontab";

	return str;
}

/*static*/ wxString WXCApp::GetLogFilename ()
{
	wxString str = GetUserConfigDir();

	str << wxFILE_SEP_PATH
		<< "wxCron.log";

	return str;
}

/*static*/ wxString WXCApp::GetTimestampFilename ()
{
	wxString str = GetUserConfigDir();

	str << wxFILE_SEP_PATH
		<< "wxCron.tsmp";

	return str;
}

/*static*/ wxString WXCApp::GetConfigFilename ()
{
	wxString str = GetUserConfigDir();

	str << wxFILE_SEP_PATH
		<< "wxCron.conf";

	return str;
}


/*virtual*/ bool WXCApp::OnInit()
{
    // working dir
    ::wxSetWorkingDirectory( wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFILE_SEP_PATH) );

	// init log and config
	WXCLog::Instance().Init();
	WXCConfig::Instance().Init();

	/* we need to handle the log file size
	   after init of the config file */
	WXCLog::Instance().CareSize();

    // log start	
    WXCLog::Do(wxEmptyString, false);
    WXCLog::Do(wxString::Format("%s started...", GetFullApplicationName()));

    // check if another instance is running
    wxSingleInstanceChecker* pChecker = new wxSingleInstanceChecker(wxString::Format("%s-%s",
                                                                                     GetFullApplicationName(),
                                                                                     wxGetUserId()));
    if ( pChecker->IsAnotherRunning() )
    {
        wxString strErr(_("Another program instance is already running, aborting."));
        wxLogError(strErr);
        WXCLog::Do(strErr);
        return false;
    }

	// create systray icon
    pTaskBarIcon_ = new WXCTaskBarIcon();

    // read the crontab file
    // create a default one if there is no one
    WXCCrontab::Read();

    // read the timestamps
    WXCTimestampFile::Instance().Read();

    // start it !
    WXCCrontab::Instance().Start ();

    return true;
}

void WXCApp::OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop))
{
    if (pFSWatcher_)
        return;

    pFSWatcher_ = new wxFileSystemWatcher();

    pFSWatcher_->SetOwner(this);

    Connect
    (
        wxEVT_FSWATCHER,
        wxFileSystemWatcherEventHandler ( WXCApp::OnFileSystemEvent )
    );

    pFSWatcher_->Add
    (
        wxFileName ( GetCrontabFilename().BeforeLast(wxFILE_SEP_PATH) ),
        wxFSW_EVENT_MODIFY
    );
}

void WXCApp::OnFileSystemEvent(wxFileSystemWatcherEvent& rEvent)
{
    if ( rEvent.GetChangeType() == wxFSW_EVENT_MODIFY )
    {
        if ( WXCCrontab::CheckModification() )
        {
            pTaskBarIcon_->ShowBalloon
            (
                "wxCron",
                _("crontab file was modified and re-readed"),
                11000,
                wxICON_INFORMATION
            );
        }
    }
}

/*virtual*/ int WXCApp::OnExit()
{
    WXCLog::Do(wxString::Format("Close %s...\n", WXC_APP_NAME));

    if ( pFSWatcher_ )
        delete pFSWatcher_;

    if (pTaskBarIcon_)
    {
        pTaskBarIcon_->RemoveIcon();
        delete pTaskBarIcon_;
    }

    return wxApp::OnExit();
}

void WXCApp::DoClose ()
{
    wxGetApp().ExitMainLoop();
}
