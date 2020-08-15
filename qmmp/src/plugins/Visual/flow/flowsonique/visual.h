#pragma once

// Note! 
// *Video points to memory in 32bit video memory, ie, the following can be used
// for your utility and to help your understanding of the pixel format used.

// In Visual C, you should start a DLL project, under code generation set it to
// use the DLL libraries, and most likely for release you will want to turn
// optimisation for speed on. Include this .h file in your project, then create
// or add your main C/C++ file and #include "vis.h"

// To start off, fill in the VisInfo structure. Render should draw a whole frame
// into the 32 bit ABGR buffer (for blur, smoke and zoom to work it should, rather
// than replacing the video data, add to it using bitwise or, saturated add, or
// alpha blend). Make sure not to exceed the boundaries given. 'Pitch' specifies
// the distance, in pixels, between the start of each line. If you have a pointer
// at the start of a line, Pointer+Pitch is the start of the next line and
// Pointer-Pitch is the start of the previous.
// Make sure that QueryModule is defined exactly as at the end of this file,
// including the extern "C" keyword if you're using C++. It should return a
// pointer to this structure. All routines that are pointed to by the structure
// must be filled in even if they don't do anything. Render is the only function
// that is required to have a body.



#define ALPHA_MASK	0xFF000000
#define RED_MASK	0x00FF0000
#define GREEN_MASK	0x0000FF00
#define BLUE_MASK	0x000000FF

#define ALPHA_SHIFT	24
#define RED_SHIFT	16
#define GREEN_SHIFT 8
#define BLUE_SHIFT	0

#define AlphaByte(x)	(((x))>>ALPHA_SHIFT)
#define RedByte(x)		(((x) & RED_MASK)>>RED_SHIFT)
#define GreenByte(x)	(((x) & GREEN_MASK)>>GREEN_SHIFT)
#define BlueByte(x)		(((x) & BLUE_MASK)>>BLUE_SHIFT)


// files should be renamed from .DLL to .SVP
#define BOOL    bool
#ifdef _WIN32
#ifndef DLLEXPORT
#define DLLEXPORT    __declspec( dllexport )
#endif
#else
#define DLLEXPORT
#endif

#define VI_WAVEFORM			0x0001		// set if you need the waveform
#define VI_SPECTRUM			0x0002		// set if you need the FFT values 
#define SONIQUEVISPROC		0x0004		// set if you want to allow Soniques user pref vis to affect your vis
										//   for example - blur, smoke and zoom

#pragma pack (push, 8)

typedef struct 
{
	unsigned int	MillSec;			// Sonique sets this to the time stamp of end this block of data
	unsigned char	Waveform[2][512];	// Sonique sets this to the PCM data being outputted at this time
	unsigned char	Spectrum[2][256];	// Sonique sets this to a lowfidely version of the spectrum data
										//   being outputted at this time
} VisData;

/*
You can pass any of these strings to Query* as expression and be returned a value
(make sure to free it later if it's a string!):

currentsonglength, currentsongposition, scheme_visual_left, scheme_visual_right, scheme_visual_blend - int
currentskinname, currentsongfilename, currentsongtitle, currentsongauthor, currengsongdisplaystring - str
*/

class QueryInterface
{
public:
	virtual bool QueryInt(char* expression, int* value);
	virtual char* QueryString(char* expression);
	virtual void FreeString(char* String);
};

typedef struct _VisInfo
{
	unsigned int Version;				// 1 = supports Clicked(x,y,buttons)

	char	*PluginName;				// Set to the name of the plugin
	int		lRequired;					// Which vis data this plugin requires (set to a combination of
										//   the VI_WAVEFORM, VI_SPECTRUM and SONIQEUVISPROC flags)

	void	(*Initialize)(void);		// Called some time before your plugin is asked to render for
										// the first time
	BOOL	(*Render)( unsigned int *Video, int width, int height, int pitch, VisData* pVD);
										// Called for each frame. Pitch is in pixels and can be negative.
										// Render like this:
										// for(y = 0; y < height; y++)
										// {
										//    for(x = 0; x < width; x++)
										//       Video[x] = <pixel value>;
										//	  Video += pitch;
										// }
										//				OR
										// void PutPixel(int x, int y, unsigned int Pixel)
										// {
										//    _ASSERT( x >= 0 && x < width && y >= 0 && y < height );
										//	  Video[y*pitch+x] = Pixel;
										// }
	BOOL	(*SaveSettings)( char* FileName );
										// Use WritePrivateProfileString to save settings when this is called
										// Example:
										// WritePrivateProfileString("my plugin", "brightness", "3", FileName);
	BOOL	(*OpenSettings)( char* FileName );
										// Use GetPrivateProfileString similarly:
										// char BrightnessBuffer[256];
										// GetPrivateProfileString("my plugin", "brightness", "3", BrightnessBuffer, sizeof(BrightnessBuffer), FileName);

	BOOL	(*Deinit)( );
	BOOL	(*Clicked)( int x, int y, int buttons );
	BOOL	(*ReceiveQueryInterface)( QueryInterface* Interface );

} VisInfo;

//#pragma pack (pop, 8)


// DLL exports this function - it should return a pointer to a static structure
// as above.
extern "C" DLLEXPORT VisInfo* QueryModule(void);
