#pragma once

#include "Visualizer.h"
#include "Geometry\Vector3.h"
#include <vector>
#include <boost\shared_ptr.hpp>

class VisualizerMultiDimSignal : public Visualizer<float>
{
	int _dim;

public:
	VisualizerMultiDimSignal();
	virtual ~VisualizerMultiDimSignal();
	void draw() override;
	void SetSignalDim( const int dim ) { _dim = dim; }
};