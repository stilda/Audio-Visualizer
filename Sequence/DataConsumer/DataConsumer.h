#pragma once

class ConverterNode;

class IDataConsumer
{
protected:
	ConverterNode * _converter_node;
public:
	IDataConsumer( ConverterNode * converter_node ) : _converter_node(converter_node) {}
	virtual void operator()() = 0;
};