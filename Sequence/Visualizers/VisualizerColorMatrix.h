#pragma once

#include "Visualizer.h"
#include "Geometry\Vector3.h"
#include "boost\numeric\ublas\matrix.hpp"

using namespace boost::numeric::ublas;

class VisualizerColorMatrix : public Visualizer<matrix<Vector3::Vect3f>>
{
public:
	typedef struct
	{
		int ix;
		int iy;
		Vector3::Vect3f color;
	} Params;
private:
	Params _params;
public:
	VisualizerColorMatrix() {}
	virtual ~VisualizerColorMatrix() {}
	void draw() override;

	void SetParams( const Params & params );
};