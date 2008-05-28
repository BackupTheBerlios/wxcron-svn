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

#include "WXCTaskBarIcon.h"
#include "WXCCrontab.h"
#include "WXCLog.h"
#include "wxCron.h"

#define WXC_APP_ID_TIMER_CRONTAB    1 + wxID_HIGHEST

IMPLEMENT_APP(WXCApp);

WXCApp::WXCApp ()
      : pTaskBarIcon_(NULL)
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

/*virtual*/ bool WXCApp::OnInit()
{
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

    // read the crontab file
    if ( !(WXCCrontab::Read()) )
        return false;

    // start it !
    WXCCrontab::Instance().Start ();

    // check crontab for modifications periodicly?
    if (WXCConfig::Instance().GetCheckCrontabIntervallInMinutes() > 0)
    {
        wxTimer* pTimer = new wxTimer(this, WXC_APP_ID_TIMER_CRONTAB);

        Connect
        (
            WXC_APP_ID_TIMER_CRONTAB,
            wxEVT_TIMER,
            wxTimerEventHandler(WXCApp::OnTimer_CheckCrontab),
            NULL,
            this
        );

        pTimer->Start(1000 * 60 * WXCConfig::Instance().GetCheckCrontabIntervallInMinutes(),
                      wxTIMER_CONTINUOUS);
    }

    // create systray icon
    pTaskBarIcon_ = new WXCTaskBarIcon();

    return true;
}


/*virtual*/ int WXCApp::OnExit()
{
    WXCLog::Do(wxString::Format("Close %s...\n", WXC_APP_NAME));

    if (pTaskBarIcon_)
        pTaskBarIcon_->RemoveIcon();

    return 1;
}

void WXCApp::DoClose ()
{
    wxGetApp().ExitMainLoop();
}

void WXCApp::OnTimer_CheckCrontab (wxTimerEvent& rEvent)
{
    WXCCrontab::CheckModification();
}

/*static* long WXCApp::CountString (const wxString& strSource, const wxString& strToFind)
{
    long lCount     = -1;
    long idx        = 0;
    long idxCurr    = -(strToFind.Length());

    while (idx != wxNOT_FOUND )
    {
        ++lCount;

        idxCurr = idxCurr + idx + strToFind.Length();

        idx = strSource.Mid(idxCurr).Find(strToFind);
    }

    return lCount;
}*/
