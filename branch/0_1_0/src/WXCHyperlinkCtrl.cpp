/**
 * Name:        WXCHyperlinkCtrl.cpp
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-28
 * Copyright:   (c) 2007 Christian Buhtz <exsudat@gmx.de>
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


#include "WXCHyperlinkCtrl.h"

BEGIN_EVENT_TABLE(WXCHyperlinkCtrl, wxHyperlinkCtrl)
  EVT_HYPERLINK ( 1, WXCHyperlinkCtrl::OnHyperlink )
END_EVENT_TABLE()

//
WXCHyperlinkCtrl::WXCHyperlinkCtrl (wxWindow* pParent,
                                    wxWindowID id,
                                    const wxString& strLabel,
                                    const wxString& strUrl)
                : wxHyperlinkCtrl (pParent, 1, strLabel, strUrl)
{
}


//
/*virtual*/ WXCHyperlinkCtrl::~WXCHyperlinkCtrl ()
{
}

void WXCHyperlinkCtrl::OnHyperlink (wxHyperlinkEvent& rEvent)
{
    new WXCThread_LaunchBrowser(GetURL());
}

WXCThread_LaunchBrowser::WXCThread_LaunchBrowser (const wxString& url)
                       : strUrl_(url)
{
    Create();
    Run();
}

/*virtual*/ WXCThread_LaunchBrowser::~WXCThread_LaunchBrowser ()
{
}

/*virtual*/ void* WXCThread_LaunchBrowser::Entry()
{
    wxLaunchDefaultBrowser(strUrl_, wxBROWSER_NEW_WINDOW);
    return NULL;
}
