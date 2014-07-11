#pragma once

#include "DataSupplier.h"
#include "../Storage.h"

class TestDataSupplier : public IDataSupplier<Storage<__int16>>
{
public:
	TestDataSupplier( Storage<__int16> * storage );
	void run() override;
};