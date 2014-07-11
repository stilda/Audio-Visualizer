#pragma once

#include "Converter.h"
#include <vector>

class StorageCursor;

class ConverterIntToM1P1 : public StorageConverter<__int16,float>
{
	std::vector<__int16> _int_buffer;
	std::vector<float> _float_buffer;
public:
	ConverterIntToM1P1();
	void process() override;
};
