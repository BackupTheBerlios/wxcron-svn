/**
 * Name:        WXCHyperlinkCtrl.h
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


#ifndef WXCHYPERLINKCTRL_H
#define WXCHYPERLINKCTRL_H

#include <wx/hyperlink.h>
#include <wx/thread.h>

///
class WXCHyperlinkCtrl : public wxHyperlinkCtrl
{
    public:
        /// ctor
        WXCHyperlinkCtrl (wxWindow* pParent,
                          wxWindowID id,
                          const wxString& strLabel,
                          const wxString& strUrl);

        /// virtual dtor
        virtual ~WXCHyperlinkCtrl ();

        ///
        void OnHyperlink (wxHyperlinkEvent& rEvent);

        DECLARE_EVENT_TABLE();
};

///
class WXCThread_LaunchBrowser : public wxThread
{
    private:
        ///
        wxString        strUrl_;

    public:
        ///
        WXCThread_LaunchBrowser (const wxString& url);
        ///
        virtual ~WXCThread_LaunchBrowser ();

        /// thread execution starts here
        virtual void *Entry();
};

#endif    // WXCHYPERLINKCTRL_H
