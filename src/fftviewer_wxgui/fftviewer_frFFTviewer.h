#ifndef __fftviewer_frFFTviewer__
#define __fftviewer_frFFTviewer__

/**
@file
Subclass of frFFTviewer, which is generated by wxFormBuilder.
*/
#include <thread>
#include <atomic>
#include <vector>
#include "fftviewer_wxgui.h"
#include "lime/LimeSuite.h"
#include <wx/timer.h>
//// end generated include

/** Implementing frFFTviewer */
class fftviewer_frFFTviewer : public frFFTviewer
{
protected:
	// Handlers for frFFTviewer events.
	void OnFFTsamplesCountChanged( wxSpinEvent& event );
	void OnWindowFunctionChanged( wxCommandEvent& event );
	void OnbtnStartStop( wxCommandEvent& event );
        void OnChannelVisibilityChange(wxCommandEvent& event);
        static void Streamer(fftviewer_frFFTviewer* pthis, const unsigned int fftSize, const int channelsCount, const uint32_t format);

        void OnUpdatePlots(wxTimerEvent& event);
	public:
		/** Constructor */
		fftviewer_frFFTviewer( wxWindow* parent );
        ~fftviewer_frFFTviewer();
	//// end generated class members
        void Initialize(lms_device_t* pDataPort);
        void SetNyquistFrequency(float freqMHz);

        void StartStreaming();
        void StopStreaming();

protected:
    
    struct DataToGUI
    {
        std::vector<float> samplesI[2];
        std::vector<float> samplesQ[2];
        std::vector<float> fftBins_dbFS[2];
        float nyquist_Hz;
        float rxDataRate_Bps;
        float txDataRate_Bps;

        DataToGUI& operator=(const DataToGUI& src)
        {
            for (int ch = 0; ch < 2; ++ch)
            {
                this->samplesI[ch].clear();
                this->samplesI[ch].reserve(src.samplesI[ch].size());
                this->samplesI[ch] = src.samplesI[ch];
                this->samplesQ[ch].clear();
                this->samplesQ[ch].reserve(src.samplesQ[ch].size());
                this->samplesQ[ch] = src.samplesQ[ch];
                this->fftBins_dbFS[ch].clear();
                this->fftBins_dbFS[ch].reserve(src.fftBins_dbFS[ch].size());
                this->fftBins_dbFS[ch] = src.fftBins_dbFS[ch];
                this->nyquist_Hz = src.nyquist_Hz;
                this->rxDataRate_Bps = src.rxDataRate_Bps;
                this->txDataRate_Bps = src.txDataRate_Bps;
            }
            return *this;
        }
    }streamData;
    
    std::atomic<bool> mStreamRunning;
    std::atomic<bool> stopProcessing;
    std::thread threadProcessing;
    wxString printDataRate(float dataRate);

    lms_device_t* lmsControl;
    wxTimer* mGUIupdater;
};

#endif // __fftviewer_frFFTviewer__
