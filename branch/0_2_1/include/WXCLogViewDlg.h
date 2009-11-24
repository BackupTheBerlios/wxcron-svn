/**
 * Name:        WXCLogViewDlg.h
 * Purpose:     WXCLogViewDlg class defination
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

#ifndef WXCLOGVIEWDLG_H
#define WXCLOGVIEWDLG_H

#include <wx/dialog.h>

///
class WXCLogViewDlg : public wxDialog
{
    private:
        ///
        void Init ();

    public:
        /// constructor
        WXCLogViewDlg ();

        /// virtual destructor
        virtual ~WXCLogViewDlg ();

        ///
        void OnClose (wxCloseEvent& event);

    DECLARE_EVENT_TABLE();
};

#endif    // WXCLOGVIEWDLG_H
