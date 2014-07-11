#include "ConverterMixAllToOne.h"
#include <vector>
#include "..\StorageCursor.h"
#include "..\Storage.h"
#include <cassert>

void ConverterMixAllToOne::process()
{
	assert( _input_cursors.size() == _input_storages.size() );

	int count = in_count();

	for( int i = 0; i < count; ++i )
	{
		in_cursor(i)->snapshort( in_stor<Storage<float>*>(i) );
	}

	long p0 = in_cursor(0)->position();
	long c0 = in_cursor(0)->count();
	for( int i = 1; i < count; ++i )
	{
		long pi = in_cursor(i)->position();
		long ci = in_cursor(i)->count();
		if( p0 != pi ) 
			throw "ConverterMixAllToOne::process() : p0 != pi";
		//if( c0 != ci ) 
		//	throw "ConverterMixAllToOne::process() : c0 != ci";
	}

	long lag = in_cursor(0)->lag();
	for( int i = 1; i < count; ++i )
	{
		lag = std::min( lag, in_cursor(i)->lag() );
	}
	if( lag == 0 )
		return;

	for( int i = 0; i < count; ++i )
	{
		auto stor = in_stor<Storage<float>*>(i);
		stor->read( in_cursor(i)->position(), lag, stor->tmp_buf() );
	}

	auto ostor = out_stor<Storage<float>*>(0);
	ostor->tmp_buf().clear();
	if( (long)ostor->tmp_buf().capacity() < 3 * lag )
	{
		ostor->tmp_buf().reserve( 3 * lag );
	}
	
	for( long i = 0; i < lag; ++i )
	{
		for( int j = 0; j < count; ++j )
		{
			auto istor = in_stor<Storage<float>*>(j);
			long s = istor->tmp_buf().size();
			assert( i < s );
			ostor->tmp_buf().push_back( istor->tmp_buf()[i] );
		}
	}

	ostor->append( ostor->tmp_buf() );

	for( int i = 0; i < count; ++i )
	{
		in_cursor(i)->move_on( lag );
	}
}