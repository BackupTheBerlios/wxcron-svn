/**
 * Name:        WXCConfig.cpp
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


#include "WXCConfig.h"

#include <wx/fileconf.h>
#include <wx/wfstream.h>

#include "WXCLog.h"
#include "wxCron.h"
#include "WXCApp.h"

/*static*/ WXCConfig WXCConfig::sConfig_;

WXCConfig::WXCConfig ()
         : lMaxSize_(1024)
{
}

void WXCConfig::Init ()
{
    // check if config file exists
	if ( !(wxFile::Exists(WXCApp::GetConfigFilename())) )
    {
        wxFile file(WXCApp::GetConfigFilename(), wxFile::write);
        WXCLog::Do(wxString::Format("Log-file %s doesn't exists! Create a default one...", WXCApp::GetConfigFilename()));

        file.Write
        (
            wxString::Format
            (
                "# Maximum size of the log-file (%s) in KiloByte\n" \
                "log-size=%d\n",
                WXCApp::GetConfigFilename(),
                GetMaxLogFileSizeInKB()
            )
        );
    }

    //
    ReadConfig();
}

/*virtual*/ WXCConfig::~WXCConfig ()
{
}

void WXCConfig::ReadConfig ()
{
    wxFileInputStream   is(WXCApp::GetConfigFilename());
    wxFileConfig        fileConfig(is);
    long                lVal;

    // maximum log-file size
    if ( fileConfig.Read("log-size", &lVal) )
        lMaxSize_ = lVal;
}

bool WXCConfig::SaveConfig ()
{
    wxFileInputStream   is(WXCApp::GetConfigFilename());
    wxFileConfig        fileConfig(is);

    // maximum log-file size
    fileConfig.Write("log-size", lMaxSize_);

    // save
    wxFileOutputStream os(WXCApp::GetConfigFilename());

    if ( !(fileConfig.Save(os)) )
    {
        WXCLog::Do(wxString::Format("Error while saving the config file %s.", WXCApp::GetConfigFilename()));
        return false;
    }

    return true;
}

long WXCConfig::GetMaxLogFileSizeInKB ()
{
    return lMaxSize_;
}

void WXCConfig::SetMaxLogFileSizeInKB (long lSize)
{
    lMaxSize_ = lSize;
}
