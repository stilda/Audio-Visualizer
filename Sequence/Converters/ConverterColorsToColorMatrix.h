#pragma once

#include "Converter.h"
#include <vector>
#include "Geometry\Vector3.h"
#include "boost\numeric\ublas\matrix.hpp"

using namespace Vector3;
using namespace boost::numeric::ublas;

class StorageCursor;

class ConverterColorsToColorMatrix: public StorageConverter<Vect3f,matrix<Vect3f>>
{
	std::vector<Vect3f> _i_buf;
	std::vector<matrix<Vect3f>> _o_buf;

	int _ix;
	int _iy;

public:
	ConverterColorsToColorMatrix();
	void process() override;
	void set_sizes( const int ix, const int iy ) { _ix = ix; _iy = iy; }
};
