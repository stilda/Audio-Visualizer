#pragma once

#include "Converter.h"
#include <vector>

class ConverterFloatToFloatHistogram : public StorageConverter<float,std::vector<float>>
{
	int _wnd_size;
	int _intervals_num;
	float _range_min;
	float _range_max;
public:
	ConverterFloatToFloatHistogram() : _wnd_size(441), _intervals_num(20), _range_min(-1), _range_max(1) {}
	void set_wnd_size( const int wnd_size ) { _wnd_size = wnd_size; }
	void set_hist_params( const int intervals_num, const float range_min, const float range_max ) 
	{ 
		_intervals_num = intervals_num; 
		_range_min = range_min;
		_range_max = range_max;
	}
	void process() override;
};