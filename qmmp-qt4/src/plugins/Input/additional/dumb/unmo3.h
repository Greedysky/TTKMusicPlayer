/*
	UNMO3 library C/C++ header file
	Copyright (c) 2010-2013 Un4seen Developments Ltd.

	Free for non-commercial use
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#define WINAPI
#endif

// Get the UNMO3 library version
unsigned WINAPI UNMO3_GetVersion();

// Decode a MO3 file
// in: data/len = MO3 data/len
// out: data/len = decoded data/len (if successful)
// return: 0 = Success, 2 = It isn't a MO3 file, 3 = There was a problem decoding the MO3 file
int WINAPI UNMO3_Decode(void **data, unsigned *len, unsigned flags);

// UNMO3_Decode flags
#define UNMO3_DECODE_NOSAMPLES	1 // don't process sample data

// Free the data returned by UNMO3_Decode
void WINAPI UNMO3_Free(void *data);

#ifdef __cplusplus
}
#endif
