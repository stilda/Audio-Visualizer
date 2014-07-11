#pragma once

#include "Converter.h"
#include <vector>

class StorageCursor;

class ConverterFloatToMovingMean : public StorageConverter<float,float>
{
	std::vector<float> _i_buf;
	std::vector<float> _o_buf;
	int _window_len;
	float _sum;
public:
	ConverterFloatToMovingMean();
	void process() override;
	void SetMovingWindow( const int window_len ) { _window_len = window_len; }
};
