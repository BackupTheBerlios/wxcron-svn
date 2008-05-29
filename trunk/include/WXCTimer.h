/**
 * Name:        WXCTimer.h
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-19
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


#ifndef WXCTIMER_H
#define WXCTIMER_H

#include <wx/timer.h>
#include <wx/datetime.h>

// forewarde declarations
class WXCJob;

///
class WXCTimer : public wxTimer
{
    private:
        ///
        WXCJob*             pParentJob_;

        /** In normal cases this value is not used. It should be wxInvalidDateTime then.
            But if the intervall between wxDateTime::Now() and 'timeNextWakeUp_' is
            in milliseconds higher than a signed integer can store (INT_MAX) it is used.
            Please see Start() for more detailes. */
        wxDateTime          timeNextWakeUp_;

    public:
        /// ctor
        WXCTimer (WXCJob* pParentJob);

        /// virtual dtor
        virtual ~WXCTimer ();

        ///
        bool Start (const wxDateTime& timeNextWakeUp);

        ///
        void Notify ();
};

#endif    // WXCTIMER_H
