#pragma once

#include "DataConsumer.h"

class ConverterNode;

class TestDataConsumer : public IDataConsumer
{
public:
	TestDataConsumer( ConverterNode * converter_node );
	void operator()() override;
};