#pragma once

#include <Windows.h>
#include <functional>

class VisualizerListBuilder;

class UserInterface
{
public:
	typedef bool KeysTable[256];

protected:
	VisualizerListBuilder * _vlb;
	bool _ctrl;
	bool _shift;
	char _ch;
	KeysTable _keys;
	struct
	{
		int x;
		int y;
		int x0;
		int y0;
	} _mouse;
public:
	std::function<void(unsigned char)> KeyDownCallback;
	std::function<void(unsigned char)> KeyUpCallback;
public:
	UserInterface( VisualizerListBuilder * vlb );
	~UserInterface();


	void wm_char( const char key_code );
	void wm_keydown( const LPARAM lParam, const WPARAM wParam );
	void wm_keyup( const LPARAM lParam, const WPARAM wParam );

	bool is_wm_mouse( UINT message );
	void wm_mouse( UINT message, WPARAM wParam, LPARAM lParam );
	const KeysTable & keys() const { return _keys; }
};
