#include "musickwqueryinterface.h"
#include "musicabstractnetwork.h"

#include "qalgorithm/qdeswrapper.h"

void MusicKWInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}


void MusicKWQueryInterface::readFromMusicLLAttribute(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QDesWrapper des;
    const QByteArray &parameter = des.encrypt(MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_DATA_URL, false).arg(info->m_songId).arg(suffix).arg(format).toUtf8(),
                                              MusicUtils::Algorithm::mdII(_SIGN, ALG_UNIMP_KEY, false).toUtf8());
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_MOVIE_URL, false).arg(QString(parameter)));
    MusicKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    if(!bytes.isEmpty() && !bytes.contains("res not found"))
    {
        const QString text(bytes);
        QRegExp regx(".*url=(.*)\r\nsig=");

        if(text.indexOf(regx) != -1)
        {
            MusicObject::MusicSongAttribute attr;
            attr.m_url = regx.cap(1);
            attr.m_bitrate = bitrate;
            attr.m_format = suffix;
            attr.m_size = TTK_DEFAULT_STR;
            if(attr.m_url.isEmpty() || info->m_songAttrs.contains(attr))
            {
                return;
            }

            info->m_songAttrs.append(attr);
        }
    }
}

void MusicKWQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    if(format.contains("MP3128") && bitrate == MB_128 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = TTK_DEFAULT_STR;
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MP3192") && bitrate == MB_192 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = TTK_DEFAULT_STR;
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("MP3H") && bitrate == MB_320 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = TTK_DEFAULT_STR;
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("ALFLAC") && bitrate == MB_1000 && suffix == FLAC_FILE_PREFIX)
    {
        readFromMusicLLAttribute(info, FLAC_FILE_PREFIX, "2000kflac", bitrate);
    }
    else if(format.contains("AL") && bitrate == MB_750 && suffix == APE_FILE_PREFIX)
    {
        readFromMusicLLAttribute(info, APE_FILE_PREFIX, "1000kape", bitrate);
    }
}

void MusicKWQueryInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, const QString &suffix, const QString &format, int bitrate)
{
    if(format.contains("128kmp3") && bitrate == MB_128 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = TTK_DEFAULT_STR;
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("192kmp3") && bitrate == MB_192 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = TTK_DEFAULT_STR;
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("320kmp3") && bitrate == MB_320 && suffix == MP3_FILE_PREFIX)
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = suffix;
        attr.m_size = TTK_DEFAULT_STR;
        attr.m_url = MusicUtils::Algorithm::mdII(KW_SONG_DETAIL_URL, false).arg(bitrate).arg(info->m_songId);
        info->m_songAttrs.append(attr);
    }
    else if(format.contains("2000kflac") && bitrate == MB_1000 && suffix == FLAC_FILE_PREFIX)
    {
        readFromMusicLLAttribute(info, FLAC_FILE_PREFIX, "2000kflac", bitrate);
    }
    else if(format.contains("1000kape") && bitrate == MB_750 && suffix == APE_FILE_PREFIX)
    {
        readFromMusicLLAttribute(info, APE_FILE_PREFIX, "1000kape", bitrate);
    }
}

void MusicKWQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &format, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, MP3_FILE_PREFIX, format, MB_128);
        readFromMusicSongAttribute(info, MP3_FILE_PREFIX, format, MB_192);
        readFromMusicSongAttribute(info, MP3_FILE_PREFIX, format, MB_320);
        readFromMusicSongAttribute(info, APE_FILE_PREFIX, format, MB_750);
        readFromMusicSongAttribute(info, FLAC_FILE_PREFIX, format, MB_1000);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, MP3_FILE_PREFIX, format, MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, MP3_FILE_PREFIX, format, MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, MP3_FILE_PREFIX, format, MB_320);
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, APE_FILE_PREFIX, format, MB_750);
            readFromMusicSongAttribute(info, FLAC_FILE_PREFIX, format, MB_1000);
        }
    }
}

void MusicKWQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantList &format, const QString &quality, bool all)
{
    for(const QVariant &var : qAsConst(format))
    {
        const QString &fs = var.toString();
        if(all)
        {
            readFromMusicSongAttributeNew(info, MP3_FILE_PREFIX, fs, MB_128);
            readFromMusicSongAttributeNew(info, MP3_FILE_PREFIX, fs, MB_192);
            readFromMusicSongAttributeNew(info, MP3_FILE_PREFIX, fs, MB_320);
            readFromMusicSongAttributeNew(info, APE_FILE_PREFIX, fs, MB_750);
            readFromMusicSongAttributeNew(info, FLAC_FILE_PREFIX, fs, MB_1000);
        }
        else
        {
            if(quality == QObject::tr("SD"))
            {
                readFromMusicSongAttributeNew(info, MP3_FILE_PREFIX, fs, MB_128);
            }
            else if(quality == QObject::tr("HQ"))
            {
                readFromMusicSongAttributeNew(info, MP3_FILE_PREFIX, fs, MB_192);
            }
            else if(quality == QObject::tr("SQ"))
            {
                readFromMusicSongAttributeNew(info, MP3_FILE_PREFIX, fs, MB_320);
            }
            else if(quality == QObject::tr("CD"))
            {
                readFromMusicSongAttributeNew(info, APE_FILE_PREFIX, fs, MB_750);
                readFromMusicSongAttributeNew(info, FLAC_FILE_PREFIX, fs, MB_1000);
            }
        }
    }
}

void MusicKWQueryInterface::readFromMusicSongPicture(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info->m_songId));
    MusicKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            value = value["songinfo"].toMap();
            info->m_smallPicUrl = value["pic"].toString();
            if(!info->m_smallPicUrl.contains(TTK_HTTP) && !info->m_smallPicUrl.contains(TTK_NULL_STR))
            {
                info->m_smallPicUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false) + info->m_smallPicUrl;
            }

            if(info->m_duration.isEmpty())
            {
                info->m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);
            }
        }
    }
}
