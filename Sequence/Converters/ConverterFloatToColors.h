#pragma once

#include "Converter.h"
#include <vector>
#include "Geometry\Vector3.h"

class StorageCursor;

class ConverterFloatToColors: public StorageConverter<float,Vector3::Vect3f>
{
	std::vector<float> _i_buf;
	std::vector<Vector3::Vect3f> _o_buf;
public:
	ConverterFloatToColors();
	void process() override;
};
