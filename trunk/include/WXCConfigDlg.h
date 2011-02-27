/**
 * Name:        WXCConfigDlg.h
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


#ifndef WXCCONFIGDLG_H
#define WXCCONFIGDLG_H

#include <wx/dialog.h>

///
class WXCConfigDlg : public wxDialog
{
    private:
        ///
        wxSpinCtrl*         pSpinMaxLog_;

        ///
        void ReadDataFromConfig ();
        ///
        void SaveDataToConfig ();

    public:
        /// ctor
        WXCConfigDlg ();

        /// virtual dtor
        virtual ~WXCConfigDlg ();

        ///
        void OnClose (wxCloseEvent& event);
        ///
        void OnButton_Ok (wxCommandEvent& rEvent);
        ///
        void OnButton_Cancel (wxCommandEvent& rEvent);

        DECLARE_EVENT_TABLE();
};

#endif    // WXCCONFIGDLG_H
