#include "ConverterTree.h"
#include "Converter.h"

ConverterNode::ConverterNode( boost::shared_ptr<IConverter> iconverter ) 
	: _iconverter(iconverter) 
{

}

boost::shared_ptr<ConverterNode> & ConverterNode::add_child( boost::shared_ptr<IConverter> iconverter ) 
{ 
	_children.push_back( boost::shared_ptr<ConverterNode>( new ConverterNode(iconverter) ) );
	return _children.back();
}

void ConverterNode::process() 
{ 
	if( _iconverter )
	{
		_iconverter->process(); 
	}
	process_children();
}

void ConverterNode::process_children() 
{
	for( auto it = _children.begin(); it != _children.end(); ++it )
	{
		(*it)->process();
	}
}

//IConverter * ConverterNode::find( const std::string & name )
//{
//	if( _iconverter && (_iconverter->get_name() == name) )
//	{
//		return _iconverter;
//	}
//	else
//	{
//		for( auto it = _children.begin(); it != _children.end(); ++it )
//		{
//			IConverter * res = (*it)->find( name );
//			if( res )
//			{
//				return res;
//			}
//		}
//	}
//	return nullptr;
//}


