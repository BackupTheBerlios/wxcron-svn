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

/*static*/ WXCConfig WXCConfig::sConfig_;

WXCConfig::WXCConfig ()
         : lMaxSize_(1024),
           lCheckCrontab_(0)
{
    // check if config file exists
    if ( !(wxFile::Exists(WXC_CONFIG)) )
    {
        wxFile file(WXC_CONFIG, wxFile::write);
        WXCLog::Do(wxString::Format("Log-file %s doesn't exists! Create a default one...", WXC_CONFIG));

        file.Write
        (
            wxString::Format
            (
                "# Maximum size of the log-file (%s) in KiloByte\n" \
                "log-size=%d\n\n" \
                "# Intervall in minutes to check crontab for modifications.\n" \
                "# 0 = Check on only on startup of wxCron.\n" \
                "crontab-check=%d\n",
                WXC_CONFIG,
                GetMaxLogFileSizeInKB(),
                GetCheckCrontabIntervallInMinutes()
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
    wxFileInputStream   is(WXC_CONFIG);
    wxFileConfig        fileConfig(is);
    long                lVal;

    // maximum log-file size
    if ( fileConfig.Read("log-size", &lVal) )
        lMaxSize_ = lVal;

    // crontab check intervall
    if ( fileConfig.Read("crontab-check", &lVal) )
        lCheckCrontab_ = lVal;
}

bool WXCConfig::SaveConfig ()
{
    wxFileInputStream   is(WXC_CONFIG);
    wxFileConfig        fileConfig(is);

    // maximum log-file size
    fileConfig.Write("log-size", lMaxSize_);

    // crontab check intervall
    fileConfig.Write("crontab-check", lCheckCrontab_);

    // save
    wxFileOutputStream os(WXC_CONFIG);

    if ( !(fileConfig.Save(os)) )
    {
        WXCLog::Do(wxString::Format("Error while saving the config file %s.", WXC_CONFIG));
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

long WXCConfig::GetCheckCrontabIntervallInMinutes ()
{
    return lCheckCrontab_;
}

void WXCConfig::SetCheckCrontabIntervallInMinutes (long lIntervall)
{
    lCheckCrontab_ = lIntervall;
}
