#include "musicdownloadquerykgmoviethread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musiccoreutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKGMovieThread::MusicDownLoadQueryKGMovieThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Kugou";
}

QString MusicDownLoadQueryKGMovieThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKGMovieThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_SONG_SEARCH_URL, false).arg(text).arg(0).arg(50);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKGMovieThread::startToSingleSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(getClassName()).arg(text));
    m_searchText = text.trimmed();
    deleteAll();
    m_interrupt = true;

    QTimer::singleShot(MT_MS, this, SLOT(singleDownLoadFinished()));
}

void MusicDownLoadQueryKGMovieThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["info"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = value["singername"].toString();
                    musicInfo.m_songName = value["songname"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);

                    musicInfo.m_songId = value["mvhash"].toString();
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicMVAttribute(&musicInfo, false);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItems(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    ///extra yyt movie
    if(m_queryExtraMovie && m_currentType == MovieQuery)
    {
        MusicSemaphoreLoop loop;
        MusicDownLoadQueryYYTThread *yyt = new MusicDownLoadQueryYYTThread(this);
        connect(yyt, SIGNAL(createSearchedItems(MusicSearchedItem)), SIGNAL(createSearchedItems(MusicSearchedItem)));
        connect(yyt, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        yyt->startToSearch(MusicDownLoadQueryYYTThread::MovieQuery, m_searchText);
        loop.exec();
        m_musicSongInfos << yyt->getMusicSongInfos();
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKGMovieThread::singleDownLoadFinished()
{
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    MusicObject::MusicSongInformation musicInfo;
    musicInfo.m_songId = m_searchText;
    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    readFromMusicMVInfo(&musicInfo);
    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    readFromMusicMVAttribute(&musicInfo, true);
    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

    if(!musicInfo.m_songAttrs.isEmpty())
    {
        MusicSearchedItem item;
        item.m_songName = musicInfo.m_songName;
        item.m_singerName = musicInfo.m_singerName;
        item.m_time = findTimeStringByAttrs(musicInfo.m_songAttrs);
        item.m_type = mapQueryServerString();
        emit createSearchedItems(item);
        m_musicSongInfos << musicInfo;
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKGMovieThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, bool more)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    QByteArray encodedData = MusicUtils::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8()).toHex().toLower();
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_MV_ATTR_URL, false).arg(QString(encodedData)).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
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
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(!value.isEmpty() && value.contains("mvdata"))
        {
            if(more)
            {
                info->m_songName = value["songname"].toString();
                info->m_singerName = value["singer"].toString();
            }

            value = value["mvdata"].toMap();
            QVariantMap mv = value["sd"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVInfoAttribute(info, mv);
            }
            mv = value["hd"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVInfoAttribute(info, mv);
            }
            mv = value["sq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVInfoAttribute(info, mv);
            }
            mv = value["rq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVInfoAttribute(info, mv);
            }
        }
    }
}

void MusicDownLoadQueryKGMovieThread::readFromMusicMVInfo(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_MV_HOME_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get( request );
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QString text(reply->readAll());
    QRegExp regx("mv_hash = \"([^\"]+)");
    int pos = text.indexOf(regx);
    while(pos != -1)
    {
        info->m_songId = regx.cap(1);
        pos += regx.matchedLength();
        pos = regx.indexIn(text, pos);
        break;
    }
}

void MusicDownLoadQueryKGMovieThread::readFromMusicMVInfoAttribute(MusicObject::MusicSongInformation *info,
                                                                   const QVariantMap &key)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["downurl"].toString();
    attr.m_size = MusicUtils::Number::size2Label(key["filesize"].toInt());
    attr.m_format = MusicUtils::Core::fileSuffix(attr.m_url);

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

QString MusicDownLoadQueryKGMovieThread::findTimeStringByAttrs(const MusicObject::MusicSongAttributes &attrs)
{
    foreach(const MusicObject::MusicSongAttribute &attr, attrs)
    {
        if(!attr.m_duration.isEmpty())
        {
            return attr.m_duration;
        }
    }

    return QString("-");
}
