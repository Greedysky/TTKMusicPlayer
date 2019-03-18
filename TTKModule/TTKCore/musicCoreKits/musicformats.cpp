#include "musicformats.h"

QStringList MusicFormats::supportFormatsString()
{
    return QStringList()<< "mpc" << "wv" << "ogg" << "wav" << "au" << "snd" << "aif" << "aiff" << "8svx"
                        << "sph" << "sf" << "voc" << "w64" << "cue" << "mp1" << "mp2" << "mp3" << "wav"
                        << "ay" << "gms" << "gym" << "hes" << "kss" << "nsf" << "nsfe" << "sap" << "spc"
                        << "vgm" << "vgz" << "flac" << "oga" << "ape" << "wma" << "ape" << "tta" << "m4a"
                        << "ra" << "shn" << "vqf" << "ac3" << "mod" << "s3m" << "xm" << "it" << "669"
                        << "amf" << "ams" << "dbm" << "dmf" << "dsm" << "far" << "mdl" << "med" << "mtm"
                        << "okt" << "ptm" << "stm" << "ult" << "umx" << "mt2" << "psm" << "mdz" << "s3z"
                        << "xmz" << "itz" << "mdr" << "s3r" << "xmr" << "itr" << "dgz" << "s3gz" << "xmgz"
                        << "itgz" << "opus" << "mid"
#ifndef Q_OS_WIN
                        << "sid" << "mus" << "str" << "prg" << "P00"
#endif
                        << "aac";
}

MStringListMap MusicFormats::supportFormatsStringMap()
{
    MStringListMap formats;
    formats.insert("mpc", QStringList() << "mpc");
    formats.insert("wavpack", QStringList() << "wv");
    formats.insert("vorbis", QStringList() << "ogg");
    formats.insert("opus", QStringList() << "opus");
    formats.insert("sndfile", QStringList() << "wav" << "au" << "snd" << "aif" << "aiff" << "8svx"
                                            << "sph" << "sf" << "voc" << "w64");
    formats.insert("cue", QStringList() << "cue");
    formats.insert("mpeg", QStringList() << "mp1" << "mp2" << "mp3" << "wav");
    formats.insert("gme", QStringList() << "ay" << "gms" << "gym" << "hes" << "kss" << "nsf" << "nsfe"
                                        << "sap" << "spc" << "vgm" << "vgz)");
    formats.insert("flac", QStringList() << "flac" << "oga");
    formats.insert("aac", QStringList() << "aac");
    formats.insert("wildmidi", QStringList() << "mid");
#ifndef Q_OS_WIN
    formats.insert("sid", QStringList() << "sid" << "mus" << "str" << "prg" << "P00");
#endif
    formats.insert("ffmpeg", QStringList() << "wma" << "ape" << "tta" << "m4a" << "ra" << "shn" << "vqf" << "ac3" << "tak");
    formats.insert("modplug", QStringList() << "mod" << "s3m" << "xm" << "it" << "669" << "amf" << "ams" << "dbm" << "dmf"
                                            << "dsm" << "far" << "mdl" << "med" << "mtm" << "okt" << "ptm" << "stm" << "ult"
                                            << "umx" << "mt2" << "psm" << "mdz" << "s3z" << "xmz" << "itz" << "mdr" << "s3r"
                                            << "xmr" << "itr" << "dgz" << "s3gz" << "xmgz" << "itgz");
    return formats;
}

QStringList MusicFormats::supportFormatsFilterString()
{
    return QStringList()<< "*.mpc" << "*.wv" << "*.ogg" << "*.wav" << "*.au" << "*.snd" << "*.aif" << "*.aiff" << "*.8svx"
                        << "*.sph" << "*.sf" << "*.voc" << "*.w64" << "*.cue" << "*.mp1" << "*.mp2" << "*.mp3" << "*.mid"
                        << "*.ay" << "*.gms" << "*.gym" << "*.hes" << "*.kss" << "*.nsf" << "*.nsfe" << "*.sap" << "*.spc"
                        << "*.vgm" << "*.vgz" << "*.flac" << "*.oga" << "*.ape" << "*.wma" << "*.tta" << "*.m4a" << "*.opus"
                        << "*.ra" << "*.shn" << "*.vqf" << "*.ac3" << "*.mod" << "*.s3m" << "*.xm" << "*.it" << "*.669"
                        << "*.amf" << "*.ams" << "*.dbm" << "*.dmf" << "*.dsm" << "*.far" << "*.mdl" << "*.med" << "*.mtm"
                        << "*.okt" << "*.ptm" << "*.stm" << "*.ult" << "*.umx" << "*.mt2" << "*.psm" << "*.mdz" << "*.s3z"
                        << "*.xmz" << "*.itz" << "*.mdr" << "*.s3r" << "*.xmr" << "*.itr" << "*.dgz" << "*.s3gz" << "*.xmgz"
                        << "*.itgz"
#ifndef Q_OS_WIN
                        << "*.sid" << "*.mus" << "*.str" << "*.prg" << "*.P00"
#endif
                        << "*.aac";
}

QStringList MusicFormats::supportFormatsFilterDialogString()
{
    return QStringList()<< "All File(*.*)" << "MusePack File(*.mpc)" << "WavPack File(*.wv)" << "Ogg Vorbis File(*.ogg)"
                        << "PCM File(*.wav *.au *.snd *.aif *.aiff *.8svx *.sph *.sf *.voc *.w64)"
                        << "CUE File(*.cue)" << "MPEG File(*.mp1 *.mp2 *.mp3 *.wav)"
                        << "Game File(*.ay *.gms *.gym *.hes *.kss *.nsf *.nsfe *.sap *.spc *.vgm *.vgz)"
                        << "FLAC File(*.flac *.oga)" << "Monkey's Audio File(*.ape)"
                        << "Ogg Opus File(*.opus)" << "ADTS AAC File(*.aac)" << "MIDI File(*.mid)"
#ifndef Q_OS_WIN
                        << "SID File(*.sid *.mus *.str *.prg *.P00)"
#endif
                        << "FFmpeg File(*.wma *.ape *.tta *.m4a *.ra *.shn *.vqf *.ac3 *.tak)"
                        << "ModPlug File(*.mod *.s3m *.xm *.it *.669 *.amf *.ams *.dbm *.dmf *.dsm *.far *.mdl *.med *.mtm "
                           "*.okt *.ptm *.stm *.ult *.umx *.mt2 *.psm *.mdz *.s3z *.xmz *.itz *.mdr *.s3r *.xmr *.itr "
                           "*.dgz *.s3gz *.xmgz *.itgz)";
}

QStringList MusicFormats::supportFormatsSpekString()
{
    return QStringList()<< "*.3gp *.aac *.aif *.aifc *.aiff *.amr *.awb *.ape *.au *.dts *.flac "
                           "*.flv *.gsm *.m4a *.m4p *.mp3 *.mp4 *.mp+ *.mpc *.mpp *.oga *.ogg *.opus *.ra "
                           "*.ram *.snd *.wav *.wma *.wv";
}

QStringList MusicFormats::supportFormatsPlaylistDialogString()
{
    return QStringList()<< "File(*.tkpl *.m3u *.m3u8 *.pls *.wpl *.xspf *.asx *.kwl *.kgl *.fpl)"
                        << "TTKlist File(*.tkpl)" << "M3U File(*.m3u)" << "M3U8 File(*.m3u8)" << "Playlist File(*.pls)"
                        << "Windows Playlist File(*.wpl)" << "XML Shareable Playlist File(*.xspf)" << "Foobar2k Playlist File(*.fpl)"
                        << "Windows Media Playlist File(*.asx)" << "KuWo Playlist File(*.kwl)" << "KuGou Playlist File(*.kgl)";
}

QString MusicFormats::supportFormatsPlaylistString()
{
    return QString ("TTKlist File(*.tkpl);;M3U File(*.m3u);;M3U8 File(*.m3u8);;Playlist File(*.pls);;Foobar2k Playlist File(*.fpl);;"
                    "Windows Playlist File(*.wpl);;XML Shareable Playlist File(*.xspf);;Windows Media Playlist File(*.asx)");
}
