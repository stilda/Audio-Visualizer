#pragma once

#include "Storage.h"

class StorageCursor
{
protected:
	long _pos;
	long _step;
	long _total_in_storage;
	bool _on_pause;
public:
	StorageCursor() : _pos(0), _step(1), _total_in_storage(0), _on_pause(false) {}
	long count();
	long position();
	long lag();
	void move_on( const long pos_shift );
	void move_to( const long pos );
	void set_sync( const long step );
	void change_pause_state() { _on_pause = !_on_pause; }
	bool on_pause() { return _on_pause; }
	inline long sync( const long a ) { return a - a%_step; }
	template <typename E> void snapshort( Storage<E> * storage );
};

template <typename E>
void StorageCursor::snapshort( Storage<E> * storage )
{
	if( !_on_pause )
	{
		_total_in_storage = storage->count();
	}
	else
	{
		_total_in_storage = _total_in_storage;
	}
}
