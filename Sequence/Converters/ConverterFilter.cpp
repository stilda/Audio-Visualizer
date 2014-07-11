#include "ConverterFilter.h"
#include "..\StorageCursor.h"

ConverterFilter::ConverterFilter() 
{
	_compensation = true;
}

void ConverterFilter::SetFilter( const std::vector<float> & filter_coefs )
{
	_filter_coefs = filter_coefs;
}

void ConverterFilter::process()
{
	const int filter_size = _filter_coefs.size();
	cursor()->snapshort( input_storage() );
	long lag = cursor()->lag();
	if( lag < filter_size )
		return;

	input_storage()->read( cursor()->position(), lag, _ib );
	
	_ob.clear();
	_ob.reserve( lag );
	for( int i = 0; i < lag - filter_size + 1; ++i )
	{
		float sum = 0;
		for( int j = 0; j < filter_size; ++j )
		{
			sum += _ib[i+j] * _filter_coefs[j];
		}
		_ob.push_back( sum );
	}
	if( _compensation && output_storage()->count() == 0 )
	{
		std::vector<float> edge_elements( filter_size/2, 0 );
		output_storage()->append( edge_elements );
	}
	output_storage()->append( _ob );
		
	cursor()->move_on( lag - filter_size + 1 );
}
