#pragma once

#include <vector>
//#include <string>
#include <boost\shared_ptr.hpp>

class IConverter;

class ConverterNode
{
	boost::shared_ptr<IConverter> _iconverter;
	std::vector<boost::shared_ptr<ConverterNode>> _children;
	void process_children(); 
public:
	ConverterNode( boost::shared_ptr<IConverter> iconverter );
	boost::shared_ptr<ConverterNode> & add_child( boost::shared_ptr<IConverter> iconverter );
	void process();
	//IConverter * find( const std::string & name );
};
