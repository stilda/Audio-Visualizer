#include "ConverterM1P1ToMeanStatistics.h"
#include "..\StorageCursor.h"

void ConverterM1P1ToMeanStatistics::process()
{
	cursor()->snapshort( input_storage() );
	cursor()->set_sync( _wnd_size );
	long lag = cursor()->lag();
	if( lag == 0 )
		return;

	input_storage()->read( cursor()->position(), lag, _ib );
	_ob.clear();
	for( long iBlockStart = 0; iBlockStart < lag; iBlockStart+=_wnd_size )
	{
		int nBalance(0);
		for( int i = iBlockStart; i < iBlockStart+_wnd_size; ++i )
		{
			if( _ib[i] > 0 )
			{
				nBalance++;
			}
			else
			{
				nBalance--;
			}
		}
		_ob.push_back( (float)nBalance );
	}
	output_storage()->append( _ob );
	cursor()->move_on( lag );
}