#include "ConverterFloatToFloatHistogram.h"
#include "..\StorageCursor.h"
#include <cmath>

void ConverterFloatToFloatHistogram::process()
{
	cursor()->snapshort( input_storage() );
	cursor()->set_sync( _wnd_size );
	long lag = cursor()->lag();
	if( lag == 0 )
		return;

	const float interval_width = ( _range_max - _range_min ) / _intervals_num;

	input_storage()->read( cursor()->position(), lag, _ib );
	for( long iBlockStart = 0; iBlockStart < lag; iBlockStart += _wnd_size )
	{
		std::vector<float> hist(_intervals_num,0);
		for( int i = iBlockStart; i < iBlockStart + _wnd_size; ++i )
		{
			int iInterval = int( (_ib[i] - _range_min) / interval_width );
			if( iInterval >=0 && iInterval < _intervals_num )
			{
				hist[iInterval] += 1;
			}
		}
		output_storage()->append_elem( hist );
	}
	cursor()->move_on( lag );
}