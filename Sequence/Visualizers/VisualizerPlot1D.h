#pragma once

#include "Visualizer.h"
#include <vector>
#include "Geometry\Vector3.h"

class VisualizerPlot1D : public Visualizer<std::vector<float>>
{
public:
	typedef struct
	{
		float scale;
		Vector3::Vect3f color;
	} Params;
private:
	Params _params;
public:
	VisualizerPlot1D();
	virtual ~VisualizerPlot1D();
	void draw() override;
	void SetParams( const Params & params );
};