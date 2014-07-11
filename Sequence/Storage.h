#pragma once

#include <vector>
#include <boost\thread\mutex.hpp>
#include <boost\thread\locks.hpp>

class ISignalStorage
{
public:
	ISignalStorage() {}
	virtual ~ISignalStorage() {}
};

template <typename E>
class Storage : public ISignalStorage
{
public:
	typedef E ElementType;
	typedef std::vector<E> ArrayType;
private:
	boost::mutex _mutex;
	std::vector<E> _data;
	std::vector<E> _tmp_buf;
	bool _thread_safe;
public:

	Storage( const bool thread_safe ) : _thread_safe(thread_safe) {}

	~Storage() {}

	std::vector<E> & tmp_buf() { return _tmp_buf; }

	void append( const std::vector<E> & new_data )
	{
		if( _thread_safe )
		{
			boost::lock_guard<boost::mutex> guard( _mutex );
			_data.insert( _data.end(), new_data.cbegin(), new_data.cend() );
		}
		else
		{
			_data.insert( _data.end(), new_data.cbegin(), new_data.cend() );
		}
	}

	void read( const long pos_start, const long count, std::vector<E> & buffer )
	{
		buffer.clear();
		if( _thread_safe )
		{
			boost::lock_guard<boost::mutex> guard( _mutex );
			buffer.insert( buffer.end(), _data.cbegin() + pos_start, _data.cbegin() + pos_start + count );	
		}
		else
		{
			buffer.insert( buffer.end(), _data.cbegin() + pos_start, _data.cbegin() + pos_start + count );
		}
	}

	void append_elem( const E new_elem )
	{
		if( _thread_safe )
		{
			boost::lock_guard<boost::mutex> guard( _mutex );
			_data.insert( _data.end(), new_elem );
		}
		else
		{
			_data.insert( _data.end(), new_elem );
		}
	}

	E read_elem( const long pos )
	{
		if( _thread_safe )
		{
			boost::lock_guard<boost::mutex> guard( _mutex );
			return *(_data.cbegin() + pos);
		}
		else
		{
			return *(_data.cbegin() + pos);
		}
	}

	long count()
	{
		if( _thread_safe )
		{
			boost::lock_guard<boost::mutex> guard( _mutex );
			return _data.size();
		}
		else
		{
			return _data.size();
		}
	}

};