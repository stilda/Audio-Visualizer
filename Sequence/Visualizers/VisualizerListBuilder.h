#pragma once

#include "Visualizer.h"
#include <vector>
#include <map>
#include <boost\shared_ptr.hpp>

class ISignalStorage;
class StorageCursor;
template<typename ET> class Storage;

class VisualizerListBuilder
{
private:
	std::vector< boost::shared_ptr<IVisualizer> > _visualizers;
	std::vector< boost::shared_ptr<StorageCursor> > _cursors;
	std::map< std::string, boost::shared_ptr<ISignalStorage> > * _storages;

	template<typename ET> Storage<ET> * find_storage( const std::string name );
	template<typename VT> VT * add_new_visualizer();
	StorageCursor * add_new_cursor();
public:
	void set_storages( std::map< std::string, boost::shared_ptr<ISignalStorage> > * storages )
	{ 
		_storages = storages; 
	}
	void build();
	void change_pause_state();
	std::vector< boost::shared_ptr<IVisualizer> > * get_visualizers() { return &_visualizers; }
};