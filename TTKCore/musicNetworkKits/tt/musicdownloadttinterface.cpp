#include "musicdownloadttinterface.h"
#include "musicnumberutils.h"
#include "musictime.h"

void MusicDownLoadTTInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute songAttr;
    songAttr.m_url = key["url"].toString();
    songAttr.m_size = MusicUtils::Number::size2Label(key["size"].toInt());
    songAttr.m_format = key["suffix"].toString();
    songAttr.m_bitrate = bitrate == -1 ? key["bitRate"].toInt() : bitrate;
    songAttr.m_duration = MusicTime::msecTime2LabelJustified(key["duration"].toInt());
    info->m_songAttrs << songAttr;
}

void MusicDownLoadTTInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, const QString &quality, bool all)
{
    ///music normal songs urls
    QVariantList auditions = key["auditionList"].toList();
    foreach(const QVariant &audition, auditions)
    {
        QVariantMap audUrlsValue = audition.toMap();
        if(audUrlsValue.isEmpty())
        {
            continue;
        }

        if(all)
        {
            readFromMusicSongAttribute(info, audUrlsValue, -1);
        }
        else
        {
            int bit = map2NormalBitrate(audUrlsValue["bitRate"].toInt());
            if(quality == QObject::tr("ST") && bit == MB_32)
            {
                readFromMusicSongAttribute(info, audUrlsValue, bit);
            }
            else if(quality == QObject::tr("SD") && bit == MB_128)
            {
                readFromMusicSongAttribute(info, audUrlsValue, bit);
            }
            else if(quality == QObject::tr("HQ") && bit == MB_192)
            {
                readFromMusicSongAttribute(info, audUrlsValue, bit);
            }
            else if(quality == QObject::tr("SQ") && bit == MB_320)
            {
                readFromMusicSongAttribute(info, audUrlsValue, bit);
            }
        }
    }
    ///music cd songs urls
    QVariantList llUrls = key["llList"].toList();
    foreach(const QVariant &llUrl, llUrls)
    {
        QVariantMap llUrlValue = llUrl.toMap();
        if(llUrlValue.isEmpty())
        {
            continue;
        }

        if(all)
        {
            readFromMusicSongAttribute(info, llUrlValue, -1);
        }
        else
        {
            int bit = map2NormalBitrate(llUrlValue["bitRate"].toInt());
            if(quality == QObject::tr("CD") && bit == MB_500)
            {
                readFromMusicSongAttribute(info, llUrlValue, bit);
            }
        }
    }
}

int MusicDownLoadTTInterface::map2NormalBitrate(int bitrate)
{
    if(bitrate > MB_0 && bitrate <= MB_64)
        return MB_32;
    else if(bitrate > MB_64 && bitrate < MB_128)
        return MB_128;
    else if(bitrate > MB_128 && bitrate < MB_192)
        return MB_192;
    else if(bitrate > MB_192 && bitrate < MB_320)
        return MB_320;
    else if(bitrate > MB_320)
        return MB_500;
    else
        return bitrate;
}
