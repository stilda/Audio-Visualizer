//#include "VisualizerFFT.h"
//#include "StorageCursor.h"
//#include <gl/gl.h>
//#include <gl/glu.h>
//#include "math.h"
//#include "mathUtils.h"
//#include "glDrawUtils.h"
//#include <fftw3.h>
//
//VisualizerFFT::VisualizerFFT()
//	: Visualizer(),
//	_fftwf_in(0),
//	_fftwf_out(0),
//	_fftwf_plan(0),
//	_ifftwf_plan(0),
//	_fft_window(0)
//{
//}
//
//VisualizerFFT::~VisualizerFFT()
//{
//}
//
//void VisualizerFFT::init( const int nSamples )
//{
//	_fft_window = 4096*2;
//	Visualizer::init( _fft_window );
//
//	_fftwf_in = fftwf_alloc_complex( _fft_window );
//	_fftwf_out = fftwf_alloc_complex( _fft_window );
//	_fftwf_plan = fftwf_plan_dft_1d( _fft_window, _fftwf_in, _fftwf_out, FFTW_FORWARD, FFTW_ESTIMATE);
//	_ifftwf_plan = fftwf_plan_dft_1d( _fft_window, _fftwf_out, _fftwf_in, FFTW_BACKWARD, FFTW_ESTIMATE);
//}
//
//void VisualizerFFT::freemem()
//{
//	Visualizer::freemem();
//	if( _fftwf_plan )
//		fftwf_destroy_plan( _fftwf_plan );
//	_fftwf_plan = 0;
//	if( _ifftwf_plan )
//		fftwf_destroy_plan( _ifftwf_plan );
//	_ifftwf_plan = 0;
//	if( _fftwf_in )
//		fftwf_free( _fftwf_in );
//	_fftwf_in = 0;
//	if( _fftwf_out )
//		fftwf_free( _fftwf_out );
//	_fftwf_out = 0;
//}
//
//void VisualizerFFT::draw( StorageCursor * storage_cursor )
//{
//	long recorded = storagecursor()->count();
//	int iMax = _fft_window;
//	GLfloat * pSource = storagecursor()->get_addr( _fft_window );
//	if( !pSource )
//		return;
//
//	for( int i = 0; i < _fft_window; i ++ )
//	{
//		_fftwf_in[i][0] = pSource[i];//+sin(2.0f*PIf/44100.f*2000.f * i) + sin(2.0f*PIf/44100.f*4000.f * i);
//		_fftwf_in[i][1] = 0;
//	}
//
//	fftwf_execute( _fftwf_plan );
//	_fftwf_out[0][0] = 0;
//	_fftwf_out[0][1] = 0;
//
//	GLfloat k = 5.0f;
//	GLfloat maxY = 0;
//	GLfloat * pDest = _coords;
//	for( int i = 0; i < (_fft_window/1); i++ )
//	{
//		pDest[0] = -_w*0.4f + log( 1 + k*i*1.0f/(_fft_window/1-1) ) / log(1+k)*_w*0.8f;
//		pDest[1] = log(1+sqrt( _fftwf_out[i][0]*_fftwf_out[i][0] + _fftwf_out[i][1]*_fftwf_out[i][1] ));
//		if( i > 0 ) 
//			maxY = max( maxY, pDest[1] );
//		pDest += 3;
//	}
//	maxY = _h * 0.4f / maxY;
//
//	glLineWidth( 1. );
//	glColor3f( 0.7f, 0.5f, 1.0f );
//	glPushMatrix();
//	glScalef( 1, maxY, 1 );
//	draw_arrays( _coords, (_fft_window/1) );
//	glPopMatrix();
//
///*
//	// Inverse
//	fftwf_execute( _ifftwf_plan );
//
//	maxY = 0;
//	pDest = _coords;
//	for( int i = 0; i < _fft_window; i++ )
//	{
//		pDest[0] = -_w*0.4f + i*_w*0.8f/(_fft_window-1);
//		pDest[1] = _fftwf_in[i][0] - _h*0.25f;
//		if( i > 0 ) 
//			maxY = max( maxY, pDest[1] );
//		pDest += 3;
//	}
//	maxY = _h * 0.4f / maxY;
//
//	glLineWidth( 1. );
//	glColor3f( 0, 0.5f, 1.0f );
//	glPushMatrix();
//	glScalef( 1, maxY, 1 );
//	draw_arrays( _coords, _fft_window );
//	glPopMatrix();
//*/
//}