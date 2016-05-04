#include <assert.h>
#include "lms7002_mainPanel.h"
#include "lms7002_pnlAFE_view.h"
#include "lms7002_pnlBIAS_view.h"
#include "lms7002_pnlBIST_view.h"
#include "lms7002_pnlCDS_view.h"
#include "lms7002_pnlCLKGEN_view.h"
#include "lms7002_pnlLDO_view.h"
#include "lms7002_pnlLimeLightPAD_view.h"
#include "lms7002_pnlTxTSP_view.h"
#include "lms7002_pnlRxTSP_view.h"
#include "lms7002_pnlRBB_view.h"
#include "lms7002_pnlRFE_view.h"
#include "lms7002_pnlSX_view.h"
#include "lms7002_pnlTBB_view.h"
#include "lms7002_pnlTRF_view.h"
#include "lms7002_pnlXBUF_view.h"
#include "lms7002_pnlCalibrations_view.h"
#include <wx/time.h>
#include <wx/msgdlg.h>
#include <iostream>
#include <wx/filedlg.h>
#include "lms7suiteEvents.h"
#include "lms7002_pnlMCU_BD_view.h"
#include "lms7002_pnlBuffers_view.h"
#include "lime/LimeSuite.h"
using namespace std;
using namespace lime;

lms7002_mainPanel::lms7002_mainPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    :
    mainPanel(parent, id, pos, size, style), lmsControl(nullptr)
{
    mTabMCU = new lms7002_pnlMCU_BD_view(tabsNotebook);
    tabsNotebook->AddPage(mTabMCU, _("MCU"));
}

lms7002_mainPanel::~lms7002_mainPanel()
{
}

void lms7002_mainPanel::UpdateVisiblePanel()
{
    wxLongLong t1, t2;
    t1 = wxGetUTCTimeMillis();
    long visibleTabId = tabsNotebook->GetCurrentPage()->GetId();
    switch (visibleTabId)
    {
    case ID_TAB_AFE:
        mTabAFE->UpdateGUI();
        break;
    case ID_TAB_BIAS:
        mTabBIAS->UpdateGUI();
        break;
    case ID_TAB_BIST:
        mTabBIST->UpdateGUI();
        break;
    case ID_TAB_CDS:
        mTabCDS->UpdateGUI();
        break;
    case ID_TAB_CGEN:
        mTabCGEN->UpdateGUI();
        break;
    case ID_TAB_LDO:
        mTabLDO->UpdateGUI();
        break;
    case ID_TAB_LIMELIGHT:
        mTabLimeLight->UpdateGUI();
        break;
    case ID_TAB_TXTSP:
        mTabTxTSP->UpdateGUI();
        break;
    case ID_TAB_RXTSP:
        mTabRxTSP->UpdateGUI();
        break;
    case ID_TAB_RBB:
        mTabRBB->UpdateGUI();
        break;
    case ID_TAB_RFE:
        mTabRFE->UpdateGUI();
        break;
    case ID_TAB_SXR:
        mTabSXR->UpdateGUI();
        break;
    case ID_TAB_SXT:
        mTabSXT->UpdateGUI();
        break;
    case ID_TAB_TBB:
        mTabTBB->UpdateGUI();
        break;
    case ID_TAB_TRF:
        mTabTRF->UpdateGUI();
        break;
    case ID_TAB_XBUF:
        mTabXBUF->UpdateGUI();
        break;
    case ID_TAB_CALIBRATIONS:
        mTabCalibrations->UpdateGUI();
        break;
    case ID_TAB_BUFFERS:
        mTabBuffers->UpdateGUI();
        break;
    }
    t2 = wxGetUTCTimeMillis();
#ifndef NDEBUG
    cout << "Visible GUI update time: " << (t2 - t1).ToString() << endl;
#endif
}

void lms7002_mainPanel::Initialize(lms_device_t* pControl)
{
    assert(pControl != nullptr);
    lmsControl = pControl;
    mTabRFE->Initialize(lmsControl);
    mTabRBB->Initialize(lmsControl);
    mTabTRF->Initialize(lmsControl);
    mTabTBB->Initialize(lmsControl);
    mTabAFE->Initialize(lmsControl);
    mTabBIAS->Initialize(lmsControl);
    mTabLDO->Initialize(lmsControl);
    mTabXBUF->Initialize(lmsControl);
    mTabCGEN->Initialize(lmsControl);
    mTabSXR->Initialize(lmsControl);
    mTabSXT->Initialize(lmsControl);
    mTabLimeLight->Initialize(lmsControl);
    mTabTxTSP->Initialize(lmsControl);
    mTabRxTSP->Initialize(lmsControl);
    mTabCDS->Initialize(lmsControl);
    mTabBIST->Initialize(lmsControl);
    mTabCalibrations->Initialize(lmsControl);
    mTabMCU->Initialize(lmsControl);
// TODO setup buffers gui
    //mTabBuffers->Initialize(lmsControl->GetControlPort());
    UpdateGUI();
}

void lms7002_mainPanel::OnResetChip(wxCommandEvent &event)
{
    int status = LMS_Reset(lmsControl);
    if (status != 0)
        wxMessageBox(wxString::Format(_("Chip reset: %s"), wxString::From8BitData(LMS_GetLastErrorMessage())), _("Warning"));
    wxNotebookEvent evt;
    Onnotebook_modulesPageChanged(evt); //after reset chip active channel might change, this refresh channel for active tab
}

void lms7002_mainPanel::UpdateGUI()
{
    wxLongLong t1, t2;
    t1 = wxGetUTCTimeMillis();
    t2 = wxGetUTCTimeMillis();
    uint16_t chan;
    LMS_ReadParam(lmsControl,LMS7param(MAC),&chan);
    if (chan == 1)
    {
        rbChannelA->SetValue(true);
        rbChannelB->SetValue(false);
    }
    else if (chan == 2)
    {
        rbChannelA->SetValue(false);
        rbChannelB->SetValue(true);
    }
    else
    {
        LMS_WriteParam(lmsControl,LMS7param(MAC),1);
        rbChannelA->SetValue(true);
        rbChannelB->SetValue(false);
    }

    UpdateVisiblePanel();
#ifndef NDEBUG
    cout << "GUI update time: " << (t2 - t1).ToString() << endl;
#endif
}

void lms7002_mainPanel::OnNewProject( wxCommandEvent& event )
{
    LMS_Reset(lmsControl);
    LMS_WriteParam(lmsControl,LMS7param(MAC),rbChannelA->GetValue() == 1 ? 1: 2);
    LMS_WriteParam(lmsControl,LMS7param(MAC),1);
    UpdateGUI();
}

void lms7002_mainPanel::OnOpenProject( wxCommandEvent& event )
{
    wxFileDialog dlg(this, _("Open config file"), "", "", "Project-File (*.ini)|*.ini", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dlg.ShowModal() == wxID_CANCEL)
        return;
    int status = LMS_LoadConfig(lmsControl,dlg.GetPath().To8BitData());
    if (status != 0)
    {
            wxMessageBox(wxString::Format(_("Failed to load file: %s"), LMS_GetLastErrorMessage()), _("Warning"));
    }
    wxCommandEvent tevt;
    LMS_WriteParam(lmsControl,LMS7param(MAC),rbChannelA->GetValue() == 1 ? 1: 2);
    UpdateGUI();
    wxCommandEvent evt;
    evt.SetEventType(CGEN_FREQUENCY_CHANGED);
    wxPostEvent(this, evt);
}

void lms7002_mainPanel::OnSaveProject( wxCommandEvent& event )
{
    wxFileDialog dlg(this, _("Save config file"), "", "", "Project-File (*.ini)|*.ini", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() == wxID_CANCEL)
        return;
    int status = LMS_LoadConfig(lmsControl,dlg.GetPath().To8BitData());
    if (status != 0)
        wxMessageBox(_("Failed to save file"), _("Warning"));
}

void lms7002_mainPanel::OnRegistersTest( wxCommandEvent& event )
{
    int status = LMS_RegisterTest(lmsControl);
    if (status != 0)
        wxMessageBox(_("Registers test failed!"), _("WARNING"));
    else
        wxMessageBox(_("Registers test passed!"), _("INFO"));
}

void lms7002_mainPanel::OnSwitchToChannelA(wxCommandEvent& event)
{
    LMS_WriteParam(lmsControl,LMS7param(MAC),1);
    UpdateVisiblePanel();
}

void lms7002_mainPanel::OnSwitchToChannelB(wxCommandEvent& event)
{
    LMS_WriteParam(lmsControl,LMS7param(MAC),2);
    UpdateVisiblePanel();
}

void lms7002_mainPanel::Onnotebook_modulesPageChanged( wxNotebookEvent& event )
{
    wxNotebookPage* page = tabsNotebook->GetCurrentPage();
    if (page == mTabAFE || page == mTabBIAS || page == mTabLDO || page == mTabXBUF || page == mTabCGEN || page == mTabCDS || page == mTabBIST)
    {
        rbChannelA->Disable();
        rbChannelB->Disable();
    }
    else if (page == mTabSXR) //change active channel to A
    {
        LMS_WriteParam(lmsControl,LMS7param(MAC),1);
        rbChannelA->Disable();
        rbChannelB->Disable();
    }
    else if (page == mTabSXT) //change active channel to B
    {
        LMS_WriteParam(lmsControl,LMS7param(MAC),2);
        rbChannelA->Disable();
        rbChannelB->Disable();
    }
    else
    {
        LMS_WriteParam(lmsControl,LMS7param(MAC),rbChannelA->GetValue() == 1 ? 1: 2);
        rbChannelA->Enable();
        rbChannelB->Enable();
    }
    UpdateVisiblePanel();
}

void lms7002_mainPanel::OnDownloadAll(wxCommandEvent& event)
{
    int status = LMS_Synchronize(lmsControl,false);
    if (status != 0)
        wxMessageBox(wxString::Format(_("Download all registers: %s"), wxString::From8BitData(LMS_GetLastErrorMessage())), _("Warning"));
    UpdateVisiblePanel();
}

void lms7002_mainPanel::OnUploadAll(wxCommandEvent& event)
{
    int status = LMS_Synchronize(lmsControl,true);
    if (status != 0)
        wxMessageBox(wxString::Format(_("Upload all registers: %s"), wxString::From8BitData(LMS_GetLastErrorMessage())), _("Warning"));
    wxCommandEvent evt;
    evt.SetEventType(CGEN_FREQUENCY_CHANGED);
    wxPostEvent(this, evt);
    UpdateVisiblePanel();
}
