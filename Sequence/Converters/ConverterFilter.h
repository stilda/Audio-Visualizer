#pragma once

#include "Converter.h"
#include <vector>

class ConverterFilter : public StorageConverter<float,float>
{
private:
	std::vector<float> _filter_coefs;
	bool _compensation;
public:
	ConverterFilter();
	void SetFilter( const std::vector<float> & filter_coefs );
	void process() override;
};
