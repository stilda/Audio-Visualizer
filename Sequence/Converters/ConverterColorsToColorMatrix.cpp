#include "ConverterColorsToColorMatrix.h"
#include "../StorageCursor.h"

ConverterColorsToColorMatrix::ConverterColorsToColorMatrix()
{
	_ix = 10;
	_iy = 10;
}

void ConverterColorsToColorMatrix::process()
{
	cursor()->snapshort( input_storage() );
	cursor()->set_sync( _ix * _iy );
	long lag = cursor()->lag();
	if( lag == 0 )
		return;

	input_storage()->read( cursor()->position(), lag, _i_buf );
	cursor()->move_on( lag );
	_o_buf.clear();
	_o_buf.reserve( lag / (_ix*_iy) );

	using namespace Vector3;
	for( auto it = _i_buf.cbegin(); it != _i_buf.cend(); it += _ix*_iy )
	{
		matrix<Vect3f> matr(_ix,_iy);
		int i(0), j(0);
		while( true )
		{
			matr(i,j) = *(it+j*_ix+i);
			++i;
			if( i == _ix )
			{
				++j;
				i = 0;
			}
			if( j == _iy )
			{
				break;
			}
		}
		_o_buf.push_back( matr );
	}

	output_storage()->append( _o_buf );
}