#include "ConverterFloatToFloatDerivate.h"
#include "..\StorageCursor.h"

void ConverterFloatToFloatDerivate::process() 
{
	cursor()->snapshort( input_storage() );

	long lag = cursor()->lag();
	if( lag < 2 )
		return;

	input_storage()->read( cursor()->position(), lag, _ib );

	_ob.clear();
	_ob.reserve( _ib.size() );
	for( int i = 0; i < lag-1; ++i )
	{
		_ob.push_back( 0.5f*(_ib[i+1] - _ib[i]) );
	}
	output_storage()->append( _ob );

	cursor()->move_on( lag-1 );
}
