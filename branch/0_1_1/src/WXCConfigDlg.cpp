/**
 * Name:        WXCConfigDlg.cpp
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


#include "WXCConfigDlg.h"

#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/msgdlg.h>
#include <wx/stattext.h>
#include <wx/gbsizer.h>

#include "WXCConfig.h"
#include "WXCLog.h"

#define WXCCONFIGDLG_ID_BUTTONOK            101 + wxID_HIGHEST
#define WXCCONFIGDLG_ID_BUTTONCANCEL        102 + wxID_HIGHEST


BEGIN_EVENT_TABLE(WXCConfigDlg, wxDialog)
  EVT_CLOSE     (                                       WXCConfigDlg::OnClose)
  EVT_BUTTON    (WXCCONFIGDLG_ID_BUTTONOK,              WXCConfigDlg::OnButton_Ok)
  EVT_BUTTON    (WXCCONFIGDLG_ID_BUTTONCANCEL,          WXCConfigDlg::OnButton_Cancel)
END_EVENT_TABLE()


WXCConfigDlg::WXCConfigDlg ()
            : wxDialog (NULL, wxID_ANY, _("Settings for wxCron")),
              pSpinMaxLog_(NULL),
              pSpinCheckCrontab_(NULL)
{
    // the book
    wxNotebook* pBook = new wxNotebook(this, wxID_ANY);

    // GENERAL page
    wxPanel* pPage = new wxPanel (pBook);

    // max log size
    wxStaticText* pStaticMaxLog = new wxStaticText(pPage, wxID_ANY, _("size (in KB) of application logfile: "));
    pSpinMaxLog_ = new wxSpinCtrl(pPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, INT_MAX);

    // check intervall
    wxStaticText* pStaticCheckCrontab = new wxStaticText(pPage, wxID_ANY, _("Intervall in minutes to check crontab for modifications: "));
    pSpinCheckCrontab_ = new wxSpinCtrl(pPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, INT_MAX);

    wxGridBagSizer* pGBSizer = new wxGridBagSizer(5);
    pGBSizer->Add(pStaticMaxLog,        wxGBPosition(0, 0), wxGBSpan(), wxALIGN_CENTER_VERTICAL);
    pGBSizer->Add(pSpinMaxLog_,         wxGBPosition(0, 1), wxGBSpan(), wxALIGN_CENTER_VERTICAL);
    pGBSizer->Add(pStaticCheckCrontab,  wxGBPosition(1, 0), wxGBSpan(), wxALIGN_CENTER_VERTICAL);
    pGBSizer->Add(pSpinCheckCrontab_,   wxGBPosition(1, 1), wxGBSpan(), wxALIGN_CENTER_VERTICAL);
    wxSizer* pPageSizer = new wxBoxSizer(wxVERTICAL);
    pPageSizer->Add(pGBSizer, wxSizerFlags(0).Border());
    pPage->SetSizer(pPageSizer);

    pBook->AddPage(pPage, _("General"));

    // BUTTONs
    wxButton* pButton_Ok        = new wxButton(this, WXCCONFIGDLG_ID_BUTTONOK, "Ok");
    wxButton* pButton_Cancel    = new wxButton(this, WXCCONFIGDLG_ID_BUTTONCANCEL, _("Cancel"));

    wxSizer* pSizerH = new wxBoxSizer(wxHORIZONTAL);
    pSizerH->Add(pButton_Ok,        wxSizerFlags(0).Border());
    pSizerH->Add(pButton_Cancel,    wxSizerFlags(0).Border());

    wxSizer* pSizerV = new wxBoxSizer(wxVERTICAL);
    pSizerV->Add(pBook,     wxSizerFlags(0).Expand().Border());
    pSizerV->Add(pSizerH,   wxSizerFlags(0).Center());

    SetSizerAndFit(pSizerV);
    ReadDataFromConfig();
    Show();
}


/*virtual*/ WXCConfigDlg::~WXCConfigDlg ()
{
}


void WXCConfigDlg::OnClose (wxCloseEvent& event)
{
    Destroy();
}


void WXCConfigDlg::OnButton_Ok (wxCommandEvent& rEvent)
{
    SaveDataToConfig();

    if ( WXCConfig::Instance().SaveConfig() == false )
        wxMessageBox(WXCLog::GetLastMessage());

    Close();
}

void WXCConfigDlg::OnButton_Cancel (wxCommandEvent& rEvent)
{
    Close();
}

void WXCConfigDlg::ReadDataFromConfig ()
{
    pSpinMaxLog_        ->SetValue(WXCConfig::Instance().GetMaxLogFileSizeInKB());
    pSpinCheckCrontab_  ->SetValue(WXCConfig::Instance().GetCheckCrontabIntervallInMinutes());
}

void WXCConfigDlg::SaveDataToConfig ()
{
    WXCConfig::Instance().SetMaxLogFileSizeInKB(pSpinMaxLog_->GetValue());
    WXCConfig::Instance().SetCheckCrontabIntervallInMinutes(pSpinCheckCrontab_->GetValue());

    WXCConfig::Instance().SaveConfig();
}

