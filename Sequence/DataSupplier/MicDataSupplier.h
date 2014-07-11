#pragma once

#include "DataSupplier.h"
#include "../Storage.h"
#include <Windows.h>

class MicDataSupplier : public IDataSupplier<Storage<__int16>>
{
	tWAVEFORMATEX * _wave_format;
public:
	MicDataSupplier( Storage<__int16> * storage, tWAVEFORMATEX * waveformat );
	~MicDataSupplier();
	void run() override;
};

void print_available_recording_devices();
tWAVEFORMATEX * create_wave_format();