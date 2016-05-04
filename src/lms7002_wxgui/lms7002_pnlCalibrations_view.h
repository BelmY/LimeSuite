#ifndef __lms7002_pnlCalibrations_view__
#define __lms7002_pnlCalibrations_view__

/**
@file
Subclass of pnlCalibrations_view, which is generated by wxFormBuilder.
*/

#include "lms7002_wxgui.h"

//// end generated include
#include <map>
#include "lime/LimeSuite.h"
namespace lime{

}
/** Implementing pnlCalibrations_view */
class lms7002_pnlCalibrations_view : public pnlCalibrations_view
{
	protected:
		// Handlers for pnlCalibrations_view events.
        void ParameterChangeHandler(wxCommandEvent& event);
		void ParameterChangeHandler( wxSpinEvent& event );
		void OnbtnCalibrateRx( wxCommandEvent& event );
		void OnbtnCalibrateTx( wxCommandEvent& event );
		void OnbtnCalibrateAll( wxCommandEvent& event );
        
	public:
		/** Constructor */
		lms7002_pnlCalibrations_view( wxWindow* parent );
	//// end generated class members
        lms7002_pnlCalibrations_view(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
    void Initialize(lms_device_t* pControl);
    void UpdateGUI();
protected:
    lms_device_t* lmsControl;
    std::map<wxWindow*, LMS7Parameter> wndId2Enum;
};

#endif // __lms7002_pnlCalibrations_view__
