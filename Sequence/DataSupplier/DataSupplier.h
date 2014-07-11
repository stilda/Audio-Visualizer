#pragma once

template <typename StorageType>
class IDataSupplier
{
protected:
	StorageType * _storage;
public:
	IDataSupplier( StorageType * storage ) : _storage(storage) {}
	virtual void run() = 0;
	void operator()() { run(); }
};