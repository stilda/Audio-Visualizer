#pragma once

#include "Converter.h"
#include "..\StorageCursor.h"
#include "..\Storage.h"
//#include <vector>

//void buf_func_to_m1p1( std::vector<ISignalStorage*> in_storages, std::vector<ISignalStorage*> out_storages );

template< typename FT >
class ConverterFunction : public IConverter
{
private:
	FT _func;
public:
	ConverterFunction() {}
	void SetFunction( FT & func ) { _func = func; }
	void process() override;
};

template< typename FT >
void ConverterFunction<FT>::process()
{
	StorageCursor * cursor = in_cursor(0);
	auto istor = in_stor<Storage<float>*>(0);
	auto ostor = out_stor<Storage<float>*>(0);
	auto ib = istor->tmp_buf();
	auto ob = ostor->tmp_buf();

	cursor->snapshort( istor );
	long lag = cursor->lag();
	if( 0 == lag )
		return;

	istor->read( cursor->position(), lag, ib );
	
	ob.clear();
	if( (long)ob.capacity() < lag )
		ob.reserve( lag );

	for( auto it = begin(ib); it != end(ib); ++it )
	{
		ob.push_back( _func(*it) );
	}
	ostor->append( ob );
		
	cursor->move_on( lag );
}