#include "musicformats.h"

#include <qmmp/format.h>

bool MusicFormats::songTrackValid(const QString &url)
{
    return Format::songTrackValid(url);
}

QStringList MusicFormats::supportMusicFormats()
{
    return Format::supportMusicFormats();
}

QStringList MusicFormats::supportMusicInputFilterFormats()
{
    return Format::supportMusicInputFilterFormats();
}

QString MusicFormats::supportMusicInputFormats()
{
    const QStringList formats(Format::supportMusicInputFormats());
    return formats.join(";;");
}

QStringList MusicFormats::supportSpekInputFilterFormats()
{
    QStringList formats;
    formats << "*.3gp" << "*.aac" << "*.aif" << "*.aifc" << "*.aiff" << "*.amr";
    formats << "*.awb" << "*.ape" << "*.au" << "*.dts" << "*.flac" << "*.flv";
    formats << "*.gsm" << "*.m4a" << "*.m4p" << "*.mp3" << "*.mp4" << "*.mp+";
    formats << "*.mpc" << "*.mpp" << "*.oga" << "*.ogg" << "*.opus" << "*.ra";
    formats << "*.ram" << "*.snd" << "*.wav" << "*.wma" << "*.wv";
    return formats;
}

QString MusicFormats::supportSpekInputFormats()
{
    return QString("Audio File(%1)").arg(supportSpekInputFilterFormats().join(" "));
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
