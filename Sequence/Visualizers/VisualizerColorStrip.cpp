#include "VisualizerColorStrip.h"
#include "..\StorageCursor.h"
#include "..\Storage.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "GlUtils\glDrawUtils.h"
#include "GlUtils\GlTextUtils.h"
#include "Geometry\Vector3.h"

void VisualizerColorStrip::SetParams( const Params & params )
{
	_params = params;
}

void VisualizerColorStrip::SetMaxVisibleSamples( const int nSamples ) 
{ 
	_nMaxVisibleSamples = nSamples; 
}

void VisualizerColorStrip::draw() 
{
	glColor3f( _params.color.x, _params.color.y, _params.color.z );
	draw_frame2D( _wnd_rect );
	glRasterPos2fv( _wnd_rect.minpt );
	glPrint( _caption );

	if( !cursor()->on_pause() )
	{
		_coords.clear();
		_colors.clear();

		cursor()->snapshort( storage() );
		long iMax = min( cursor()->count(), _nMaxVisibleSamples );
		if( iMax == 0 )
			return;

		if( _coords.capacity() == 0 )
		{
			AllocGlBuffers( 2 * _nMaxVisibleSamples );
		}
	
		long pos = cursor()->count() - iMax;
		using namespace Vector3;
		GLfloat dt = _wnd_rect.len[0] / _nMaxVisibleSamples;
		GLfloat t = _wnd_rect.len[0] * 0.5f;
		for( long i = pos; i < pos+iMax; ++i )
		{
			Vect3f col = storage()->read_elem( i );
			_colors.push_back( col.x );
			_colors.push_back( col.y );
			_colors.push_back( col.z );
			_colors.push_back( col.x );
			_colors.push_back( col.y );
			_colors.push_back( col.z );
			_coords.push_back( _wnd_rect.center[0] + t );
			_coords.push_back( _wnd_rect.center[1] - 10 );
			_coords.push_back( 0 );
			_coords.push_back( _wnd_rect.center[0] + t );
			_coords.push_back( _wnd_rect.center[1] + 10 );
			_coords.push_back( 0 );
			t -= dt;
		}
	}

	glLineWidth( 1. );
	draw_arrays( &_coords[0], &_colors[0], _coords.size() / 3, GL_LINES );
}