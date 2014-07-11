#include "ConverterIntToFloat.h"
#include "../StorageCursor.h"
#include <iostream>

ConverterIntToFloat::ConverterIntToFloat()
{
	_scale = 1.0f / ((int)1<<15);
}

void ConverterIntToFloat::process()
{
	cursor()->snapshort( input_storage() );

	long lag = cursor()->lag();

	if( verbose )
	{
		std::cout << "ConverterIntToFloat::process" << std::endl
				  << "  input count = " << cursor()->count() << std::endl
				  << "  input pos   = " << cursor()->position() << std::endl
				  << "  lag         = " << lag << std::endl;
	}
	if( lag > 0 )
	{
		input_storage()->read( cursor()->position(), lag, _int_buffer );
	
		if( verbose )
			std::cout << "  read size   = " << _int_buffer.size() << std::endl;

		cursor()->move_on( lag );

		if( verbose )
			std::cout << "  new input pos = " << cursor()->position() << std::endl;

		_float_buffer.clear();
		_float_buffer.reserve( _int_buffer.size() );
		for( auto it = _int_buffer.cbegin(); it != _int_buffer.cend(); ++it )
		{
			_float_buffer.push_back( (*it) * _scale );
		}

		if( verbose )
			std::cout << "  output count = " << output_storage()->count() << std::endl
					  << "  write size   = " << _float_buffer.size() << std::endl;

		output_storage()->append( _float_buffer );
		
		if( verbose )
			std::cout << "  new output count = " << output_storage()->count() << std::endl;

		if( cursor()->count() != output_storage()->count() )
			throw "Wrong conversion: input and output sizes differ";
	}
}