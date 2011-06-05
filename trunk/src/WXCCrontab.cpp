/**
 * Name:        WXCCrontab.cpp
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-08
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


#include "WXCCrontab.h"

#include <wx/textfile.h>
#include <wx/regex.h>
#include <wx/filename.h>

#include "WXCLog.h"
#include "WXCApp.h"
#include "wxCron.h"

/*static*/ WXCCrontab WXCCrontab::sCrontab_;

WXCCrontab::WXCCrontab ()
{
}


/*virtual*/ WXCCrontab::~WXCCrontab ()
{
}


/*static*/ void WXCCrontab::Read ()
{
    // check if config file exists
	if ( !(wxFile::Exists(WXCApp::GetCrontabFilename())) )
    {
        wxFile file(WXCApp::GetCrontabFilename(), wxFile::write);
        WXCLog::Do(wxString::Format("Crontab-file %s doesn't exists! Create a default one...", WXCApp::GetCrontabFilename()));

        file.Write
        (
            wxString::Format
            (
                "# just a sample crontab\n" \
                "#M    S   T M W    Befehl\n" \
                "\n" \
                "# each hour 3. minute\n" \
                "#3     *   * * *    calc.exe\n" \
                "\n" \
                "# 5 minute intervals\n" \
                "#*/5   *   * * *    calc.exe\n" \
                "\n" \
                "# wendsday 11:38\n" \
                "#38    11  * * 3    calc.exe\n" \
                "\n" \
                "# each midnight 0:00\n" \
                "#0     0   * * *    calc.exe\n" \
                "\n" \
                "# each thursday OR 7. of a month at 3:00 a'clock\n" \
                "#0     3   7 * 4    calc.exe\n" \
                "\n" \
                "# monday till friday 1:20 and 1:30\n" \
                "#20,50 1   * * 1-5  calc.exe\n" \
                "\n" \
                "# each midnight 0:00 but don't catchup the job\n" \
                "# if the machine wasn't on power at last midnight\n" \
                "#@nocatchup 0     0   * * *    calc.exe\n" \
                "\n" \
                "#hide the running job\n" \
                "#@hide 0 0 * * * script.bat\n"
            )
        );
    }


    wxTextFile file;

    // open the file
	if ( file.Open(WXCApp::GetCrontabFilename()) == false )
    {
        WXCLog::Do(wxString::Format("ERROR: The crontab file %s couldn't be loaded!", WXCApp::GetCrontabFilename()));
        return;
    }

    // remember modification time
    Instance().timeMod_ = wxFileName(WXCApp::GetCrontabFilename()).GetModificationTime();

    // clear job vector and their timers
    Instance().vecJobs_.clear();

    // line counter
    long lLine = 1;

    // regex for spaces
    wxRegEx regex("[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+");

    // read the file
    for (wxString strLine = file.GetFirstLine();
         !file.Eof();
         strLine = file.GetNextLine())
    {
        // remove white-spaces from the beginning and end of line
        strLine.Trim(true);
        strLine.Trim(false);

        // ignore blank lines and comments
        if ( !(strLine.IsEmpty())
          && !(strLine.StartsWith("#")) )
        {
            // check for enough parameters
            if ( regex.Matches(strLine) )
            {
                wxArrayString arrOptions;
                wxString strOpt;

                // check for options
                while ( strLine.StartsWith("@") )
                {
                    strOpt = strLine.BeforeFirst(' ');
                    strLine = strLine.AfterFirst(' ');
                    arrOptions.Add(strOpt);
                }

                Instance().vecJobs_.push_back(WXCJob(strLine, lLine, arrOptions));
            }
            else
            {
                wxString strErr = wxString::Format("ERROR: Line %d in %s is not valid!\n\"%s\"",
                                                   lLine,
                                                   WXCApp::GetCrontabFilename(),
                                                   strLine);
                wxLogError(strErr);
                WXCLog::Do(strErr);
            }
        }

        // increment line counter
        ++lLine;
    }
}

/*static*/ bool WXCCrontab::CheckModification ()
{
    bool rc = false;

    if ( Instance().timeMod_ != wxFileName(WXCApp::GetCrontabFilename()).GetModificationTime() )
    {
        WXCLog::Do("crontab was modified. Read it again...");

        rc = true;

        //
        Read();

        // start the jobs again
        Instance().Start ();
    }

    return rc;
}

bool WXCCrontab::HasLine (const wxString& strLine)
{
    for (WXCJobVectorIt it = vecJobs_.begin();
         it != vecJobs_.end();
         ++it)
    {
        if ( (*it).GetOriginalLine() == strLine )
            return true;
    }

    return false;
}


/* XXX
#include "WXCTimeValue.h"
#include "WXCTime.h"

void WXCCrontab::XXX (const wxString& str, short iFrom, short iTo)
{
    WXCTimeValue tv(str, iFrom, iTo);
    bool bHas;
    short iNext;
    wxString strLog;

    for (short i = 0; i < 100; ++i)
    {
        bHas = tv.HasThisValue(i);
        iNext = tv.GetNextValue(i);

        WXCLog::Do(wxString::Format("\"%s\" %d - %d: %s;\tCURRENT: %d => NEXT: %d",
                               str,
                               iFrom,
                               iTo,
                               bHas ? "HAS" : "HAS NOT",
                               i,
                               iNext), false);

    }
    WXCLog::Do(wxEmptyString, false);
}

void WXCCrontab::YYY (const wxString& str)
{
    WXCTime t(str);
    wxDateTime dt(19, wxDateTime::May, 2008, 12, 54, 37);
    wxDateTime dtNext(dt);

    WXCLog::Do(str, false);
    for (short i = 0; i < 500; ++i)
    {
        dtNext = t.GetNext(dt);
        WXCLog::Do(wxString::Format("%d;  CURRENT: %s => NEXT: %s",
                               i,
                               dt.Format(),
                               dtNext.Format()), false);
        dt = dtNext;

    }
    WXCLog::Do(wxEmptyString, false);
}
*/

void WXCCrontab::Start ()
{
    for (WXCJobVectorIt it = vecJobs_.begin();
         it != vecJobs_.end();
         ++it)
    {
        (*it).Start();
    }

    //OK YYY("5 * * * *");
    //OK YYY("*/5 * * * *");
    //OK YYY("5,45 * * * *");
    //OK YYY("5-50/7 * * * *");

    //OK YYY("* 5 * * *");
    //OK YYY("* */5  * * *");
    //OK YYY("* 5,13  * * *");
    //OK YYY("* 6-22/4  * * *");

    //OK YYY("30   1   5 * *");
    //OK YYY("30   1   */5 * *");
    //OK YYY("30   1   3,14 * *");
    //OK YYY("30   1   5-21/3 * *");

    //OK YYY("0 0 * 5 * ");
    //OK YYY("0 0 * */5 *");
    //OK YYY("0 0 * 5,10  *");
    //OK YYY("0 0 * 2-9/3 *");

    //OK YYY("0 0 * * 5");
    //OK YYY("0 0 * * */2 ");
    //OK YYY("0 0 * * 1,5 ");
    //OK YYY("0 0 * * 1-5/2");

    //OK YYY("0 0 10-30 * 5");
    //OK YYY("0 0 10 * */2 ");
    //OK YYY("0 0 10,16 * 1,5 ");
    //OK YYY("0 0 22,28 * 1-5/2");

    //OK YYY("17 *    * * *");
    //OK YYY("25 11   * * *");
    //OK YYY("47 12   * * 7");
    //OK YYY("52 13   1 * *");
    //OK YYY("50 10   * * *");
    //??? YYY("3/17 *  * * *");
    //OK YYY("22 */4  * * *");
    //OK YYY("*/9 *   * * *");

    //return;
}
