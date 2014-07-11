#include "MicDataSupplier.h"
#include <vector>
#include <boost\thread\thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>

void print_available_recording_devices()
{
	WAVEINCAPS     wic;
	unsigned long  iNumDevs, i;
    
	//convert from wide char to narrow char array
    char chStr[260];
    char DefChar = ' ';
    //A std:string  using the char* constructor.

	
	/* Get the number of Digital Audio In devices in this computer */
	iNumDevs = waveInGetNumDevs();

	/* Go through all of those devices, displaying their names */
	for (i = 0; i < iNumDevs; i++)
	{
		/* Get info about the next device */
		if (!waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS)))
		{
		    WideCharToMultiByte( CP_ACP, 0, wic.szPname, -1, chStr, 260, &DefChar, NULL );
			/* Display its Device ID and name */
			std::cout<< "Device ID " << i << " " << std::string(chStr) << std::endl;
		}
	}
}

tWAVEFORMATEX * create_wave_format()
{
	tWAVEFORMATEX * wave_format;
	wave_format = new WAVEFORMATEX;
	ZeroMemory( wave_format, sizeof(WAVEFORMATEX) );
	wave_format->wFormatTag      = WAVE_FORMAT_PCM;
	wave_format->nChannels       = 1;
	wave_format->nSamplesPerSec  = 44100;
	wave_format->wBitsPerSample  = 16;
	wave_format->nBlockAlign     = wave_format->wBitsPerSample / 8 * wave_format->nChannels;
	wave_format->cbSize			 = 0;
	wave_format->nAvgBytesPerSec = wave_format->nSamplesPerSec * wave_format->nBlockAlign;
	return wave_format;
}

void WaveInCheck( MMRESULT res )
{
	if( MMSYSERR_NOERROR == res ) return;
	TCHAR ErrText[MAXERRORLENGTH+1] = L"";
	waveInGetErrorText( res, ErrText, sizeof(ErrText) );
	::MessageBox( 0, ErrText, 0, 0 );
	throw "err In";
}

MicDataSupplier::MicDataSupplier( Storage<__int16> * storage, tWAVEFORMATEX * waveformat )
	: IDataSupplier(storage),
	  _wave_format(waveformat)
{
}

MicDataSupplier::~MicDataSupplier()
{
}

void MicDataSupplier::run()
{
	// Calc block_size.
	unsigned int block_size = 2048 /*samples*/ * 2/*bps*/; //44100/2048=21.xxx

	const int nBlocks = 20; 
	HANDLE  hEvent  = CreateEvent( 0, FALSE, FALSE, 0 );
	HWAVEIN hRecord = (HWAVEIN) 0;
	WAVEHDR Header[nBlocks];
	ZeroMemory( Header, sizeof(Header) );
	for( int i = 0; i < nBlocks; i++ )
	{
		Header[i].lpData = (LPSTR) new BYTE[ block_size ];
		Header[i].dwBufferLength = block_size;
	}

	std::vector<__int16> vdata( block_size/2, 0 );
	//ResetEvent( _stop_event );

	try
	{
		WaveInCheck( waveInOpen( &hRecord, WAVE_MAPPER, _wave_format, (DWORD_PTR)hEvent, 0, CALLBACK_EVENT ) );		
		for( int i = 0; i < nBlocks; i++ )
		{
			WaveInCheck( waveInPrepareHeader( hRecord, Header+i, sizeof(WAVEHDR) ) );
			WaveInCheck( waveInAddBuffer( hRecord, Header+i, sizeof(WAVEHDR) ) );
		}

		ResetEvent( hEvent );
		WaveInCheck( waveInStart( hRecord ) );

		int nCurBlock = 0;
		//while( WaitForSingleObject( _stop_event, 0 ) != WAIT_OBJECT_0 )
		while( true )
		{
			WaitForSingleObject( hEvent, INFINITE ); 
			if( Header[nCurBlock].dwBytesRecorded > 0 )
			{
				Header[nCurBlock].dwBytesRecorded = 0;
				WaveInCheck( waveInAddBuffer( hRecord, &Header[nCurBlock], sizeof(WAVEHDR) ) );
				memcpy( &vdata[0], Header[nCurBlock].lpData, block_size );
				_storage->append( vdata );
			}
			else
			{
				throw "bytes not recorded";			
			}
			nCurBlock = ( nCurBlock + 1 ) % nBlocks;
		}
	}
	catch(...)
	{
	}

	if( hRecord )
	{
		waveInReset( hRecord );
		waveInStop( hRecord );
		for( int i = 0; i < nBlocks; i++ )
		{
			waveInUnprepareHeader( hRecord, &Header[i], sizeof(WAVEHDR) );
		}
		waveInClose( hRecord );
	}

	CloseHandle( hEvent );

	for( int i = 0; i < nBlocks; i++ )
	{
		delete[] Header[i].lpData;
	}
}