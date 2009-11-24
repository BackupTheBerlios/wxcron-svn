/**
 * Name:        WXCTaskBarIcon.h
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


#ifndef WXCTASKBARICON_H
#define WXCTASKBARICON_H

#include <wx/taskbar.h>

///
class WXCTaskBarIcon : public wxTaskBarIcon
{
    private:
        /// private members

    protected:
        /// proteced members

    public:
        /// ctor
        WXCTaskBarIcon ();

        /// virtual dtor
        virtual ~WXCTaskBarIcon ();

        ///
        virtual wxMenu* CreatePopupMenu ();

        ///
        void OnClick(wxTaskBarIconEvent& rEvent);

        ///
        void OnMenuClose (wxCommandEvent& rEvent);
        ///
        void OnMenuAbout (wxCommandEvent& rEvent);
        ///
        void OnMenuSettings (wxCommandEvent& rEvent);
		///
		void OnMenuShowLog (wxCommandEvent& rEvent);
		///
		void OnMenuCheckCrontab (wxCommandEvent& rEvent);
		///
		void OnMenuEditCrontab (wxCommandEvent& rEvent);

        DECLARE_EVENT_TABLE();
};

#endif    // WXCTASKBARICON_H
