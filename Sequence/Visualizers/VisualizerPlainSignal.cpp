#include "VisualizerPlainSignal.h"
#include "..\StorageCursor.h"
#include "..\Storage.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "GlUtils\glDrawUtils.h"
#include "GlUtils\GlTextUtils.h"
#include <cassert>

VisualizerPlainSignal::VisualizerPlainSignal()
{
}

VisualizerPlainSignal::~VisualizerPlainSignal()
{
}

void VisualizerPlainSignal::SetMaxVisibleSamples( const int nSamples ) 
{ 
	_nMaxVisibleSamples = nSamples; 
}


void VisualizerPlainSignal::AddParams( const Params & params )
{
	_paramss.push_back( params );
}

void VisualizerPlainSignal::draw()
{
	assert( _storages.size() == _cursors.size() );

	glColor3f( params().color.x, params().color.y, params().color.z );
	draw_frame2D( _wnd_rect );
	glRasterPos2fv( _wnd_rect.minpt );
	glPrint( _caption );

	if( !cursor()->on_pause() ) // use first cursor for a while. TODO.
	{
		if( 0 == _coords.capacity() )
		{
			AllocGlBuffers( _nMaxVisibleSamples * (int)_storages.size() );
		}
		_coords.clear();

		_actual_points_numbers.clear();
		_actual_points_numbers.reserve( _storages.size() );
		for( int i = 0; i < (int)_storages.size(); ++i )
		{
			StorageCursor * cur = _cursors[i];
			Storage<float> * stor = _storages[i];

			cur->snapshort( stor );
			long iMax = min( cur->count(), _nMaxVisibleSamples );
			if( 0 == iMax )
				return;

			GLfloat dt = _wnd_rect.len[0] / _nMaxVisibleSamples;
			GLfloat t = _wnd_rect.len[0] * 0.5f;
			for( long pos = cur->count() - iMax; pos < cur->count(); ++pos )
			{
				_coords.push_back( _wnd_rect.center[0] + t );
				_coords.push_back( _wnd_rect.center[1] + _paramss[i].scale * stor->read_elem( pos ) );
				_coords.push_back( 0 );
				t -= dt;
			}
			_actual_points_numbers.push_back( iMax );
		}
	}

	glLineWidth( 1. );
	int ind = 0;
	for( int i = 0; i < (int)_storages.size(); ++i )
	{
		glColor3f( _paramss[i].color.x, _paramss[i].color.y, _paramss[i].color.z );
		draw_arrays( &_coords[ind*3], _actual_points_numbers[i] );
		ind += _actual_points_numbers[i];
	}
}