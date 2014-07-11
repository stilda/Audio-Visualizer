#include "VisualizerListBuilder.h"
#include "VisualizerPlainSignal.h"
#include "VisualizerMultiDimSignal.h"
#include "VisualizerColorStrip.h"
#include "VisualizerColorMatrix.h"
#include "VisualizerPlot1D.h"
#include "Geometry\Vector3.h"
#include "..\StorageCursor.h"
#include <boost\numeric\ublas\matrix.hpp>
#include <algorithm>
#include <cassert>

template<typename ET>
Storage<ET> * VisualizerListBuilder::find_storage( const std::string name )
{
	ISignalStorage * st = (*_storages)[name].get();
	auto res = dynamic_cast<Storage<ET>*>( st );
	assert( res );
	return res;
}

template<typename VT>
VT * VisualizerListBuilder::add_new_visualizer()
{
	_visualizers.push_back( boost::shared_ptr<IVisualizer>(new VT()) );
	return dynamic_cast<VT*>( _visualizers.back().get() );
}

StorageCursor * VisualizerListBuilder::add_new_cursor()
{
	_cursors.push_back( boost::shared_ptr<StorageCursor>( new StorageCursor() ) );
	return _cursors.back().get();
}

void VisualizerListBuilder::build()
{
	int nSamples = 6000;

	{   // Plain and filtered signals
		auto vis = add_new_visualizer<VisualizerPlainSignal>();
		VisualizerPlainSignal::Params params;

		params.scale = 40;
		params.color = Vector3::Vect3f(1,1,1);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Floats"), add_new_cursor() );

		params.scale = 40;
		params.color = Vector3::Vect3f(1,1,0);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("FloatsFiltered"), add_new_cursor() );

		vis->SetMaxVisibleSamples( nSamples );
		vis->SetCaption( "Plain and filtred signals" );
		vis->SetDesiredHeight( 120 );
	}

	{   // Derivates
		auto vis = add_new_visualizer<VisualizerPlainSignal>();
		VisualizerPlainSignal::Params params;

		params.scale = 1000;
		params.color = Vector3::Vect3f(1,0,0);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate1"), add_new_cursor() );

		params.scale = 1500;
		params.color = Vector3::Vect3f(0,1,0);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate2"), add_new_cursor() );

		params.scale = 1500;
		params.color = Vector3::Vect3f(0,0,1);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate3"), add_new_cursor() );

		vis->SetMaxVisibleSamples( nSamples );
		vis->SetCaption( "D1 D2 D3 signals" );
		vis->SetDesiredHeight( 200 );
	}

	/*{	// Derivates M1P1 clipping
		auto vis = add_new_visualizer<VisualizerPlainSignal>();
		VisualizerPlainSignal::Params params;

		params.scale = 20;
		params.color = Vector3::Vect3f(1,0,0);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate1_M1P1"), add_new_cursor() );
		
		params.scale = 16;
		params.color = Vector3::Vect3f(0,1,0);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate2_M1P1"), add_new_cursor() );

		params.scale = 12;
		params.color = Vector3::Vect3f(0,0,1);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate3_M1P1"), add_new_cursor() );

		vis->SetMaxVisibleSamples( nSamples );
		vis->SetCaption( "D1 D2 D3 signals' M1P1" );
		vis->SetDesiredHeight( 60 );
	}*/

	/*{   // Color strip of three derivates
		auto vis = add_new_visualizer<VisualizerColorStrip>();

		vis->AddStorageAndCursor( find_storage<Vector3::Vect3f>("Derivates_M1P1_Mix_Colors"), add_new_cursor() );

		VisualizerColorStrip::Params params;
		params.color = Vector3::Vect3f(1,0.5f,0.5f);
		vis->SetParams( params );
		vis->SetMaxVisibleSamples( nSamples );
		vis->SetCaption( "Derivates M1P1 color strip" );
		vis->SetDesiredHeight( 50 );
	}*/

	{	// Derivates 3 bit numbers
		auto vis = add_new_visualizer<VisualizerPlainSignal>();
		VisualizerPlainSignal::Params params;

		params.scale = 5;
		params.color = Vector3::Vect3f(1,1,1);
		vis->AddParams( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivates_M1P1_Mix_3bits_numbers"), add_new_cursor() );
		
		vis->SetMaxVisibleSamples( nSamples );
		vis->SetCaption( "D1 D2 D3 3bits numbers" );
		vis->SetDesiredHeight( 120 );
	}

	for( auto it = _visualizers.begin(); it != _visualizers.end(); ++it )
	{
		(*it)->SetDesiredWidth( 1400 );
	}

	{	// Derivates 1 and 2 on parametric 2D plot
		BasicPlotParameters params;
		Vector3::vect_to_array<float>( Vector3::Vect3f(0,0.5f,0), params.frame_color );
		Vector3::vect_to_array<float>( Vector3::Vect3f(0,1,0), params.plot_color );
		params.max_plot_points = nSamples;
		params.scale = 1000;

		auto vis = add_new_visualizer<VisualizerMultiDimSignal>();

		vis->SetSignalDim( 2 ); // 2D parametric plot
		vis->AddParameters( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate1Filtered"), add_new_cursor() );
		params.scale = 3000;
		vis->AddParameters( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate2Filtered"), add_new_cursor() );
		params.scale = 3000;
		vis->AddParameters( params );
		vis->AddStorageAndCursor( find_storage<float>("Derivate3Filtered"), add_new_cursor() );
		
		vis->SetCaption( "(D1,D2)" );
		vis->SetDesiredHeight( 300 );
		vis->SetDesiredWidth( 300 );
	}
}

void VisualizerListBuilder::change_pause_state()
{
	std::for_each( 
		_cursors.begin(), _cursors.end(), 
		[](boost::shared_ptr<StorageCursor> & pc){pc->change_pause_state();} 
	);
}