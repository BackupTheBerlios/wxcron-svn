/**
 * Name:        WXCTimestampFile.cpp
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


#include "WXCTimestampFile.h"

#include <wx/textfile.h>
#include <wx/utils.h>

#include "WXCLog.h"
#include "WXCCrontab.h"
#include "wxCron.h"


/*static*/ WXCTimestampFile WXCTimestampFile::sFile_;


WXCTimestampFile::WXCTimestampFile ()
{
}


/*virtual*/ WXCTimestampFile::~WXCTimestampFile ()
{
}


/*static*/ WXCTimestampFile& WXCTimestampFile::Instance ()
{
    return sFile_;
}


long WXCTimestampFile::Count ()
{
    return vecTimestamps_.size();
}


void WXCTimestampFile::Set(const wxString& strOriginalLine, const wxDateTime& dt /*= wxDateTime::Now()*/)
{
    // is there a old timestamp?
    for (VecPairTimeString::iterator it = vecTimestamps_.begin();
         it != vecTimestamps_.end();
         ++it)
    {
        if ( (*it).second == strOriginalLine )
        {
            (*it).first = dt;
            return;
        }
    }

    // add the new timestmap
    vecTimestamps_.push_back(PairTimeString(dt, strOriginalLine));
}

bool WXCTimestampFile::Read ()
{
    // clear all timestamps
    vecTimestamps_.clear();

    // check if timestamp file exists
    if ( !(wxFile::Exists(WXC_TIMESTAMP)) )
    {
        wxFile file(WXC_TIMESTAMP, wxFile::write);
        WXCLog::Do(wxString::Format("Timestamp-file %s doesn't exists! Create a default one...", WXC_TIMESTAMP));
    }

    // create temporary file
    wxTextFile file(WXC_TIMESTAMP);

    // open the file
    if (file.Open() == false)
    {
        WXCLog::Do(wxString::Format("Can not open the timestamp file %s for reading.", WXC_TIMESTAMP));
        return false;
    }

    wxDateTime dt;

    // read the file
    for (wxString strLine = file.GetFirstLine();
         !file.Eof();
         strLine = file.GetNextLine())
    {
        dt.ParseDateTime(strLine);
        strLine = file.GetNextLine();

        // check for unused timestamps
        if ( WXCCrontab::Instance().HasLine(strLine) == true )
            vecTimestamps_.push_back(PairTimeString(dt, strLine));
    }

    return true;
}


bool WXCTimestampFile::Save ()
{
    // create temporary file
    wxTempFile file(WXC_TIMESTAMP);

    // is open?
    if ( file.IsOpened() == false )
    {
        WXCLog::Do("Can not create temporary timestamp file.");
        return false;
    }

    for (VecPairTimeString::iterator it = vecTimestamps_.begin();
         it != vecTimestamps_.end();
         ++it)
    {
        // add timestamp and crontab line
        file.Write(wxString::Format("%s\n%s\n", (*it).first.Format(), (*it).second));
    }

    // close the temporary file and replace WXC_TIMESTAMP with it
    if ( file.Commit() == false )
    {
        WXCLog::Do(wxString::Format("Can not replace the old timestamp file %s with a new version of it.", WXC_TIMESTAMP));
        return false;
    }

    return true;
}
