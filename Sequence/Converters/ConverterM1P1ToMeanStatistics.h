#pragma once

#include "Converter.h"

class ConverterM1P1ToMeanStatistics : public StorageConverter<float,float>
{
	int _wnd_size;
public:
	void set_wnd_size( int wnd_size ) { _wnd_size = wnd_size; }
	ConverterM1P1ToMeanStatistics() : _wnd_size(100) {}
	void process() override;

};