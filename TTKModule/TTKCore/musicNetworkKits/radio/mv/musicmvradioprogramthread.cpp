#include "musicmvradioprogramthread.h"
#include "musicdownloadkginterface.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musiccoreutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicMVRadioProgramThread::MusicMVRadioProgramThread(QObject *parent)
    : MusicMVRadioThreadAbstract(parent)
{

}

void MusicMVRadioProgramThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        bytes = QString(bytes).split("var mvfmdata = ").back().split("$img = ").front().toUtf8();
        bytes.chop(3);

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            bool contains = false;
            foreach(const QVariant &var, data.toList())
            {
                if(m_interrupt) return;

                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                MusicResultsItem item;
                item.m_nickName = value["className"].toString();

                foreach(const QVariant &var, value["fm_list"].toList())
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    if(!contains && value["fmId"].toString() == m_searchText)
                    {
                        contains = true;
                        item.m_name = value["fmName"].toString();
                        item.m_id = value["fmId"].toString();
                        item.m_coverUrl = value["imgUrlMv"].toString();

                        emit createCategoryItem(item);

                        foreach(const QVariant &var, value["mvs"].toList())
                        {
                            if(m_interrupt) return;

                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            MusicObject::MusicSongInformation musicInfo;
                            musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["name"].toString());
                            musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["name"].toString());
                            if(musicInfo.m_singerName.contains(" - "))
                            {
                                const QStringList &ds = musicInfo.m_singerName.split(" - ");
                                if(ds.count() >= 2)
                                {
                                    musicInfo.m_singerName = ds.front();
                                    musicInfo.m_songName = ds.back();
                                }
                            }
                            musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["time"].toInt());

                            musicInfo.m_songId = value["mvhash"].toString();
                            if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                            readFromMusicMVAttribute(&musicInfo);
                            if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                            if(musicInfo.m_songAttrs.isEmpty())
                            {
                                continue;
                            }

                            MusicSearchedItem item;
                            item.m_songName = musicInfo.m_songName;
                            item.m_singerName = musicInfo.m_singerName;
                            item.m_time = musicInfo.m_timeLength;
                            item.m_albumName.clear();
                            item.m_type.clear();
                            emit createSearchedItem(item);
                            m_musicSongInfos << musicInfo;
                        }
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicMVRadioProgramThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    const QByteArray &encodedData = MusicUtils::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8()).toHex().toLower();
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KG_MV_ATTR_URL, false).arg(QString(encodedData)).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(!value.isEmpty() && value.contains("mvdata"))
        {
            value = value["mvdata"].toMap();
            QVariantMap mv = value["sd"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVAttribute(info, mv);
            }
            mv = value["hd"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVAttribute(info, mv);
            }
            mv = value["sq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVAttribute(info, mv);
            }
            mv = value["rq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVAttribute(info, mv);
            }
        }
    }
}

void MusicMVRadioProgramThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["downurl"].toString();
    attr.m_size = MusicUtils::Number::size2Label(key["filesize"].toInt());
    attr.m_format = MusicUtils::String::StringSplite(attr.m_url);

    int bitRate = key["bitrate"].toInt()/1000;
    if(bitRate <= 375)
        attr.m_bitrate = MB_250;
    else if(bitRate > 375 && bitRate <= 625)
        attr.m_bitrate = MB_500;
    else if(bitRate > 625 && bitRate <= 875)
        attr.m_bitrate = MB_750;
    else if(bitRate > 875)
        attr.m_bitrate = MB_1000;

    attr.m_duration = MusicTime::msecTime2LabelJustified(key["timelength"].toInt());
    info->m_songAttrs.append(attr);
}
