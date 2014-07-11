#include "VisualizerColorMatrix.h"
#include "..\StorageCursor.h"
#include "..\Storage.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "GlUtils\glDrawUtils.h"
#include "GlUtils\GlTextUtils.h"
#include "Geometry\Vector3.h"
#include "boost\numeric\ublas\matrix.hpp"

using namespace boost::numeric::ublas;

void VisualizerColorMatrix::SetParams( const Params & params )
{
	_params = params;
}

void VisualizerColorMatrix::draw()
{
	using namespace Vector3;
	const float scale = 2;

	glColor3f( _params.color.x, _params.color.y, _params.color.z );
	draw_frame2D( _wnd_rect );
	glRasterPos2fv( _wnd_rect.minpt );
	glPrint( _caption );

	if( !cursor()->on_pause() )
	{
		_coords.clear();
		_colors.clear();

		cursor()->snapshort( storage() );
		if( cursor()->count() < 1 )
			return;
	
		matrix<Vect3f> matr = storage()->read_elem( cursor()->count()-1 );
		if( _coords.capacity() == 0 )
		{
			AllocGlBuffers( matr.size1() * matr.size2() );
		}

		for( int i = 0; i < (int)matr.size1(); ++i )
		{
			for( int j = 0; j < (int)matr.size2(); ++j )
			{
				Vect3f col = matr(i,j);
				_coords.push_back( _wnd_rect.minpt[0] + i*scale );
				_coords.push_back( _wnd_rect.minpt[1] + j*scale );
				_coords.push_back( 0 );
				_colors.push_back( col.x );
				_colors.push_back( col.y );
				_colors.push_back( col.z );
			}
		}
	}

	glPointSize( scale );
	draw_arrays( &_coords[0], &_colors[0], _coords.size() / 3, GL_POINTS );
}