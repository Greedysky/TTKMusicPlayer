#include "musicformats.h"

///qmmp incldue
#include "format.h"

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
    formats.insert("zip", QStringList() << "zip");
    return formats;
}

QStringList MusicFormats::supportMusicInputFilterFormats()
{
    QStringList formats(Format::supportMusicInputFilterFormats());
    formats << "*.zip";
    return formats;
}

QStringList MusicFormats::supportMusicInputFormats()
{
    QStringList formats(Format::supportMusicInputFormats());
    formats << "Zip File(*.zip)";
    return formats;
}

QStringList MusicFormats::supportSpekInputFormats()
{
    return QStringList()<< "*.3gp *.aac *.aif *.aifc *.aiff *.amr *.awb *.ape *.au *.dts *.flac "
                           "*.flv *.gsm *.m4a *.m4p *.mp3 *.mp4 *.mp+ *.mpc *.mpp *.oga *.ogg *.opus *.ra "
                           "*.ram *.snd *.wav *.wma *.wv";
}

QStringList MusicFormats::supportPlaylistInputFormats()
{
    return QStringList()<< "File(*.tkpl *.m3u *.m3u8 *.pls *.wpl *.xspf *.asx *.fpl *.csv *.txt)"
                        << "TTKlist File(*.tkpl)"
                        << "M3U File(*.m3u)"
                        << "M3U8 File(*.m3u8)"
                        << "Playlist File(*.pls)"
                        << "Windows Playlist File(*.wpl)"
                        << "XML Shareable Playlist File(*.xspf)"
                        << "Windows Media Playlist File(*.asx)"
                        << "Foobar2k Playlist File(*.fpl)"
                        << "CSV Playlist File(*.csv)"
                        << "TXT Playlist File(*.txt)";
}

QString MusicFormats::supportPlaylistOutputFormats()
{
    return QString("TTKlist File(*.tkpl);; \
                    M3U File(*.m3u);; \
                    M3U8 File(*.m3u8);; \
                    Playlist File(*.pls);; \
                    Windows Playlist File(*.wpl);; \
                    XML Shareable Playlist File(*.xspf);; \
                    Windows Media Playlist File(*.asx);; \
                    CSV Playlist File(*.csv);; \
                    TXT Playlist File(*.txt)");
}
