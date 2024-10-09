/***************************************************************************
 *   Copyright (C) 2008-2024 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef QMMP_H
#define QMMP_H

#include <QString>
#include "qmmp_export.h"

#define QMMP_VERSION_MAJOR 1
#define QMMP_VERSION_MINOR 7
#define QMMP_VERSION_PATCH 0

#define QMMP_VERSION_INT (QMMP_VERSION_MAJOR<<16 | QMMP_VERSION_MINOR<<8 | QMMP_VERSION_PATCH)

/*!
 * Converts a \b QString to a \b TagLib::FileName
 */
#ifdef Q_OS_WIN
#  define QStringToFileName(s) TagLib::FileName(reinterpret_cast<const wchar_t *>(s.utf16()))
#else
#  define QStringToFileName(s) s.toLocal8Bit().constData()
#endif

#ifndef qPrintable
#  define qPrintable(s) QString(s).toLocal8Bit().constData()
#endif

#ifndef qUtf8Printable
#  define qUtf8Printable(s) QString(s).toUtf8().constData()
#endif

#ifndef QmmpPrintable
#  define QmmpPrintable(s) Qmmp::generatePrintable(s).constData()
#endif

#define CSTR_TO_QSTR(codec, str, utf) codec->toUnicode(str.toCString(utf)).trimmed()

#ifndef TTK_AS_CONST
#  if QT_VERSION < QT_VERSION_CHECK(5,7,0)
// this adds const to non-const objects (like std::as_const)
template <typename T>
Q_DECL_CONSTEXPR typename std::add_const<T>::type &qAsConst(T &t) noexcept { return t; }
// prevent rvalue arguments:
template <typename T>
void qAsConst(const T &&) = delete;
#  endif
#endif

/*! @brief The Qmmp class stores global settings and enums.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT Qmmp
{
public:
    /*!
     * Playback state enum
     */
    enum State
    {
        Playing = 0, /*!< The player is playing source */
        Paused,      /*!< The player has currently paused its playback */
        Stopped,     /*!< The player is ready to play source */
        Buffering,   /*!< The Player is waiting for data to be able to start playing.   */
        NormalError, /*!< Input source is invalid or unsupported. Player should skip this file */
        FatalError   /*!< This means unrecorvable error die audio output problems. Player should abort playback. */
    };
    /*!
     * Metadata keys
     */
    enum MetaData
    {
        UNKNOWN = -1,
        TITLE = 0,  /*!< Title */
        ARTIST,     /*!< Artist  */
        ALBUMARTIST,/*!< Album artist  */
        ALBUM,      /*!< Album */
        COMMENT,    /*!< Comment */
        GENRE,      /*!< Genre */
        COMPOSER,   /*!< Composer */
        YEAR,       /*!< Year */
        TRACK,      /*!< Track number */
        DISCNUMBER  /*!< Disc number */
    };
    /*!
     * Track properties
     */
    enum TrackProperty
    {
        UNKNOWN_PROPERTY = -1,
        BITRATE = 0,
        SAMPLERATE,
        CHANNELS,
        BITS_PER_SAMPLE,
        FORMAT_NAME,
        DECODER,
        FILE_SIZE
    };
    /*!
     * Keys of ReplayGain information
     */
    enum ReplayGainKey
    {
        REPLAYGAIN_TRACK_GAIN = 0, /*!< track gain */
        REPLAYGAIN_TRACK_PEAK,     /*!< track peak */
        REPLAYGAIN_ALBUM_GAIN,     /*!< album gain */
        REPLAYGAIN_ALBUM_PEAK      /*!< album peak */
    };
    /*!
     * Audio formats
     */
    enum AudioFormat
    {
        PCM_UNKNOWN = -1, /*!< Unknown format */
        PCM_S8 = 0, /*!< Signed 8 bit */
        PCM_U8,     /*!< Unsigned 8 bit */
        PCM_S16LE,  /*!< Signed 16 bit Little Endian */
        PCM_S16BE,  /*!< Signed 16 bit Big Endian */
        PCM_U16LE,  /*!< Unsigned 16 bit Little Endian */
        PCM_U16BE,  /*!< Unsigned 16 bit Big Endian */
        PCM_S24LE,  /*!< Signed 24 bit Little Endian using low three bytes in 32-bit word */
        PCM_S24BE,  /*!< Signed 24 bit Big Endian using low three bytes in 32-bit word */
        PCM_U24LE,  /*!< Unsigned 24 bit Little Endian using low three bytes in 32-bit word */
        PCM_U24BE,  /*!< Unsigned 24 bit Big Endian using low three bytes in 32-bit word */
        PCM_S32LE,  /*!< Signed 32 bit Little Endian */
        PCM_S32BE,  /*!< Signed 32 bit Big Endian */
        PCM_U32LE,  /*!< Unsigned 32 bit Little Endian */
        PCM_U32BE,  /*!< Unsigned 32 bit Big Endian */
        PCM_FLOAT   /*!< Float 32 bit Native Endian, range: -1.0 to 1.0 */
    };

    /*!
     * Audio channels enum.
     */
    enum ChannelPosition
    {
        CHAN_NULL         = 0x00,   /*!< No channel */
        CHAN_FRONT_LEFT   = 0x01,   /*!< Front left channel */
        CHAN_FRONT_RIGHT  = 0x02,   /*!< Front right channel */
        CHAN_REAR_LEFT    = 0x04,   /*!< Rear left channel */
        CHAN_REAR_RIGHT   = 0x08,   /*!< Rear right channel */
        CHAN_FRONT_CENTER = 0x10,   /*!< Front center channel */
        CHAN_REAR_CENTER  = 0x20,   /*!< Rear center channel */
        CHAN_SIDE_LEFT    = 0x40,   /*!< Side left channel */
        CHAN_SIDE_RIGHT   = 0x80,   /*!< Side right channel */
        CHAN_LFE          = 0x100,  /*!< Low-frequency effects channel */
    };

    /*!
     * Returns the configuration file name, including the path.
     */
    static QString configFile();
    /*!
     * Returns the configuration directory path.
     */
    static QString configDir();
    /*!
     * Returns the cache directory path.
     */
    static QString cacheDir();
    /*!
     * Returns %Qmmp library version.
     */
    static QString strVersion();
    /*!
     * Returns the location of the installed Qmmp plugins.
     */
    static QString pluginPath();
    /*!
     * Returns a list of found Qmmp plugins (full paths).
     * @param prefix Plugin type or directory name (examples: Inpunt, Transport, Output).
     */
    static QStringList findPlugins(const QString &prefix);
    /*!
     * Returns the location of the installed ttk plugins.
     */
    static QString ttkPluginPath();
    /*!
     * Returns the decoded text by intput text.
     * @param input text.
     */
    static QByteArray generatePrintable(const QString &text);
};

#endif
