/**
 * Name:        WXCTaskBarIcon.cpp
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


#include "WXCTaskBarIcon.h"

#include <wx/menu.h>

#include "WXCApp.h"
#include "WXCConfigDlg.h"
#include "WXCAboutDlg.h"
#include "WXCLogViewDlg.h"
#include "WXCCrontabEditDlg.h"
#include "WXCCrontab.h"
#include "wxCron.h"

#define WXC_TASKBARICON_ID_CLOSE			1 + wxID_HIGHEST
#define WXC_TASKBARICON_ID_ABOUT			2 + wxID_HIGHEST
#define WXC_TASKBARICON_ID_SETTINGS			3 + wxID_HIGHEST
#define WXC_TASKBARICON_ID_LOG				4 + wxID_HIGHEST
#define WXC_TASKBARICON_ID_CHECKCRONTAB		5 + wxID_HIGHEST
#define WXC_TASKBARICON_ID_EDITCRONTAB		6 + wxID_HIGHEST

BEGIN_EVENT_TABLE(WXCTaskBarIcon, wxTaskBarIcon)
    EVT_MENU                (WXC_TASKBARICON_ID_CLOSE,			WXCTaskBarIcon::OnMenuClose)
    EVT_MENU                (WXC_TASKBARICON_ID_ABOUT,			WXCTaskBarIcon::OnMenuAbout)
    EVT_MENU                (WXC_TASKBARICON_ID_SETTINGS,		WXCTaskBarIcon::OnMenuSettings)
	EVT_MENU				(WXC_TASKBARICON_ID_LOG,			WXCTaskBarIcon::OnMenuShowLog)
	EVT_MENU				(WXC_TASKBARICON_ID_CHECKCRONTAB,	WXCTaskBarIcon::OnMenuCheckCrontab)
	EVT_MENU				(WXC_TASKBARICON_ID_EDITCRONTAB,	WXCTaskBarIcon::OnMenuEditCrontab)
    EVT_TASKBAR_RIGHT_DOWN  (									WXCTaskBarIcon::OnClick)
    EVT_TASKBAR_LEFT_DOWN   (									WXCTaskBarIcon::OnClick)
END_EVENT_TABLE()

WXCTaskBarIcon::WXCTaskBarIcon ()
{
    SetIcon
    (
        wxIcon("aaaaaaaa", wxBITMAP_TYPE_ICO_RESOURCE),
        WXCApp::GetFullApplicationName()
    );
}


/*virtual*/ WXCTaskBarIcon::~WXCTaskBarIcon ()
{
}


/*virtual*/ wxMenu* WXCTaskBarIcon::CreatePopupMenu ()
{
    wxMenu* pMenu = new wxMenu();

    pMenu->Append(WXC_TASKBARICON_ID_SETTINGS,		_("Settings"));
	pMenu->Append(WXC_TASKBARICON_ID_LOG,			_("Show Log"));
    pMenu->Append(WXC_TASKBARICON_ID_ABOUT,			_("About"));
	pMenu->AppendSeparator();
	pMenu->Append(WXC_TASKBARICON_ID_CHECKCRONTAB,	_("Check crontab"));
	pMenu->Append(WXC_TASKBARICON_ID_EDITCRONTAB,	_("Edit crontab"));
    pMenu->AppendSeparator();
    pMenu->Append(WXC_TASKBARICON_ID_CLOSE,			_("Close"));

    return pMenu;
}

void WXCTaskBarIcon::OnMenuClose (wxCommandEvent& rEvent)
{
    wxGetApp().DoClose();
}

void WXCTaskBarIcon::OnMenuAbout (wxCommandEvent& rEvent)
{
    new WXCAboutDlg();
}

void WXCTaskBarIcon::OnMenuSettings (wxCommandEvent& rEvent)
{
    new WXCConfigDlg();
}


void WXCTaskBarIcon::OnMenuShowLog (wxCommandEvent& rEvent)
{
	new WXCLogViewDlg();
}

void WXCTaskBarIcon::OnMenuCheckCrontab (wxCommandEvent& rEvent)
{
	WXCCrontab::CheckModification();
}


void WXCTaskBarIcon::OnMenuEditCrontab (wxCommandEvent& rEvent)
{
	new WXCCrontabEditDlg();
}

void WXCTaskBarIcon::OnClick(wxTaskBarIconEvent& rEvent)
{
    PopupMenu(CreatePopupMenu());
}
