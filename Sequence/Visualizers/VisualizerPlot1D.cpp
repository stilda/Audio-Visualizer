#include "VisualizerPlot1D.h"
#include "GlUtils\glDrawUtils.h"
#include "GlUtils\GlTextUtils.h"

VisualizerPlot1D::VisualizerPlot1D()
{

}

VisualizerPlot1D::~VisualizerPlot1D()
{

}

void VisualizerPlot1D::SetParams( const Params & params )
{
	_params = params;
}

void VisualizerPlot1D::draw()
{
	glColor3f( _params.color.x, _params.color.y, _params.color.z );
	draw_frame2D( _wnd_rect );
	draw_axes2D( _wnd_rect );
	glRasterPos2fv( _wnd_rect.minpt );
	glPrint( _caption );

	if( !cursor()->on_pause() )
	{
		_coords.clear();

		cursor()->snapshort( storage() );
		if( cursor()->count() < 1 ) 
			return;

		std::vector<float> data = storage()->read_elem( cursor()->count()-1 );
		if( _coords.capacity() == 0 )
		{
			AllocGlBuffers( data.size() );
		}

		GLfloat dt = _wnd_rect.len[0] / data.size();
		GLfloat t = _wnd_rect.len[0] * 0.5f;
		for( long i = 0; i < (long)data.size(); ++i )
		{
			_coords.push_back( _wnd_rect.center[0] + t );
			_coords.push_back( _wnd_rect.center[1] + _params.scale * data[i] );
			_coords.push_back( 0 );
			t -= dt;
		}
	}

	glLineWidth( 2. );
	glColor3f( _params.color.x, _params.color.y, _params.color.z );
	draw_arrays( &_coords[0], _coords.size() / 3 );
}