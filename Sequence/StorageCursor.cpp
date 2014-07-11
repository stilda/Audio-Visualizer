#include "StorageCursor.h"

long StorageCursor::count()
{
	return sync(_total_in_storage);
}

long StorageCursor::position()
{
	return sync(_pos);
}

void StorageCursor::set_sync( const long step )
{
	_step = step;
}

long StorageCursor::lag()
{
	return sync( _total_in_storage - _pos );
}

void StorageCursor::move_on( const long pos_shift )
{
	_pos += pos_shift;
}

void StorageCursor::move_to( const long pos )
{
	_pos = pos;
}