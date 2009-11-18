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

#include "WXCConfig.h"
#include "WXCApp.h"
#include "wxCron.h"

/*static*/ WXCLog WXCLog::sLog_;


WXCLog::WXCLog ()
      : lMaxSize_(-1)	// -1 means it is not initializied
{
}


/*virtual*/ WXCLog::~WXCLog ()
{
}

void WXCLog::Init ()
{
	if ( lMaxSize_ != -1 )
	{
		wxLogFatalError
		(
			"Try to initialize a still initilized log object.\n" \
			"in WXCLog::Init()\n" \
			"Please contact the developer!"
		);
		return;
	}
		
	// max log file size
	// not able to set because the config file wasn't read at this point

	// open the physical log file
	fileLog_.Open(WXCApp::GetLogFilename(), wxFile::write_append);
}

long WXCLog::GetMaxSize ()
{
    return lMaxSize_;
}


void WXCLog::CareSize ()
{
	// set max log file size
	lMaxSize_ = WXCConfig::Instance().GetMaxLogFileSizeInKB() * 1024;

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
        ::wxRenameFile(WXCApp::GetLogFilename(), WXCApp::GetLogFilename() + ".old", true);
        // create a new empty file
        fileLog_.Open(WXCApp::GetLogFilename(), wxFile::write_append);
    }
}

/*static*/ void WXCLog::Do(const wxString& str, bool bWithTimestamp /*= true*/)
{
    // result string written to the file
    wxString strLog;

    if (bWithTimestamp)
    {
        // create message with timestamp
        strLog << wxDateTime::Now().Format("%Y-%m-%d %H:%M:%S : ")
               << str
               << '\n';
    }
    else
    {
        // create message without timestamp
        strLog << str << '\n';
    }

    // write to the file
    Instance().fileLog_.Write(strLog);

    // remember
    Instance().strLastMessage_ = strLog;
}

/*static*/ const wxString& WXCLog::GetLastMessage ()
{
    return Instance().strLastMessage_;
}
