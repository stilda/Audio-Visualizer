#include "VisualizerMultiDimSignal.h"
#include "..\StorageCursor.h"
#include "..\Storage.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "GlUtils\glDrawUtils.h"
#include "GlUtils\GlTextUtils.h"
#include <cassert>

VisualizerMultiDimSignal::VisualizerMultiDimSignal()
{
	_dim = 1;
}

VisualizerMultiDimSignal::~VisualizerMultiDimSignal()
{
}

void VisualizerMultiDimSignal::draw()
{
	static GLfloat a(0), b(0), c(0);
	a += 0.1f;
	b += 0.3f;
	c += -0.015f;

	// Support only one plot. TODO: make it support several plots in one window.
	assert( _dim == (int)_storages.size() );
	assert( _dim >= 2 );

	assert( _storages.size() == _cursors.size() );
	assert( _storages.size() == _plot_params.size() );

	glColor3fv( _plot_params[0].frame_color );
	draw_frame2D( _wnd_rect );
	glRasterPos2fv( _wnd_rect.minpt );
	glPrint( _caption );

	if( !cursor()->on_pause() ) // use first cursor for a while. TODO.
	{
		if( 0 == _coords.capacity() )
		{
			AllocGlBuffers( _plot_params[0].max_plot_points * (int)_storages.size() );
		}

		_coords.clear();

		for( int i = 0; i < (int)_storages.size(); ++i )
		{
			_cursors[i]->snapshort( _storages[i] );
			if( _cursors[i]->position() != _cursors[0]->position() )
				throw "VisualizerMultiDimSignal::draw() : positions are not equal";
			//if( _cursors[i]->count() != _cursors[0]->count() )
			//	throw "VisualizerMultiDimSignal::draw() : counts are not equal";
		}

		long min_count = _cursors[0]->count();
		for( int i = 1; i < (int)_cursors.size(); ++i )
		{
			min_count = min( min_count, _cursors[i]->count() );
		}
		long iMax = min( min_count, _plot_params[0].max_plot_points );
		if( 0 == iMax )
			return;

		for( long pos = min_count - iMax; pos < min_count; ++pos )
		{
			_coords.push_back( _plot_params[0].scale * _storages[0]->read_elem( pos ) );
			_coords.push_back( _plot_params[1].scale * _storages[1]->read_elem( pos ) );
			if( _storages.size() == 3 )
				_coords.push_back( _plot_params[2].scale * _storages[2]->read_elem( pos ) );
			else
				_coords.push_back( 0 );
		}
	}

	glPushMatrix();
	glTranslatef( _wnd_rect.center[0], _wnd_rect.center[1], 0 );

	if( _storages.size() == 3 )
	{	
		glRotatef( a, 1,0,0 );
		glRotatef( b, 0,1,0 );
		glRotatef( c, 0,0,1 );
	}
	glColor3fv( _plot_params[0].frame_color );
	draw_axes3D( 100, 100, 100 );
	
	glLineWidth( 1. );
	glColor3fv( _plot_params[0].plot_color );
	draw_arrays( &_coords[0], (GLsizei)_coords.size()/3 );
	
	glLineWidth( 2.0f );
	glBegin( GL_LINE_LOOP );
	for( float angle = 0; angle < 6.28f; angle += 0.06f )
	{
		glVertex3f( 100*sin(angle), 100* cos(angle), 0 );
	}
	glEnd();

	glPopMatrix();
	
}