#pragma once

#include <vector>
#include "Converter.h"

class StorageCursor;

class ConverterFloatToMeanFloat : public StorageConverter<float,float>
{
	int _window_size;
	std::vector<float> _input_buffer;
	std::vector<float> _output_buffer;
public:
	ConverterFloatToMeanFloat();
	void process() override;
	void SetWindowSize( const int window_size );
};