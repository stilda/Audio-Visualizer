#include "ConverterFloatToMeanFloat.h"
#include "../StorageCursor.h"
#include <iostream>

ConverterFloatToMeanFloat::ConverterFloatToMeanFloat() 
	: _window_size(1)
{
}

void ConverterFloatToMeanFloat::SetWindowSize( const int window_size )
{
	_window_size = window_size;
	cursor()->set_sync( _window_size );
}

void ConverterFloatToMeanFloat::process()
{
	cursor()->snapshort( input_storage() );

	long lag = cursor()->lag();

	if( verbose )
	{
		std::cout << "  ConverterFloatToMeanFloat::process" << std::endl
				  << "    input count = " << cursor()->count() << std::endl
				  << "    input pos   = " << cursor()->position() << std::endl
				  << "    lag         = " << lag << std::endl;
	}
	if( lag > 0 )
	{
		input_storage()->read( cursor()->position(), lag, _input_buffer );
	
		if( verbose )
			std::cout << "    read size   = " << _input_buffer.size() << std::endl;

		cursor()->move_on( lag );

		if( verbose )
			std::cout << "    new input pos = " << cursor()->position() << std::endl;

		_output_buffer.clear();
		_output_buffer.reserve( _output_buffer.size() );
		int iSampleWithinWindow = 0;
		float mean = 0;
		for( auto it = _input_buffer.cbegin(); it != _input_buffer.cend(); ++it )
		{
			mean += *it;
			iSampleWithinWindow++;

			if( iSampleWithinWindow == _window_size )
			{
				_output_buffer.push_back( mean );
				mean = 0;
				iSampleWithinWindow = 0;
			}
		}

		if( verbose )
			std::cout << "    output count = " << output_storage()->count() << std::endl
					  << "    write size   = " << _output_buffer.size() << std::endl;

		output_storage()->append( _output_buffer );

		if( verbose )
			std::cout << "    new output count = " << output_storage()->count() << std::endl;
	}
}
