#pragma once

#include "Visualizer.h"
#include "Geometry\Vector3.h"

class VisualizerColorStrip : public Visualizer<Vector3::Vect3f>
{
public:
	typedef struct
	{
		Vector3::Vect3f color;
	} Params;
private:
	Params _params;
	int _nMaxVisibleSamples;
public:
	VisualizerColorStrip() {}
	virtual ~VisualizerColorStrip() {}
	void draw() override;
	void SetMaxVisibleSamples( const int nSamples );
	void SetParams( const Params & params );
};