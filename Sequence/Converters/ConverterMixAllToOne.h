#pragma once

#include "Converter.h"

class ConverterMixAllToOne : public IConverter
{
private:
public:
	ConverterMixAllToOne() {}
	~ConverterMixAllToOne() {}
	void process() override;
};