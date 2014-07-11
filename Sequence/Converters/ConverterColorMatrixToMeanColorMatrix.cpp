#include "ConverterColorMatrixToMeanColorMatrix.h"
#include "../StorageCursor.h"

void ConverterColorMatrixToMeanColorMatrix::process()
{
	cursor()->snapshort( input_storage() );
	cursor()->set_sync( _ws );
	long lag = cursor()->lag();
	if( lag == 0 )
		return;

	for( int pos = cursor()->position(); pos < cursor()->count(); pos += _ws )
	{
		matrix<Vect3f> meanMatr( 40, 40, Vect3f(0,0,0) );
		for( int iMatr = 0; iMatr < _ws; iMatr++ )
		{
			meanMatr += input_storage()->read_elem( pos + iMatr );
		}

		meanMatr *= 1.0f / _ws;

		output_storage()->append_elem( meanMatr );
	}

	cursor()->move_on( lag );
}