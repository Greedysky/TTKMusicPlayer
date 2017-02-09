#include "musicdownloadttinterface.h"
#include "musicnumberutils.h"
#include "musictime.h"

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

        if(all == true || (all == false &&
           audUrlsValue["typeDescription"].toString() == quality))
        {
            MusicObject::MusicSongAttribute songAttr;
            songAttr.m_url = audUrlsValue["url"].toString();
            songAttr.m_size = MusicUtils::Number::size2Label(audUrlsValue["size"].toInt());
            songAttr.m_format = audUrlsValue["suffix"].toString();
            songAttr.m_bitrate = audUrlsValue["bitRate"].toInt();
            songAttr.m_duration = MusicTime::msecTime2LabelJustified(audUrlsValue["duration"].toInt());
            info->m_songAttrs << songAttr;
            if(!all)
            {
                break;
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

        if(all == true || (all == false &&
           llUrlValue["typeDescription"].toString() == quality))
        {
            MusicObject::MusicSongAttribute songAttr;
            songAttr.m_url = llUrlValue["url"].toString();
            songAttr.m_size = MusicUtils::Number::size2Label(llUrlValue["size"].toInt());
            songAttr.m_format = llUrlValue["suffix"].toString();
            songAttr.m_bitrate = llUrlValue["bitRate"].toInt();
            songAttr.m_duration = MusicTime::msecTime2LabelJustified(key["duration"].toInt());
            info->m_songAttrs << songAttr;
            if(!all)
            {
                break;
            }
        }
    }
}
