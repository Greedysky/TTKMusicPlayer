#include "musicdownloadqqinterface.h"
#include "musiccryptographichash.h"
#include "musicnumberdefine.h"
#include "musicnumberutils.h"

void MusicDownLoadQQInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, int bitrate)
{
    QString mid = key["strMediaMid"].toString();
    if(mid.isEmpty())
    {
        mid = key["songmid"].toString();
    }

    if(key["size128"].toULongLong() != 0 && bitrate == MB_128)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_128_URL, URL_KEY).arg(key["songid"].toULongLong() + 30000000);
        attr.m_size = MusicUtils::Number::size2Label(key["size128"].toULongLong());
        attr.m_format = "mp3";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeogg"].toULongLong() != 0 && bitrate == MB_192)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_192_URL, URL_KEY).arg(key["songid"].toULongLong() + 40000000);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeogg"].toULongLong());
        attr.m_format = "ogg";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["size320"].toULongLong() != 0 && bitrate == MB_320)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_320_URL, URL_KEY).arg(mid);
        attr.m_size = MusicUtils::Number::size2Label(key["size320"].toULongLong());
        attr.m_format = "mp3";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeape"].toULongLong() != 0 && bitrate == MB_500)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_APE_URL, URL_KEY).arg(mid);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeape"].toULongLong());
        attr.m_format = "ape";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
    else if(key["sizeflac"].toULongLong() != 0 && bitrate == MB_1000)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_url = MusicCryptographicHash::decryptData(QQ_SONG_FLAC_URL, URL_KEY).arg(mid);
        attr.m_size = MusicUtils::Number::size2Label(key["sizeflac"].toULongLong());
        attr.m_format = "flac";
        attr.m_bitrate = bitrate;
        info->m_songAttrs.append(attr);
    }
}
