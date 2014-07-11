#pragma once

#include "Visualizer.h"
#include "Geometry\Vector3.h"
#include <vector>

class VisualizerPlainSignal : public Visualizer<float>
{
public:
	typedef struct
	{
		float scale;
		Vector3::Vect3f color;
	} Params;
private:
	int _nMaxVisibleSamples;
	std::vector<Params> _paramss;
public:
	VisualizerPlainSignal();
	virtual ~VisualizerPlainSignal();
	void draw() override;
	void SetMaxVisibleSamples( const int nSamples );
	Params params() { return _paramss.front(); }
	void AddParams( const Params & params );
};