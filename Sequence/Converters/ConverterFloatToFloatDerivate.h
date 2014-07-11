#pragma once

#include "Converter.h"

class ConverterFloatToFloatDerivate : public StorageConverter<float,float>
{
public:
	void process() override;
};