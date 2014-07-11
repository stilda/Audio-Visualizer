#include <boost/thread.hpp>
#include "UserInterface.h"
#include "DataSupplier\TestDataSupplier.h"
#include "DataSupplier\MicDataSupplier.h"
#include "DataConsumer\TestDataConsumer.h"
#include "DataConsumer\GlDataConsumer.h"
#include "Converters\ConverterTreeBuilder.h"
#include "Visualizers\VisualizerListBuilder.h"

int main()
{
	print_available_recording_devices();

	Storage<__int16> storage_top(true);

	// Top Input (artificial test signal)
	TestDataSupplier test_data_supplier( &storage_top );
	// Top Input (recording mic)
	boost::shared_ptr<tWAVEFORMATEX> p_wave_format( create_wave_format() );
	MicDataSupplier mic_data_supplier( &storage_top, p_wave_format.get() );

	// Converters
	ConverterTreeBuilder tree_builder;
	tree_builder.set_top_storage( dynamic_cast<ISignalStorage*>(&storage_top) );
	tree_builder.build();

	// Visualizers
	VisualizerListBuilder visualizer_list_builder;
	visualizer_list_builder.set_storages( tree_builder.get_storages() );
	visualizer_list_builder.build();

	// UI
	UserInterface ui( &visualizer_list_builder );

	// Data consumer
	//TestDataConsumer data_consumer( &nodeRoot );
	GlDataConsumer gl_consumer( tree_builder.get_root_node(), &ui );
	auto visualizer_list = visualizer_list_builder.get_visualizers(); 
	for( auto it = visualizer_list->cbegin(); it != visualizer_list->cend(); ++it )
	{
		gl_consumer.AddVizualizer( it->get() );
	}


	// Run threasds
	//boost::thread supplier( test_data_supplier );
	//boost::thread consumer( data_consumer );
	boost::thread supplier( mic_data_supplier );
	boost::thread consumer( gl_consumer );

	supplier.join();
	consumer.join();
}