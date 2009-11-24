/**
 * Name:        WXCCrontabEditDlg.h
 * Purpose:     WXCCrontabEditDlg class defination
 *				based on BFLogViewDlg (blackfisk)
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2007-07-21
 * Copyright:   (c) 2007 Christian Buhtz <blackfisk@web.de>
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

#ifndef WXCCRONTABEDITDLG_H
#define WXCCRONTABEDITDLG_H

#include <wx/dialog.h>

// forwarde declarations
class wxTextCtrl;

///
class WXCCrontabEditDlg : public wxDialog
{
    private:
		///
		wxTextCtrl*			pCtrl_;

        ///
        void Init ();

		///
		void ReadCrontab (wxString& strCrontabContent, unsigned int& iLineLength, unsigned int& iLineCount);
		///
		void SaveCrontab (wxString& strCrontabContent);

    public:
        /// constructor
        WXCCrontabEditDlg ();

        /// virtual destructor
        virtual ~WXCCrontabEditDlg ();

        ///
        void OnClose (wxCloseEvent& event);

    DECLARE_EVENT_TABLE();
};

#endif
