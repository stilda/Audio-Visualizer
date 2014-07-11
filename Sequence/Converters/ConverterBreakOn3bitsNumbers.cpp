#include "ConverterBreakOn3bitsNumbers.h"
#include <vector>
#include "..\StorageCursor.h"
#include "..\Storage.h"
#include <cassert>

float conv_to_bit( const float v )
{
	return v < 0.0f ? 0.0f : 1.0f;
}

void ConverterBreakOn3bitsNumbers::process()
{
	auto ostor = out_stor<Storage<float>*>(0);
	auto istor = in_stor<Storage<float>*>(0);
	std::vector<float> & ibuf = istor->tmp_buf();
	std::vector<float> & obuf = ostor->tmp_buf();
	auto icur = in_cursor(0);

	icur->set_sync( 3 );
	icur->snapshort( istor );

	long lag = icur->lag();
	if( lag == 0 )
		return;

	istor->read( icur->position(), lag, ibuf );
	obuf.clear();
	if( (int)obuf.capacity() < lag / 3 )
		obuf.reserve( lag / 3 );

	for( int i = 0; i < (int)ibuf.size(); i += 3 )
	{
		float v = 1 * conv_to_bit( ibuf[i] ) + 2 * conv_to_bit( ibuf[i+1] ) + 4 * conv_to_bit( ibuf[i+2] );
		obuf.push_back( v );
	}

	ostor->append( obuf );
	icur->move_on( lag );
}