/*#pragma once

#include "stdafx.h"
#include "Visualizer.h"
#include <fftw3.h>

class StorageCursor;

class VisualizerFFT : public Visualizer
{
	fftwf_complex * _fftwf_in;
	fftwf_complex * _fftwf_out;
	fftwf_plan _fftwf_plan;
	fftwf_plan _ifftwf_plan;
	int _fft_window;
public:

	VisualizerFFT();
	virtual ~VisualizerFFT();
	
	virtual void init( const int nSamples );
	virtual void freemem();
	virtual void draw( StorageCursor * storage_cursor );
};
*/