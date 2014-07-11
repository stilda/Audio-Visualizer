#include "ConverterFloatToMovingMean.h"
#include "..\StorageCursor.h"
#include <iostream>

ConverterFloatToMovingMean::ConverterFloatToMovingMean()
{
	_window_len = 10;
	_sum = 0;
}

void ConverterFloatToMovingMean::process()
{
	cursor()->snapshort( input_storage() );
	//cursor()->set_sync( _window_len );

	long lag = cursor()->lag();

	if( lag > 0 )
	{
		input_storage()->read( cursor()->position(), lag, _i_buf );
		cursor()->move_on( lag );
		_o_buf.clear();
		_o_buf.reserve( _i_buf.size() );
		for( auto it = _i_buf.cbegin()+1; it != _i_buf.cend(); ++it )
		{
			_sum += (*it);
			_o_buf.push_back( _sum );
		}
		std::cout << "_sum = " << _sum << std::endl;
		output_storage()->append( _o_buf );
	}
}