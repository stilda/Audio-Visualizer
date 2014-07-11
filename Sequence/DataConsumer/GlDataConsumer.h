#pragma once

#include "DataConsumer.h"
#include <Windows.h>
#include <memory>
#include <vector>
#include "..\Visualizers\Visualizer.h"

class ConverterNode;
class UserInterface;

class GlDataConsumer : public IDataConsumer
{
	HWND _hWnd;
	HDC _hDC;
	HGLRC _hGLRC;
	UserInterface * _ui;
	std::vector<IVisualizer*> _visualizers;

	void CreateAndShowWnd();
	void LayoutVisualizers();
	int SetWindowPixelFormat();
	bool ProcessMessageQue();
	void display( void );
	void resize( int width, int height );
public:
	GlDataConsumer( ConverterNode * converter_node, UserInterface * ui );
	void operator()() override;
	void AddVizualizer( IVisualizer * v );

	void wm_create( HWND hWnd );
	void wm_destroy();
	void wm_paint();
	void wm_size( int width, int height );
	UserInterface * ui() { return _ui; }
};

