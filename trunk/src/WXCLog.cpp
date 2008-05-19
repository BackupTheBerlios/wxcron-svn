/**
 * Name:        WXCLog.cpp
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-18
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


#include "WXCLog.h"

#include <wx/datetime.h>

#include "wxCron.h"

/*static*/ WXCLog WXCLog::sLog_;


WXCLog::WXCLog ()
      : lMaxSize_(WXC_LOG_MAXSIZE),
        fileLog_(WXC_LOG, wxFile::write_append)
{
    CareSize();
}


/*virtual*/ WXCLog::~WXCLog ()
{
}


long WXCLog::GetMaxSize ()
{
    return lMaxSize_;
}


void WXCLog::CareSize ()
{
    // the size does not matter
    if (GetMaxSize() <= 0)
        return;

    if ( !(fileLog_.IsOpened()) )
        return;

    // check log-file size
    if (fileLog_.Length() > GetMaxSize())
    {
        // close the file
        fileLog_.Close();
        // backup the file
        ::wxRenameFile(WXC_LOG, WXC_LOG_OLD, true);
        // create a new empty file
        fileLog_.Open(WXC_LOG, wxFile::write_append);
    }
}

/*static*/ void WXCLog::Do(const wxString& str)
{
    // result string written to the file
    wxString strLog;

    // create message with timestamp
    strLog << wxDateTime::Now().Format("%Y-%m-%d %H:%M:%S : ")
           << str
           << '\n';

    // write to the file
    Instance().fileLog_.Write(strLog);
}
