/**
 * Name:        WXCLogViewDlg.h
 * Purpose:     WXCLogViewDlg class implementation
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

#include "WXCLogViewDlg.h"

#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/textfile.h>
#

#include "WXCApp.h"
#include "wxCron.h"

BEGIN_EVENT_TABLE(WXCLogViewDlg, wxDialog)
  EVT_CLOSE (WXCLogViewDlg::OnClose)
END_EVENT_TABLE()

//
WXCLogViewDlg::WXCLogViewDlg ()
            : wxDialog( NULL,
					    wxID_ANY,
						wxString(_("Log for ")) + WXCApp::GetFullApplicationName(),
						wxDefaultPosition,
						wxDefaultSize,
						wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE)
{
    Init();
	Show();
}


//
/*virtual*/ WXCLogViewDlg::~WXCLogViewDlg ()
{
}

void WXCLogViewDlg::Init ()
{
    // the log file
    wxTextFile fileLog;
	fileLog.Open(WXC_LOG);

    // error while opening
    if ( !(fileLog.IsOpened()) )
    {
        wxMessageBox(wxString::Format("Can not open the log file %s.", WXC_LOG), "Error");
        return;
    }

    // create the text control
    wxTextCtrl* pCtrl = new wxTextCtrl(this,
                                       wxID_ANY,
                                       wxEmptyString,
                                       wxDefaultPosition,
                                       wxDefaultSize,
                                       wxTE_MULTILINE | wxTE_READONLY);
    wxFont font(pCtrl->GetFont());
    font.SetFaceName("Courier New");
    font.SetFamily(wxFONTFAMILY_MODERN);
    pCtrl->SetFont(font);

	wxString strL;

    // read the file
    for (wxString strLine = fileLog.GetFirstLine();
         !fileLog.Eof();
         strLine = fileLog.GetNextLine())
    {
        (*pCtrl) << strLine << "\n";

		if (strL.Length() < strLine.Length())
			strL = strLine;
    }

	// width
	strL = strL + "          ";
	int iWidthStr = wxWindowDC(pCtrl).GetTextExtent(strL).GetWidth();
	int iWidthMax = wxScreenDC().GetSize().GetWidth() / 10 * 9;

	if ( iWidthStr > iWidthMax )
		iWidthStr = iWidthMax;

	// XXX (*pCtrl) << wxString::Format("\n\niWidthStr: %d\tiWidthMax: %d\n%s", iWidthStr, iWidthMax, strL);

	// arrange
	wxSizer* pSizer = new wxBoxSizer(wxVERTICAL);
	pSizer->Add(pCtrl, wxSizerFlags(1).Expand());
	SetSizer(pSizer);

	SetSize(iWidthStr, GetSize().GetHeight());

	Center();

	pCtrl->SetInsertionPointEnd();
}

void WXCLogViewDlg::OnClose (wxCloseEvent& event)
{
    Destroy();
}
