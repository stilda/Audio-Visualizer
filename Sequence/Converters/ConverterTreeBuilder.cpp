#include "ConverterTreeBuilder.h"
#include "..\StorageCursor.h"
#include "..\Storage.h"
#include "ConverterIntToFloat.h"
#include "ConverterIntTo-1+1.h"
#include "ConverterFloatToColors.h"
#include "ConverterColorsToColorMatrix.h"
#include "ConverterM1P1ToMeanStatistics.h"
#include "ConverterFloatToFloatHistogram.h"
#include "ConverterFloatToFloatDerivate.h"
#include "ConverterFilter.h"
#include "ConverterFunction.h"
#include "ConverterMixAllToOne.h"
#include "ConverterBreakOn3bitsNumbers.h"
#include "Geometry\Vector3.h"
#include <cmath>

using namespace boost;

float func_to_m1p1( float t )
{
	if( t > 0.0f ) return 1;
	if( t < 0.0f ) return -1;
	return 0;
}

template<typename CT>
CT * ConverterTreeBuilder::add_converter( const std::string & name )
{
	auto converter = shared_ptr<IConverter>( new CT() );
	_converters[name] = converter;

	_root_node.add_child( converter );

	return dynamic_cast<CT*>( converter.get() );
}

void ConverterTreeBuilder::add_converter_input( IConverter * converter, ISignalStorage * istor )
{
	auto cursor = shared_ptr<StorageCursor>( new StorageCursor() );
	_cursors.push_back( cursor );
	converter->AddInput( istor, cursor.get() );
}

template<typename OST>
void ConverterTreeBuilder::add_converter_output( IConverter * converter, const std::string & name )
{
	auto ostor = shared_ptr<ISignalStorage>( new OST(false) );
	_storages[name] = ostor;
	converter->AddOutput( ostor.get() );
}

template<typename CT>
CT * ConverterTreeBuilder::add_new_converter( ISignalStorage * istor, const std::string & name )
{
	return add_new_converter0<CT,CT::OStorageType>( istor, name );
}

template<typename CT, typename OST>
CT * ConverterTreeBuilder::add_new_converter0( ISignalStorage * istor, const std::string & name )
{
	_cursors.push_back( shared_ptr<StorageCursor>( new StorageCursor() ) );
	_storages[name] = shared_ptr<ISignalStorage>( new OST(false) );
	auto converter = shared_ptr<IConverter>( new CT() );
	_converters[name] = converter;
	converter->AddInput( istor, _cursors.back().get() );
	converter->AddOutput( _storages[name].get() );

	_root_node.add_child( converter );

	return dynamic_cast<CT*>( converter.get() );
}

void ConverterTreeBuilder::build()
{
	// Filter to get first derivate
	std::vector<float> filter_D1;
	filter_D1.push_back( 0.5f );
	filter_D1.push_back( -0.5f );
	// Filter to get first derivate
	std::vector<float> filter_D1_adv;
	filter_D1_adv.push_back( 1.0f );
	filter_D1_adv.push_back( -8.0f );
	filter_D1_adv.push_back( 0.0f );
	filter_D1_adv.push_back( 8.0f );
	filter_D1_adv.push_back( -1.0f );
	// Filter to get second derivate
	std::vector<float> filter_D2;
	filter_D2.push_back( 1.0f );
	filter_D2.push_back( -2.0f );
	filter_D2.push_back( 1.0f );
	// Filter to get third derivate
	std::vector<float> filter_D3;
	filter_D3.push_back( -1.0f );
	filter_D3.push_back( 3.0f );
	filter_D3.push_back( -3.0f );
	filter_D3.push_back( 1.0f );
	// High freq filter
	std::vector<float> filter_HF;
	filter_HF.push_back( (1.0f+sqrtf(3))/4/sqrtf(2) );
	filter_HF.push_back( (3.0f+sqrtf(3))/4/sqrtf(2) );
	filter_HF.push_back( (3.0f-sqrtf(3))/4/sqrtf(2) );
	filter_HF.push_back( (1.0f-sqrtf(3))/4/sqrtf(2) );

	// Convert top input to floats
	add_new_converter<ConverterIntToFloat>( _top_storage, "Floats" );

	// Filter out high frequencies
	auto cnvrtr_1 = add_new_converter<ConverterFilter>( _storages["Floats"].get(), "FloatsFiltered" );
	cnvrtr_1->SetFilter( filter_HF );

	// First derivate
	auto cnvrtr_2 = add_new_converter<ConverterFilter>( _storages["FloatsFiltered"].get(), "Derivate1" );
	cnvrtr_2->SetFilter( filter_D1 );
	// Second derivate
	auto cnvrtr_3 = add_new_converter<ConverterFilter>( _storages["FloatsFiltered"].get(), "Derivate2" );
	cnvrtr_3->SetFilter( filter_D2 );
	// Third derivate
	auto cnvrtr_4 = add_new_converter<ConverterFilter>( _storages["FloatsFiltered"].get(), "Derivate3" );
	cnvrtr_4->SetFilter( filter_D3 );

	// Filter derivatives
	auto cnvrtr_2f = add_new_converter<ConverterFilter>( _storages["Derivate1"].get(), "Derivate1Filtered" );
	cnvrtr_2f->SetFilter( filter_HF );
	auto cnvrtr_3f = add_new_converter<ConverterFilter>( _storages["Derivate2"].get(), "Derivate2Filtered" );
	cnvrtr_3f->SetFilter( filter_HF );
	auto cnvrtr_4f = add_new_converter<ConverterFilter>( _storages["Derivate3"].get(), "Derivate3Filtered" );
	cnvrtr_4f->SetFilter( filter_HF );

	// Derivatives to -1, 0, +1
	std::string suffix = "Filtered"; //"";
	std::function<float(float)> fu ( func_to_m1p1 );
	typedef ConverterFunction< std::function<float(float)> > ConverterType;
	auto cnvrtr_2q = add_new_converter0<ConverterType,Storage<float>>( _storages["Derivate1"+suffix].get(), "Derivate1_M1P1" );
	cnvrtr_2q->SetFunction( fu );
	//
	auto cnvrtr_3q = add_new_converter0<ConverterType,Storage<float>>( _storages["Derivate2"+suffix].get(), "Derivate2_M1P1" );
	cnvrtr_3q->SetFunction( fu );
	//
	auto cnvrtr_4q = add_new_converter0<ConverterType,Storage<float>>( _storages["Derivate3"+suffix].get(), "Derivate3_M1P1" );
	cnvrtr_4q->SetFunction( fu );
	
	// Mix all derivates -1, -, +1 into one sequence
	auto cnvrtr_mix = add_converter<ConverterMixAllToOne>( "Derivates_M1P1_Mix" );
	add_converter_output<Storage<float>>( cnvrtr_mix, "Derivates_M1P1_Mix" );
	add_converter_input( cnvrtr_mix, _storages["Derivate1_M1P1"].get() );
	add_converter_input( cnvrtr_mix, _storages["Derivate2_M1P1"].get() );
	add_converter_input( cnvrtr_mix, _storages["Derivate3_M1P1"].get() );
	
	// And convert to colors
	add_new_converter<ConverterFloatToColors>( _storages["Derivates_M1P1_Mix"].get(), "Derivates_M1P1_Mix_Colors" );
	
	// Or convert to 3bit numbers
	auto cnvrtr_3bit = add_converter<ConverterBreakOn3bitsNumbers>(  "Derivates_M1P1_Mix_3bits_numbers" );
	add_converter_output<Storage<float>>( cnvrtr_3bit, "Derivates_M1P1_Mix_3bits_numbers" );
	add_converter_input( cnvrtr_3bit, _storages["Derivates_M1P1_Mix"].get() );

	// Convert top input to -1 +1
	add_new_converter<ConverterIntToM1P1>( _top_storage, "M1P1" );

	// Convert to continuous colors
	add_new_converter<ConverterFloatToColors>( _storages["Floats"].get(), "FloatsColors" );
	//
	add_new_converter<ConverterFloatToColors>( _storages["Derivate1"].get(), "Derivate1Colors" );
	// Convert to continuous colors matrix
	auto cnvrtr0 = add_new_converter<ConverterColorsToColorMatrix>( _storages["FloatsColors"].get(), "FloatsColorsMatrix" );
	cnvrtr0->set_sizes( 200, 80 );
	//
	auto cnvrtr1 = add_new_converter<ConverterColorsToColorMatrix>( _storages["Derivate1Colors"].get(), "FloatsDerivate1ColorsMatrix" );
	cnvrtr1->set_sizes( 200, 80 );
	// Converter M1P1 to colors
	add_new_converter<ConverterFloatToColors>( _storages["M1P1"].get(), "M1P1Colors" );
	// Converter colors M1P1ColorsMatrix
	auto cnvrtr2 = add_new_converter<ConverterColorsToColorMatrix>( _storages["M1P1Colors"].get(), "M1P1ColorsMatrix" );
	cnvrtr2->set_sizes( 200, 80 );
	// Converter M1P1 to mean value statistics
	auto cnvrtr3 = add_new_converter<ConverterM1P1ToMeanStatistics>( _storages["M1P1"].get(), "M1P1MeanStat" );
	cnvrtr3->set_wnd_size( 100 );
	// Converter of mean statistics to histogram charts
	auto cnvrtr4 = add_new_converter<ConverterFloatToFloatHistogram>( _storages["M1P1MeanStat"].get(), "M1P1MeanStatHistogram" );
	cnvrtr4->set_hist_params( 20, -20, 20 );
	cnvrtr4->set_wnd_size( 400 );
}