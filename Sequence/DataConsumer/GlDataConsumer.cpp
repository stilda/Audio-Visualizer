#include <boost\date_time\posix_time\posix_time.hpp>
#include <boost\thread\thread.hpp>
#include "DataConsumer.h"
#include "GlDataConsumer.h"
#include "..\UserInterface.h"
#include "..\converters\ConverterTree.h"
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLAUX.H>
#include "GlUtils\GlTextUtils.h"
#include <iostream>
#include "..\Visualizers\Visualizer.h"
#include "..\Visualizers\VisualizerPlainSignal.h"
#include "Geometry\GeometryStructures2D.h"

GlDataConsumer::GlDataConsumer( ConverterNode * converter_node, UserInterface * ui )
	: IDataConsumer(converter_node),
	  _ui(ui)
{
}

LRESULT CALLBACK WindowFunc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	GlDataConsumer * consumer = nullptr;
	if( msg != WM_CREATE )
		consumer = reinterpret_cast<GlDataConsumer*>( GetWindowLongPtr(hWnd,GWLP_USERDATA) );

	// Handle mouse events
	if( consumer && consumer->ui() && consumer->ui()->is_wm_mouse(msg) )
	{
		consumer->ui()->wm_mouse( msg, wParam, lParam );
		return 0;
	}

	PAINTSTRUCT ps;
	switch(msg)
	{
		case WM_CREATE:
		{
			// save data
			CREATESTRUCT * pStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG_PTR)pStruct->lpCreateParams );

			consumer = reinterpret_cast<GlDataConsumer*>(pStruct->lpCreateParams);
			consumer->wm_create( hWnd );
			break;
		}

		case WM_DESTROY:
			consumer->wm_destroy();
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			BeginPaint( hWnd, &ps );
			consumer->wm_paint();
			EndPaint( hWnd, &ps );
			break;

		case WM_SIZE:
			consumer->wm_size( LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_CHAR:
			if( consumer->ui() ) 
			{
				consumer->ui()->wm_char( (char)wParam );
			}

		case WM_KEYDOWN:
			if( consumer->ui() ) 
			{
				consumer->ui()->wm_keydown( lParam, wParam );
			}

		case WM_KEYUP:
			if( consumer->ui() ) 
			{
				consumer->ui()->wm_keyup( lParam, wParam );
			}

		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

void GlDataConsumer::CreateAndShowWnd()
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong( GetConsoleWindow(), GWL_HINSTANCE );

	// Reg window class
	WNDCLASS wcl;
	wcl.hInstance = hInstance;
	wcl.lpszClassName = L"GlauxDataConsumerClass";
	wcl.lpfnWndProc = WindowFunc;
	wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcl.hIcon = NULL;
	wcl.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	RegisterClass( &wcl );

	// Create window
	HWND hWnd = CreateWindow( 
		L"GlauxDataConsumerClass", L"wau", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
		10, 10, 600, 600, HWND_DESKTOP, NULL, hInstance, this );

	// Show window
	ShowWindow( hWnd, SW_SHOWNORMAL );
	UpdateWindow( hWnd );
}

void GlDataConsumer::wm_create( HWND hWnd )
{
	_hWnd = hWnd;
	_hDC = GetDC( hWnd );
	SetWindowPixelFormat();
	_hGLRC = wglCreateContext( _hDC );
	wglMakeCurrent( _hDC, _hGLRC );
	BuildFont( _hDC );

	float pos[4] = {3,3,3,1};
	float dir[3] = {-1,-1,-1};

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glLightfv(GL_LIGHT0, GL_POSITION, pos);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
}

void GlDataConsumer::wm_destroy()
{
	if( _hGLRC )
	{
		KillFont();
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( _hGLRC );
	}
	ReleaseDC( _hWnd, _hDC );
}

void GlDataConsumer::wm_paint()
{
	display();
}

void GlDataConsumer::wm_size( int width, int height )
{
	resize( width, height );
}

void GlDataConsumer::resize(int width,int height)
{
	int	depth = width > height ? width : height;
   glViewport(0,0,width,height);
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho(-width/2,width/2, -height/2,height/2, -depth/2,depth/2);
   gluLookAt( 0,0,5, 0,0,0, 0,1,0 );
   glMatrixMode( GL_MODELVIEW );

   LayoutVisualizers();
}

void GlDataConsumer::display(void)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for( auto it = _visualizers.begin(); it != _visualizers.end(); ++it )
	{
		(*it)->draw();
	}

	glFinish();
	SwapBuffers( wglGetCurrentDC() );
}

int GlDataConsumer::SetWindowPixelFormat()
{
	int m_GLPixelIndex;
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize       = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion    = 1;
	pfd.dwFlags   = PFD_DRAW_TO_WINDOW |
					PFD_SUPPORT_OPENGL |
					PFD_DOUBLEBUFFER;
	pfd.iPixelType     = PFD_TYPE_RGBA;
	pfd.cColorBits     = 32;
	pfd.cRedBits       = 8;
	pfd.cRedShift      = 16;
	pfd.cGreenBits     = 8;
	pfd.cGreenShift    = 8;
	pfd.cBlueBits      = 8;
	pfd.cBlueShift     = 0;
	pfd.cAlphaBits     = 0;
	pfd.cAlphaShift    = 0;
	pfd.cAccumBits     = 64;
	pfd.cAccumRedBits  = 16;
	pfd.cAccumGreenBits   = 16;
	pfd.cAccumBlueBits    = 16;
	pfd.cAccumAlphaBits   = 0;
	pfd.cDepthBits        = 32;
	pfd.cStencilBits      = 8;
	pfd.cAuxBuffers       = 0;
	pfd.iLayerType        = PFD_MAIN_PLANE;
	pfd.bReserved         = 0;
	pfd.dwLayerMask       = 0;
	pfd.dwVisibleMask     = 0;
	pfd.dwDamageMask      = 0;
	m_GLPixelIndex = ChoosePixelFormat( _hDC, &pfd );
	if(m_GLPixelIndex==0) // Let's choose a default index.
	{
		m_GLPixelIndex = 1;
		if( DescribePixelFormat(_hDC,m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pfd) == 0 )
			return 0;
	}
	if (SetPixelFormat( _hDC, m_GLPixelIndex, &pfd)==FALSE)
		return 0;
	return 1;
}

bool  GlDataConsumer::ProcessMessageQue()
{
	MSG msg;
	while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if( GetMessage( &msg, NULL, 0, 0 ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			return false;
		}
	}
	return true;
}

void GlDataConsumer::AddVizualizer( IVisualizer * v )
{
	_visualizers.push_back( v );
}

void GlDataConsumer::LayoutVisualizers()
{
	RECT rect;
	GetClientRect( _hWnd, &rect );
	long w = (rect.right-rect.left);
	long h = (-rect.top+rect.bottom);
	
	int n = 0;
	float total_height = 0;
	for( auto it = _visualizers.begin(); it != _visualizers.end(); ++it )
	{
		GLfloat width = (*it)->GetDesiredWidth();
		GLfloat height = (*it)->GetDesiredHeight();
		(*it)->SetPlotRange( Rect2D<GLfloat>(-w/2+10+width/2,h/2-10-total_height-height/2,width,height) );
		(*it)->SetPixelWidth( 1 );

		total_height += height + 4;
		n++;
	}
}

void GlDataConsumer::operator()()
{
	CreateAndShowWnd();
	bool window_shown = true;

	LayoutVisualizers();

	// Timing
	LARGE_INTEGER freq, t0, t1, t2;
	QueryPerformanceFrequency( &freq );

	// Enter main loop
	while( true )
	{
		if( window_shown )
			window_shown = ProcessMessageQue();
		QueryPerformanceCounter( &t0 );
		_converter_node->process();
		QueryPerformanceCounter( &t1 );
		if( window_shown )
			display();
		QueryPerformanceCounter( &t2 );
		//std::cout << "Process TIME = " << (t1.QuadPart-t0.QuadPart)*1000.0f / freq.QuadPart 
		//		  << ", Draw TIME = " << (t2.QuadPart-t1.QuadPart)*1000.0f / freq.QuadPart << std::endl;
		//std::cout << "FREQ = " << freq.QuadPart << std::endl;
		//boost::this_thread::sleep( boost::posix_time::milliseconds(20) );
	}
}