#include <boost\date_time\posix_time\posix_time.hpp>
#include <boost\thread\thread.hpp>
#include "DataConsumer.h"
#include "TestDataConsumer.h"
#include "..\converters\ConverterTree.h"
#include <Windows.h>
#include <iostream>

TestDataConsumer::TestDataConsumer( ConverterNode * converter_node )
	: IDataConsumer(converter_node)
{
}

void TestDataConsumer::operator()()
{
	LARGE_INTEGER freq, t0, t1;
	QueryPerformanceFrequency( &freq );
	while( true )
	{
		QueryPerformanceCounter( &t0 );
		_converter_node->process();
		QueryPerformanceCounter( &t1 );
		std::cout << "TIME = " << (t1.QuadPart-t0.QuadPart)*1000.0f / freq.QuadPart << std::endl;
		//std::cout << "FREQ = " << freq.QuadPart << std::endl;
		boost::this_thread::sleep( boost::posix_time::milliseconds(20) );
	}
}