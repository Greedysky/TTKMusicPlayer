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

QStringList MusicFormats::supportFormats()
{
    QStringList formats(Format::decodeForString());
    formats << "zip";
    return formats;
}

TTKStringListMap MusicFormats::supportFormatsMap()
{
    TTKStringListMap formats(Format::decodeForStringMap());
    formats.insert("zip", QStringList() << "zip");
    return formats;
}

QStringList MusicFormats::supportFormatsFilter()
{
    QStringList formats(Format::decodeForFilter());
    formats << "*.zip";
    return formats;
}

QStringList MusicFormats::supportFormatsDialogFilter()
{
    QStringList formats(Format::decodeForFilterDialog());
    formats << "Zip File(*.zip)";
    return formats;
}

QStringList MusicFormats::supportFormatsSpekString()
{
    return QStringList()<< "*.3gp *.aac *.aif *.aifc *.aiff *.amr *.awb *.ape *.au *.dts *.flac "
                           "*.flv *.gsm *.m4a *.m4p *.mp3 *.mp4 *.mp+ *.mpc *.mpp *.oga *.ogg *.opus *.ra "
                           "*.ram *.snd *.wav *.wma *.wv";
}

QStringList MusicFormats::supportFormatsPlaylistDialogString()
{
    return QStringList()<< "File(*.tkpl *.m3u *.m3u8 *.pls *.wpl *.xspf *.asx *.kwl *.kgl *.fpl *.csv *.txt)"
                        << "TTKlist File(*.tkpl)"
                        << "M3U File(*.m3u)"
                        << "M3U8 File(*.m3u8)"
                        << "Playlist File(*.pls)"
                        << "Windows Playlist File(*.wpl)"
                        << "XML Shareable Playlist File(*.xspf)"
                        << "Foobar2k Playlist File(*.fpl)"
                        << "Windows Media Playlist File(*.asx)"
                        << "KuWo Playlist File(*.kwl)"
                        << "KuGou Playlist File(*.kgl)"
                        << "CSV Playlist File(*.csv)"
                        << "TXT Playlist File(*.txt)";
}

QString MusicFormats::supportFormatsPlaylistString()
{
    return QString("TTKlist File(*.tkpl);; \
                    M3U File(*.m3u);; \
                    M3U8 File(*.m3u8);; \
                    Playlist File(*.pls);; \
                    Windows Playlist File(*.wpl);; \
                    XML Shareable Playlist File(*.xspf);; \
                    Windows Media Playlist File(*.asx);; \
                    CSV Playlist File(*.csv);; \
                    TXT Playlist File(*.txt);; \
                    Numbered Playlist File(*.nfn)");
}
