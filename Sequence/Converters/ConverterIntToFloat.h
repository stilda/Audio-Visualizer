#pragma once

#include <vector>
#include "Converter.h"

class StorageCursor;

class ConverterIntToFloat : public StorageConverter<__int16,float>
{
	std::vector<__int16> _int_buffer;
	std::vector<float> _float_buffer;
	float _scale;
public:
	ConverterIntToFloat();
	void process() override;
};
