#pragma once

#include <vector>
#include <map>
#include <boost\shared_ptr.hpp>
//#include <boost\weak_ptr.hpp>
#include "ConverterTree.h"

class IConverter;
class ISignalStorage;
class StorageCursor;

class ConverterTreeBuilder
{
private:

	std::map<std::string,boost::shared_ptr<IConverter>> _converters;
	std::vector<boost::shared_ptr<StorageCursor>> _cursors;
	std::map<std::string,boost::shared_ptr<ISignalStorage>> _storages;

	ISignalStorage * _top_storage;
	ConverterNode _root_node;

	template<typename CT>
	CT * add_new_converter( ISignalStorage * istor, const std::string & name );

	template<typename CT, typename OST>
	CT * add_new_converter0( ISignalStorage * istor, const std::string & name );


	template<typename CT>
	CT * add_converter( const std::string & name );

	void add_converter_input( IConverter * converter, ISignalStorage * istor );

	template<typename OST>
	void add_converter_output( IConverter * converter, const std::string & name );

public:
	ConverterTreeBuilder() : _root_node(boost::shared_ptr<IConverter>()) {}
	~ConverterTreeBuilder() {}

	void build();
	void set_top_storage( ISignalStorage * top_storage ) { _top_storage = top_storage; }

	ConverterNode * get_root_node() { return &_root_node; }
	std::map<std::string,boost::shared_ptr<ISignalStorage>> * get_storages() { return &_storages; }
};