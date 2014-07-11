#pragma once

#include "..\Storage.h"
#include <vector>
#include <cassert>

class StorageCursor;
class ISignalStorage;

class IConverter
{
protected:
	std::vector<StorageCursor*> _input_cursors;
	std::vector<ISignalStorage*> _input_storages;
	std::vector<ISignalStorage*> _output_storages;
public:
	virtual void process() = 0;

	void AddInput( ISignalStorage * ISignalStorage, StorageCursor * icursor )
	{
		assert( icursor );
		assert( ISignalStorage );
		_input_cursors.push_back( icursor );
		_input_storages.push_back( ISignalStorage );
	}

	void AddOutput( ISignalStorage * ostorage )
	{
		assert( ostorage );
		_output_storages.push_back( ostorage );
	}

	template< typename ST > ST in_stor( const int index )
	{
		if( index >= 0 && index < (int)_input_storages.size() )
		{
			return dynamic_cast<ST>( _input_storages[index] );
		}
		else
		{
			return nullptr;
		}
	}

	template< typename ST > ST out_stor( const int index )
	{
		if( index >= 0 && index < (int)_output_storages.size() )
		{
			return dynamic_cast<ST>( _output_storages[index] );
		}
		else
		{
			return nullptr;
		}
	}

	StorageCursor * in_cursor( const int index ) 
	{ 
		if( index >= 0 && index < (int)_input_cursors.size() )
			return _input_cursors[index]; 
		else 
			return nullptr;
	}

	int in_count() 
	{
		return (int)_input_cursors.size();
	}
};

template <typename IElem, typename OElem>
class StorageConverter : public IConverter
{
public:
	typedef Storage<IElem> ISignalStorageType;
	typedef Storage<OElem> OStorageType;
protected:
	typename ISignalStorageType::ArrayType _ib;
	typename OStorageType::ArrayType _ob;
public:
	bool verbose;
public:
	StorageConverter() : verbose(false) {}

	OStorageType * output_storage() { return out_stor<OStorageType*>(0); }
	ISignalStorageType * input_storage() { return in_stor<ISignalStorageType*>(0); }
	StorageCursor * cursor() { return _input_cursors[0]; }
};