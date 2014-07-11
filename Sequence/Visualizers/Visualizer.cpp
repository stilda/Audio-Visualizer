/*#include "Visualizer.h"
#include "..\StorageCursor.h"
#include <gl/gl.h>

template<typename E>
Visualizer<E>::Visualizer( const int nSamples ) :
	_coords(0),
	_colors(0),
	_count(0),
	_w(1),
	_h(1),
	_w_pixel(1)
{
	_coords = new GLfloat[nSamples*3];
	_colors = new GLfloat[nSamples*3];
	_count = nSamples;

	for( GLfloat * p = _coords; p < &_coords[_count*3]; ++p )
		*p = 0.0f;
	for( GLfloat * p = _colors; p < &_colors[_count*3]; ++p )
		*p = 0.0f;
}

template<typename E>
Visualizer<E>::~Visualizer()
{
	_count = 0;
	if( _coords )
		delete []_coords;
	_coords = 0;
	if( _colors )
		delete []_colors;
	_colors = 0;
}
template<typename E>
void Visualizer<E>::SetPlotRange( const GLfloat width, const GLfloat height )
{
	_w = width;
	_h = height;
}

template<typename E>
void Visualizer<E>::SetPixelWidth( const GLfloat wpixel )
{
	_w_pixel = wpixel;
}*/
/*
void Visualizer::process( StorageCursor * storage_cursor )
{
}

void Visualizer::keyboard_state( char ch, const bool keys[256] )
{
}
*/