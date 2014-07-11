#include "ConverterFloatToColors.h"
#include "../StorageCursor.h"
#include "Geometry\Vector3.h"

ConverterFloatToColors::ConverterFloatToColors()
{
}

void ConverterFloatToColors::process()
{
	cursor()->snapshort( input_storage() );
	cursor()->set_sync( 3 );
	long lag = cursor()->lag();
	if( lag == 0 )
		return;

	input_storage()->read( cursor()->position(), lag, _i_buf );
	cursor()->move_on( lag );
	_o_buf.clear();
	_o_buf.reserve( _i_buf.size() );

	using namespace Vector3;
	for( auto it = _i_buf.cbegin(); it != _i_buf.cend(); it += 3 )
	{
		Vect3f color = linear_transform_to_01( Vect3f( *it, *(it+1), *(it+2) ) );
		_o_buf.push_back( color );
	}

	output_storage()->append( _o_buf );
}
