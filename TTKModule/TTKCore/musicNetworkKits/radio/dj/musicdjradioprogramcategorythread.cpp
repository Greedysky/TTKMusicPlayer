#include "musicdjradioprogramcategorythread.h"
#include "musicdjradiothreadabstract.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDJRadioProgramCategoryThread::MusicDJRadioProgramCategoryThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicDJRadioProgramCategoryThread::startToSearch(QueryType type, const QString &category)
{
    if(type == MusicQuery)
    {
        startToSearch(category);
    }
    else
    {
        m_searchText = category;
        startToPage(0);
    }
}

void MusicDJRadioProgramCategoryThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    const QUrl &musicUrl(MusicUtils::Algorithm::mdII(DJ_RADIO_LIST_URL, false).arg(m_searchText));
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDJRadioProgramCategoryThread::startToSearch(const QString &category)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(category));
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(DJ_DETAIL_N_URL, false),
                      MusicUtils::Algorithm::mdII(DJ_DETAIL_NDT_URL, false).arg(category));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    QNetworkReply *reply = m_manager->post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDJRadioProgramCategoryThread::getProgramInfo(MusicResultsItem &item)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 getProgramInfo %2").arg(getClassName()).arg(item.m_id));

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(DJ_PROGRAM_INFO_N_URL, false),
                      MusicUtils::Algorithm::mdII(DJ_PROGRAM_INFO_NDT_URL, false).arg(item.m_id));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->post(request, parameter);
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
        if(value["code"].toInt() == 200 && value.contains("djRadio"))
        {
            value = value["djRadio"].toMap();
            item.m_coverUrl = value["picUrl"].toString();
            item.m_name = value["name"].toString();

            value = value["dj"].toMap();
            item.m_nickName = value["nickname"].toString();
        }
    }
}

void MusicDJRadioProgramCategoryThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        m_pageTotal = m_pageSize;
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("djRadios"))
            {
                const QVariantList &datas = value["djRadios"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicResultsItem info;
                    info.m_id = QString::number(value["id"].toInt());

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    info.m_coverUrl = value["picUrl"].toString();
                    info.m_name = value["name"].toString();
                    value = value["dj"].toMap();
                    info.m_nickName = value["nickname"].toString();
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    emit createProgramItem(info);
                }
            }
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDJRadioProgramCategoryThread::getDetailsFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    M_LOGGER_INFO(QString("%1 getDetailsFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(reply && reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("programs"))
            {
                bool categoryFlag = false;
                //
                const QVariantList &datas = value["programs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["name"].toString());
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());

                    const QVariantMap &radioObject = value["radio"].toMap();
                    musicInfo.m_smallPicUrl = radioObject["picUrl"].toString();
                    musicInfo.m_artistId = QString::number(radioObject["id"].toInt());
                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(radioObject["name"].toString());

                    const QVariantMap &mainSongObject = value["mainSong"].toMap();
                    musicInfo.m_songId = QString::number(mainSongObject["id"].toInt());

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    readFromMusicSongAttribute(&musicInfo, mainSongObject, m_searchQuality, true);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    //
                    if(!categoryFlag)
                    {
                        categoryFlag = true;
                        MusicResultsItem info;
                        info.m_name = musicInfo.m_songName;
                        info.m_nickName = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_smallPicUrl;
                        info.m_playCount = QString::number(radioObject["subCount"].toInt());
                        info.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["createTime"].toULongLong()).toString("yyyy-MM-dd");
                        emit createCategoryInfoItem(info);
                    }
                    //
                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName.clear();
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    M_LOGGER_INFO(QString("%1 getDetailsFinished deleteAll").arg(getClassName()));
}
