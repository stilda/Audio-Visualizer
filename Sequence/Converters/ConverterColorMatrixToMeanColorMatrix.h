#pragma once

#include "Converter.h"
#include "Geometry\Vector3.h"
#include "boost\numeric\ublas\matrix.hpp"

using namespace Vector3;
using namespace boost::numeric::ublas;

class ConverterColorMatrixToMeanColorMatrix : public StorageConverter<matrix<Vect3f>,matrix<Vect3f>>
{
	int _ws;
public:
	ConverterColorMatrixToMeanColorMatrix() : _ws(10) {}
	void process() override;
	void SetWindowSize( int ws ) { _ws = ws; }
};