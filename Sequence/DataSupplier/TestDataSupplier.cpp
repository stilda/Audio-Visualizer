#include "TestDataSupplier.h"
#include <vector>
#include <boost\thread\thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

TestDataSupplier::TestDataSupplier( Storage<__int16> * storage )
	: IDataSupplier(storage)
{
}

void TestDataSupplier::run()
{
	std::vector<__int16> recorded_block;
	recorded_block.resize( 400 );
	static float fArg = 0;
	while(true)
	{
		for( auto it = recorded_block.begin(); it != recorded_block.end(); ++it )
		{
			*it = __int16( 20000 * sin(fArg) * sin(fArg/8) + 500 * sin(3*fArg) );
			fArg += 0.1f;
		}
		_storage->append( recorded_block );
		boost::this_thread::sleep( boost::posix_time::milliseconds(20) );
	}

	//static int nSample = 0;
	//while(true)
	//{
	//	for( auto it = recorded_block.begin(); it != recorded_block.end(); ++it )
	//	{
	//		*it = (nSample % 99 == 0)||(nSample % 99 == 0) ? 20000 : 0;
	//		nSample++;
	//	}
	//	_storage->append( recorded_block );
	//	boost::this_thread::sleep( boost::posix_time::milliseconds(20) );
	//}
}