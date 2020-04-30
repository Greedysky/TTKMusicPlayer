// Generated automatically with "cito". Do not edit.
#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct ASAP ASAP;
typedef struct ASAPInfo ASAPInfo;
typedef struct ASAPWriter ASAPWriter;

/**
 * Format of output samples.
 */
typedef enum {
	/**
	 * Unsigned 8-bit.
	 */
	ASAPSampleFormat_U8,
	/**
	 * Signed 16-bit little-endian.
	 */
	ASAPSampleFormat_S16_L_E,
	/**
	 * Signed 16-bit big-endian.
	 */
	ASAPSampleFormat_S16_B_E
} ASAPSampleFormat;

ASAP *ASAP_New(void);
void ASAP_Delete(ASAP *self);

/**
 * Output sample rate.
 */
#define ASAP_SAMPLE_RATE 44100

/**
 * Enables silence detection.
 * Causes playback to stop after the specified period of silence.
 * @param self This <code>ASAP</code>.
 * @param seconds Length of silence which ends playback. Zero disables silence detection.
 */
void ASAP_DetectSilence(ASAP *self, int seconds);

/**
 * Loads music data ("module").
 * @param self This <code>ASAP</code>.
 * @param filename Filename, used to determine the format.
 * @param module Contents of the file.
 * @param moduleLen Length of the file.
 */
bool ASAP_Load(ASAP *self, const char *filename, uint8_t const *module, int moduleLen);

/**
 * Returns information about the loaded module.
 * @param self This <code>ASAP</code>.
 */
const ASAPInfo *ASAP_GetInfo(const ASAP *self);

/**
 * Mutes the selected POKEY channels.
 * @param self This <code>ASAP</code>.
 * @param mask An 8-bit mask which selects POKEY channels to be muted.
 */
void ASAP_MutePokeyChannels(ASAP *self, int mask);

/**
 * Prepares playback of the specified song of the loaded module.
 * @param self This <code>ASAP</code>.
 * @param song Zero-based song index.
 * @param duration Playback time in milliseconds, -1 means infinity.
 */
bool ASAP_PlaySong(ASAP *self, int song, int duration);

/**
 * Returns current playback position in blocks.
 * A block is one sample or a pair of samples for stereo.
 * @param self This <code>ASAP</code>.
 */
int ASAP_GetBlocksPlayed(const ASAP *self);

/**
 * Returns current playback position in milliseconds.
 * @param self This <code>ASAP</code>.
 */
int ASAP_GetPosition(const ASAP *self);

/**
 * Changes the playback position.
 * @param self This <code>ASAP</code>.
 * @param block The requested absolute position in samples (always 44100 per second, even in stereo).
 */
bool ASAP_SeekSample(ASAP *self, int block);

/**
 * Changes the playback position.
 * @param self This <code>ASAP</code>.
 * @param position The requested absolute position in milliseconds.
 */
bool ASAP_Seek(ASAP *self, int position);

/**
 * Fills leading bytes of the specified buffer with WAV file header.
 * Returns the number of changed bytes.
 * @param self This <code>ASAP</code>.
 * @param buffer The destination buffer.
 * @param format Format of samples.
 * @param metadata Include metadata (title, author, date).
 */
int ASAP_GetWavHeader(const ASAP *self, uint8_t *buffer, ASAPSampleFormat format, bool metadata);

/**
 * Fills the specified buffer with generated samples.
 * @param self This <code>ASAP</code>.
 * @param buffer The destination buffer.
 * @param bufferLen Number of bytes to fill.
 * @param format Format of samples.
 */
int ASAP_Generate(ASAP *self, uint8_t *buffer, int bufferLen, ASAPSampleFormat format);

/**
 * Returns POKEY channel volume - an integer between 0 and 15.
 * @param self This <code>ASAP</code>.
 * @param channel POKEY channel number (from 0 to 7).
 */
int ASAP_GetPokeyChannelVolume(const ASAP *self, int channel);

ASAPInfo *ASAPInfo_New(void);
void ASAPInfo_Delete(ASAPInfo *self);

/**
 * ASAP version - major part.
 */
#define ASAPInfo_VERSION_MAJOR 5

/**
 * ASAP version - minor part.
 */
#define ASAPInfo_VERSION_MINOR 0

/**
 * ASAP version - micro part.
 */
#define ASAPInfo_VERSION_MICRO 1

/**
 * ASAP version as a string.
 */
#define ASAPInfo_VERSION "5.0.1"

/**
 * Years ASAP was created in.
 */
#define ASAPInfo_YEARS "2005-2020"

/**
 * Short credits for ASAP.
 */
#define ASAPInfo_CREDITS "Another Slight Atari Player (C) 2005-2020 Piotr Fusik\nCMC, MPT, TMC, TM2 players (C) 1994-2005 Marcin Lewandowski\nRMT player (C) 2002-2005 Radek Sterba\nDLT player (C) 2009 Marek Konopka\nCMS player (C) 1999 David Spilka\nFC player (C) 2011 Jerzy Kut\n"

/**
 * Short license notice.
 * Display after the credits.
 */
#define ASAPInfo_COPYRIGHT "This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version."

/**
 * Maximum length of a supported input file.
 * You may assume that files longer than this are not supported by ASAP.
 */
#define ASAPInfo_MAX_MODULE_LENGTH 65000

/**
 * Maximum length of text metadata.
 */
#define ASAPInfo_MAX_TEXT_LENGTH 127

/**
 * Maximum number of songs in a file.
 */
#define ASAPInfo_MAX_SONGS 32

/**
 * Returns author's name.
 * A nickname may be included in parentheses after the real name.
 * Multiple authors are separated with <code>" &amp; "</code>.
 * An empty string means the author is unknown.
 * @param self This <code>ASAPInfo</code>.
 */
const char *ASAPInfo_GetAuthor(const ASAPInfo *self);

/**
 * Sets author's name.
 * A nickname may be included in parentheses after the real name.
 * Multiple authors are separated with <code>" &amp; "</code>.
 * An empty string means the author is unknown.
 * @param self This <code>ASAPInfo</code>.
 * @param value New author's name for the current music.
 */
bool ASAPInfo_SetAuthor(ASAPInfo *self, const char *value);

/**
 * Returns music title.
 * An empty string means the title is unknown.
 * @param self This <code>ASAPInfo</code>.
 */
const char *ASAPInfo_GetTitle(const ASAPInfo *self);

/**
 * Sets music title.
 * An empty string means the title is unknown.
 * @param self This <code>ASAPInfo</code>.
 * @param value New title for the current music.
 */
bool ASAPInfo_SetTitle(ASAPInfo *self, const char *value);

/**
 * Returns music title or filename.
 * If title is unknown returns filename without the path or extension.
 * @param self This <code>ASAPInfo</code>.
 */
const char *ASAPInfo_GetTitleOrFilename(const ASAPInfo *self);

/**
 * Returns music creation date.
 * Some of the possible formats are:
 * <ul>
 * <li>YYYY</li>
 * <li>MM/YYYY</li>
 * <li>DD/MM/YYYY</li>
 * <li>YYYY-YYYY</li>
 * </ul>
 * An empty string means the date is unknown.
 * @param self This <code>ASAPInfo</code>.
 */
const char *ASAPInfo_GetDate(const ASAPInfo *self);

/**
 * Sets music creation date.
 * Some of the possible formats are:
 * <ul>
 * <li>YYYY</li>
 * <li>MM/YYYY</li>
 * <li>DD/MM/YYYY</li>
 * <li>YYYY-YYYY</li>
 * </ul>
 * An empty string means the date is unknown.
 * @param self This <code>ASAPInfo</code>.
 * @param value New music creation date.
 */
bool ASAPInfo_SetDate(ASAPInfo *self, const char *value);

/**
 * Returns music creation year.
 * -1 means the year is unknown.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetYear(const ASAPInfo *self);

/**
 * Returns music creation month (1-12).
 * -1 means the month is unknown.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetMonth(const ASAPInfo *self);

/**
 * Returns day of month of the music creation date.
 * -1 means the day is unknown.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetDayOfMonth(const ASAPInfo *self);

/**
 * Returns 1 for mono or 2 for stereo.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetChannels(const ASAPInfo *self);

/**
 * Returns number of songs in the file.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetSongs(const ASAPInfo *self);

/**
 * Returns 0-based index of the "main" song.
 * The specified song should be played by default.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetDefaultSong(const ASAPInfo *self);

/**
 * Sets the 0-based index of the "main" song.
 * @param self This <code>ASAPInfo</code>.
 * @param song New default song.
 */
bool ASAPInfo_SetDefaultSong(ASAPInfo *self, int song);

/**
 * Returns length of the specified song.
 * The length is specified in milliseconds. -1 means the length is indeterminate.
 * @param self This <code>ASAPInfo</code>.
 * @param song Song to get length of, 0-based.
 */
int ASAPInfo_GetDuration(const ASAPInfo *self, int song);

/**
 * Sets length of the specified song.
 * The length is specified in milliseconds. -1 means the length is indeterminate.
 * @param self This <code>ASAPInfo</code>.
 * @param song Song to set length of, 0-based.
 * @param duration New length in milliseconds.
 */
bool ASAPInfo_SetDuration(ASAPInfo *self, int song, int duration);

/**
 * Returns information whether the specified song loops.
 * Returns:
 * <ul>
 * <li><code>true</code> if the song loops</li>
 * <li><code>false</code> if the song stops</li>
 * </ul>
 * 
 * @param self This <code>ASAPInfo</code>.
 * @param song Song to check for looping, 0-based.
 */
bool ASAPInfo_GetLoop(const ASAPInfo *self, int song);

/**
 * Sets information whether the specified song loops.
 * Use:
 * <ul>
 * <li><code>true</code> if the song loops</li>
 * <li><code>false</code> if the song stops</li>
 * </ul>
 * 
 * @param self This <code>ASAPInfo</code>.
 * @param song Song to set as looping, 0-based.
 * @param loop <code>true</code> if the song loops.
 */
bool ASAPInfo_SetLoop(ASAPInfo *self, int song, bool loop);

/**
 * Returns <code>true</code> for NTSC song and <code>false</code> for PAL song.
 * @param self This <code>ASAPInfo</code>.
 */
bool ASAPInfo_IsNtsc(const ASAPInfo *self);

/**
 * Returns the letter argument for the TYPE SAP tag.
 * Returns zero for non-SAP files.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetTypeLetter(const ASAPInfo *self);

/**
 * Returns player routine rate in Atari scanlines.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetPlayerRateScanlines(const ASAPInfo *self);

/**
 * Returns approximate player routine rate in Hz.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetPlayerRateHz(const ASAPInfo *self);

/**
 * Returns the address of the module.
 * Returns -1 if unknown.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetMusicAddress(const ASAPInfo *self);

/**
 * Causes music to be relocated.
 * Use only with <code>ASAPWriter.Write</code>.
 * @param self This <code>ASAPInfo</code>.
 * @param address New music address.
 */
bool ASAPInfo_SetMusicAddress(ASAPInfo *self, int address);

/**
 * Returns the address of the player initialization routine.
 * Returns -1 if no initialization routine.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetInitAddress(const ASAPInfo *self);

/**
 * Returns the address of the player routine.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetPlayerAddress(const ASAPInfo *self);

/**
 * Returns the address of the COVOX chip.
 * Returns -1 if no COVOX enabled.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetCovoxAddress(const ASAPInfo *self);

/**
 * Retturns the length of the SAP header in bytes.
 * @param self This <code>ASAPInfo</code>.
 */
int ASAPInfo_GetSapHeaderLength(const ASAPInfo *self);

/**
 * Returns the offset of instrument names for RMT module.
 * Returns -1 if not an RMT module or RMT module without instrument names.
 * @param self This <code>ASAPInfo</code>.
 * @param module Content of the RMT file.
 * @param moduleLen Length of the RMT file.
 */
int ASAPInfo_GetInstrumentNamesOffset(const ASAPInfo *self, uint8_t const *module, int moduleLen);

/**
 * Returns the number of milliseconds represented by the given string.
 * @param s Time in the <code>"mm:ss.xxx"</code> format.
 */
int ASAPInfo_ParseDuration(const char *s);

/**
 * Checks whether the filename represents a module type supported by ASAP.
 * Returns <code>true</code> if the filename is supported by ASAP.
 * @param filename Filename to check the extension of.
 */
bool ASAPInfo_IsOurFile(const char *filename);

/**
 * Checks whether the filename extension represents a module type supported by ASAP.
 * Returns <code>true</code> if the filename extension is supported by ASAP.
 * @param ext Filename extension without the leading dot.
 */
bool ASAPInfo_IsOurExt(const char *ext);

/**
 * Loads file information.
 * @param self This <code>ASAPInfo</code>.
 * @param filename Filename, used to determine the format.
 * @param module Contents of the file.
 * @param moduleLen Length of the file.
 */
bool ASAPInfo_Load(ASAPInfo *self, const char *filename, uint8_t const *module, int moduleLen);

/**
 * Returns human-readable description of the filename extension.
 * @param ext Filename extension without the leading dot.
 */
const char *ASAPInfo_GetExtDescription(const char *ext);

/**
 * Returns the extension of the original module format.
 * For native modules it simply returns their extension.
 * For the SAP format it attempts to detect the original module format.
 * @param self This <code>ASAPInfo</code>.
 * @param module Contents of the file.
 * @param moduleLen Length of the file.
 */
const char *ASAPInfo_GetOriginalModuleExt(const ASAPInfo *self, uint8_t const *module, int moduleLen);

ASAPWriter *ASAPWriter_New(void);
void ASAPWriter_Delete(ASAPWriter *self);

/**
 * Maximum number of extensions returned by <code>GetSaveExts</code>.
 */
#define ASAPWriter_MAX_SAVE_EXTS 3

/**
 * Maximum length of text representation of a duration.
 * Corresponds to the longest format which is <code>"mm:ss.xxx"</code>.
 */
#define ASAPWriter_MAX_DURATION_LENGTH 9

/**
 * Enumerates possible file types the given module can be written as.
 * Returns the number of extensions written to <code>exts</code>.
 * @param exts Receives filename extensions without the leading dot.
 * @param info File information.
 * @param module Contents of the file.
 * @param moduleLen Length of the file.
 */
int ASAPWriter_GetSaveExts(const char **exts, const ASAPInfo *info, uint8_t const *module, int moduleLen);

/**
 * Writes text representation of the given duration.
 * Returns the number of bytes written to <code>result</code>.
 * @param result The output buffer.
 * @param value Number of milliseconds.
 */
int ASAPWriter_DurationToString(uint8_t *result, int value);

/**
 * Sets the destination array for <code>Write</code>.
 * @param self This <code>ASAPWriter</code>.
 * @param output The destination array.
 * @param startIndex The array offset to start writing at.
 * @param endIndex The array offset to finish writing before.
 */
void ASAPWriter_SetOutput(ASAPWriter *self, uint8_t *output, int startIndex, int endIndex);

/**
 * Writes the given module in a possibly different file format.
 * @param self This <code>ASAPWriter</code>.
 * @param targetFilename Output filename, used to determine the format.
 * @param info File information got from the source file with data updated for the output file.
 * @param module Contents of the source file.
 * @param moduleLen Length of the source file.
 * @param tag Display information (xex output only).
 */
int ASAPWriter_Write(ASAPWriter *self, const char *targetFilename, const ASAPInfo *info, uint8_t const *module, int moduleLen, bool tag);

#ifdef __cplusplus
}
#endif
