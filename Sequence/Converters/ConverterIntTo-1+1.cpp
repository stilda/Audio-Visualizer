#include "ConverterIntTo-1+1.h"
#include "..\StorageCursor.h"
#include <iostream>

ConverterIntToM1P1::ConverterIntToM1P1()
{
}

void ConverterIntToM1P1::process()
{
	cursor()->snapshort( input_storage() );

	long lag = cursor()->lag();

	if( verbose )
	{
		std::cout << "ConverterIntTo-1+1::process" << std::endl
				  << "  input count = " << cursor()->count() << std::endl
				  << "  input pos   = " << cursor()->position() << std::endl
				  << "  lag         = " << lag << std::endl;
	}
	if( lag > 1 )
	{
		input_storage()->read( cursor()->position(), lag, _int_buffer );
	
		if( verbose )
			std::cout << "  read size   = " << _int_buffer.size() << std::endl;

		cursor()->move_on( lag );

		if( verbose )
			std::cout << "  new input pos = " << cursor()->position() << std::endl;
		
		_float_buffer.clear();
		_float_buffer.reserve( _int_buffer.size() );
		for( auto it = _int_buffer.cbegin()+1; it != _int_buffer.cend(); ++it )
		{
			if( (*it) > *(it-1) )
				_float_buffer.push_back( 1.0f );
			else 
			if( (*it) < *(it-1) )
				_float_buffer.push_back( -1.0f );
			//if( (*it) > 0 )
			//	_float_buffer.push_back( 1.0f );
			//else 
			//if( (*it) < 0 )
			//	_float_buffer.push_back( -1.0f );
		}

		if( verbose )
			std::cout << "  output count = " << output_storage()->count() << std::endl
					  << "  write size   = " << _float_buffer.size() << std::endl;

		output_storage()->append( _float_buffer );
		
		if( verbose )
			std::cout << "  new output count = " << output_storage()->count() << std::endl;
	}
}