#include "UserInterface.h"
#include <WindowsX.h>
#include "Visualizers\VisualizerListBuilder.h"
#include <iostream>

UserInterface::UserInterface( VisualizerListBuilder * vlb ) : _vlb(vlb)
{

}

UserInterface::~UserInterface()
{

}

void UserInterface::wm_char( const char key_code )
{
	_ch = key_code;
	if( 'p' == _ch )
	{
		_vlb->change_pause_state();
	}
}

bool UserInterface::is_wm_mouse( UINT message )
{
	return message==WM_MOUSEMOVE || message==WM_LBUTTONDOWN || message==WM_LBUTTONUP;
}

void UserInterface::wm_mouse( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_MOUSEMOVE:
			_mouse.x = GET_X_LPARAM(lParam);
			_mouse.y = GET_Y_LPARAM(lParam);
			break;
		default:
			;
	}
}

void UserInterface::wm_keydown( const LPARAM lParam, const WPARAM wParam )
{
	if( wParam==VK_CONTROL && !_ctrl )
	{
		_mouse.x0 = _mouse.x;
		_mouse.y0 = _mouse.y;
		_ctrl = true;
		//std::cout<<"1";
	}

	unsigned char key_index = wParam & 0xFF; // ensure that it is in 0-255 range
	// save pressed key
	_keys[key_index] = true;
}

void UserInterface::wm_keyup( const LPARAM lParam, const WPARAM wParam )
{
	//bool sent_first_time = true;((lParam & (1<<30)) == 0);

	if( wParam==VK_CONTROL /*&& sent_first_time*/ )
	{
		_ctrl = false;
		//std::cout<<"0";
	}

	unsigned char key_index = wParam & 0xFF; // ensure that it is in 0-255 range
	// save unpressed key
	_keys[key_index] = false;
}
