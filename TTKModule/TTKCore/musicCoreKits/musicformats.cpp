#include "musicformats.h"

#include <qmmp/format.h>

bool MusicFormats::SongTrackValid(const QString &file)
{
    return Format::SongTrackValid(file);
}

bool MusicFormats::SongTrackTpyeContains(const QString &file)
{
    return Format::SongTrackTpyeContains(file);
}

QStringList MusicFormats::supportMusicFormats()
{
    QStringList formats(Format::supportMusicFormats());
    formats << "zip";
    return formats;
}

TTKStringListMap MusicFormats::supportMusicMapFormats()
{
    TTKStringListMap formats(Format::supportMusicMapFormats());
    formats.insert("zip", {"zip"});
    return formats;
}

QStringList MusicFormats::supportMusicInputFilterFormats()
{
    QStringList formats(Format::supportMusicInputFilterFormats());
    formats << "*.zip";
    return formats;
}

QString MusicFormats::supportMusicInputFormats()
{
    QStringList formats(Format::supportMusicInputFormats());
    formats << "Zip File(*.zip)";
    return formats.join(";;");
}

QString MusicFormats::supportSpekInputFormats()
{
    return QString("Audio File(*.3gp *.aac *.aif *.aifc *.aiff *.amr *.awb *.ape *.au *.dts *.flac \
                    *.flv *.gsm *.m4a *.m4p *.mp3 *.mp4 *.mp+ *.mpc *.mpp *.oga *.ogg *.opus *.ra \
                    *.ram *.snd *.wav *.wma *.wv)");
}

QString MusicFormats::supportPlaylistInputFormats()
{
    QStringList formats;
    formats << "Playlist File(*.tkpl *.m3u *.m3u8 *.pls *.wpl *.xspf *.asx *.fpl *.dbpl *.csv *.txt)";
    formats << "TTKlist File(*.tkpl)";
    formats << "M3U File(*.m3u)";
    formats << "M3U8 File(*.m3u8)";
    formats << "Playlist File(*.pls)";
    formats << "Windows Playlist File(*.wpl)";
    formats << "XML Shareable Playlist File(*.xspf)";
    formats << "Windows Media Playlist File(*.asx)";
    formats << "Foobar2k Playlist File(*.fpl)";
    formats << "Deadbeef Playlist File(*.dbpl)";
    formats << "CSV Playlist File(*.csv)";
    formats << "TXT Playlist File(*.txt)";
    return formats.join(";;");
}

QString MusicFormats::supportPlaylistOutputFormats()
{
    QStringList formats;
    formats << "TTKlist File(*.tkpl)";
    formats << "M3U File(*.m3u)";
    formats << "M3U8 File(*.m3u8)";
    formats << "Playlist File(*.pls)";
    formats << "Windows Playlist File(*.wpl)";
    formats << "XML Shareable Playlist File(*.xspf)";
    formats << "Windows Media Playlist File(*.asx)";
    formats << "CSV Playlist File(*.csv)";
    formats << "TXT Playlist File(*.txt)";
    return formats.join(";;");
}
