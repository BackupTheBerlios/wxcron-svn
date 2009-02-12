/**
 * Name:        WXCAboutDlg.h
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


#ifndef WXCABOUTDLG_H
#define WXCABOUTDLG_H

#include <wx/dialog.h>

///
class WXCAboutDlg : wxDialog
{
    private:
        ///
        wxSizer* CreateThirdPartySizer (const wxString& strUrl,
                                        const wxString& strUrlLabel,
                                        const wxString& strAuthor,
                                        const wxString& strDesc);

    public:
        /// ctor
        WXCAboutDlg ();

        /// virtual dtor
        virtual ~WXCAboutDlg ();

        ///
        void OnViewLicense (wxCommandEvent& rEvent);
        ///
        void OnOk (wxCommandEvent& rEvent);
        ///
        void OnClose (wxCloseEvent& event);

    DECLARE_EVENT_TABLE();
};

#endif    // WXCABOUTDLG_H
