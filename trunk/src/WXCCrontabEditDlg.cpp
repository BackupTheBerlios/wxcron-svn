/**
 * Name:        WXCCrontabEditDlg.h
 * Purpose:     WXCCrontabEditDlg class implementation
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

#include "WXCCrontabEditDlg.h"

#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/textfile.h>

#include "WXCApp.h"
#include "wxCron.h"

BEGIN_EVENT_TABLE(WXCCrontabEditDlg, wxDialog)
  EVT_CLOSE (WXCCrontabEditDlg::OnClose)
END_EVENT_TABLE()

//
WXCCrontabEditDlg::WXCCrontabEditDlg ()
            : wxDialog( NULL,
					    wxID_ANY,
						wxString(_("Edit ")) + WXCApp::GetCrontabFilename(),
						wxDefaultPosition,
						wxDefaultSize,
						wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    Init();
	Show();
}


//
/*virtual*/ WXCCrontabEditDlg::~WXCCrontabEditDlg ()
{
}

void WXCCrontabEditDlg::ReadCrontab (wxString& strCrontabContent, unsigned int& iLineLength, unsigned int& iLineCount)
{
	strCrontabContent	.Clear();
	iLineLength			= 0;
	iLineCount			= 0;

    wxTextFile file(WXCApp::GetCrontabFilename());

	file.Open();

    // error while opening
    if ( !(file.IsOpened()) )
    {
        wxMessageBox(wxString::Format("Can not open the crontab file '%s' for reading.", WXCApp::GetCrontabFilename()), "Error");
        return;
    }

    // read the file
    for (wxString strLine = file.GetFirstLine();
         !file.Eof();
         strLine = file.GetNextLine())
    {
        strCrontabContent << strLine << "\n";

		// count the line
		iLineCount++;

		// count the characters
		if ( iLineLength < strLine.Length() )
			iLineLength = strLine.Length();
    }
}

void WXCCrontabEditDlg::SaveCrontab (wxString& strCrontabContent)
{
	wxTempFile file(WXCApp::GetCrontabFilename());

    // error while opening
    if ( !(file.IsOpened()) )
    {
        wxMessageBox(wxString::Format("Can not open the crontab file '%s' writing.", WXCApp::GetCrontabFilename()), "Error");
        return;
    }

	file.Write(strCrontabContent);
	file.Commit();
}

void WXCCrontabEditDlg::Init ()
{
	wxString strCrontabContent;
	unsigned int iLineCount, iLineLength;

	ReadCrontab(strCrontabContent, iLineLength, iLineCount);

	if ( iLineLength < 5 )
		iLineLength = 20;

	if ( iLineCount < 2 )
		iLineCount = 3;

    // create the text control
    pCtrl_ = new wxTextCtrl(this,
							wxID_ANY,
							wxEmptyString,
							wxDefaultPosition,
							wxDefaultSize,
							wxTE_MULTILINE | wxTE_RICH2);

    wxFont font(pCtrl_->GetFont());
    font.SetFaceName("Courier New");
    font.SetFamily(wxFONTFAMILY_MODERN);
    pCtrl_->SetFont(font);

	// window size
	wxSize sStr		= wxWindowDC(pCtrl_).GetTextExtent("W");
	int iWidthMax	= wxScreenDC().GetSize().GetWidth() / 10 * 9;
	int iHeightMax	= wxScreenDC().GetSize().GetHeight() / 10 * 9;

	int iWidthStr	= sStr.GetWidth() * (iLineLength+10);
	int iHeightStr	= sStr.GetHeight() * (iLineCount+3);

	if ( iWidthStr > iWidthMax )
		iWidthStr = iWidthMax;

	if ( iHeightStr > iHeightMax )
		iHeightStr = iHeightMax;

	// arrange
	wxSizer* pSizer = new wxBoxSizer(wxVERTICAL);
	pSizer->Add(pCtrl_, wxSizerFlags(1).Expand());
	SetSizer(pSizer);

	SetSize(iWidthStr, iHeightStr);

	Center();

	//
	pCtrl_->AppendText(strCrontabContent);
	pCtrl_->DiscardEdits();
}

void WXCCrontabEditDlg::OnClose (wxCloseEvent& event)
{
	if ( pCtrl_->IsModified() )
	{
		if ( wxMessageBox("Do you want to save the changes to the crontab file?", "Save changes?", wxYES_NO) == wxYES )
		{
			wxString strCrontabContent = pCtrl_->GetValue();
			SaveCrontab(strCrontabContent);
		}
	}

    Destroy();	
}
