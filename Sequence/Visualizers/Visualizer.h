#pragma once

#include <Windows.h>
#include <gl/gl.h>
#include "..\Storage.h"
#include "..\StorageCursor.h"
#include "geometry\GeometryStructures2D.h"
#include <vector>
#include <cassert>

class IVisualizer
{
protected:
	GLfloat _desired_height;
	GLfloat _desired_width;
	Rect2D<GLfloat> _wnd_rect;
	GLfloat _w_pixel;
	char _caption[256];
public:
	virtual ~IVisualizer() {};
	
	virtual void draw() = 0;

	void SetDesiredHeight( const GLfloat height )
	{
		_desired_height = height;
	}
	
	void SetDesiredWidth( const GLfloat width )
	{
		_desired_width = width;
	}

	float GetDesiredHeight() { return _desired_height; }
	float GetDesiredWidth() { return _desired_width; }

	void SetPlotRange( const Rect2D<GLfloat> & wnd_rect )
	{
		_wnd_rect = wnd_rect;
	}

	void SetPixelWidth( const GLfloat wpixel )
	{
		_w_pixel = wpixel;
	}

	void SetCaption( const char caption[256] )
	{
		memset( _caption, 0, 256 );
		memcpy( _caption, caption, strlen(caption)+1 );
	}

};

class PlotData
{
public:
	std::vector<GLfloat> _coords;
	std::vector<GLfloat> _colors;
};

class BasicPlotParameters
{
public:
	float scale;
	float plot_color[3];
	float frame_color[3];
	int max_plot_points;
};

template<typename E>
class Visualizer : public IVisualizer
{
protected:
	std::vector<GLfloat> _coords;
	std::vector<GLfloat> _colors;
	std::vector<Storage<E>*> _storages;
	std::vector<StorageCursor*> _cursors;
	std::vector<PlotData> _plot_datas;
	std::vector<BasicPlotParameters> _plot_params;
	std::vector<int> _actual_points_numbers;
public:

	void AllocGlBuffers( const int nSamples ) 
	{
		_coords.clear();
		_coords.reserve( 3*nSamples );
		_colors.clear();
		_colors.reserve( 3*nSamples );
	}

	Visualizer() {}
	~Visualizer() {}
	
	Storage<E> * storage() { return _storages.empty() ? nullptr : _storages.front(); }
	StorageCursor * cursor() { return _cursors.empty() ? nullptr : _cursors.front(); }
	int storages_count() { return (int)_storages.size(); }
	BasicPlotParameters BasicPlotParams() { return _plot_params.empty() ? nullptr : _plot_params.front(); }
	
	void AddStorageAndCursor( Storage<E> * storage, StorageCursor * cursor )
	{
		assert( storage );
		assert( cursor );
		_storages.push_back( storage );
		_cursors.push_back( cursor );
	}

	void AddParameters( const BasicPlotParameters & params )
	{
		_plot_params.push_back( params );
	}

};