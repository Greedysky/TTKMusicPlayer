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
    formats << "*.wma" << "*.ape" << "*.tta" << "*.m4a" << "*.m4b" << "*.aac" << "*.ra" << "*.shn" << "*.ac3" << "*.mka" << "*.vqf" << "*.tak" << "*.spx" << "*.adx" << "*.aix";
    formats << "*.wve" << "*.sln" << "*.paf" << "*.pvf" << "*.ircam" << "*.gsm" << "*.avr" << "*.amr" << "*.dsf" << "*.dsdiff";
    formats << "*.webm" << "*.3g2" << "*.3gp" << "*.asf" << "*.avi" << "*.f4v" << "*.flv" << "*.hevc" << "*.m4v" << "*.mov" << "*.mp4" << "*.mpeg" << "*.rm" << "*.swf" << "*.vob" << "*.wtv";
    formats << "*.aiff" << "*.au" << "*.dts" << "*.mp3" << "*.ogg" << "*.opus" << "*.snd" << "*.oga" << "*.flac";
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
