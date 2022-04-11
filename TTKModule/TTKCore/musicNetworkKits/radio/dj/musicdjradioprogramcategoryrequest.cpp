#include "musicdjradioprogramcategoryrequest.h"
#include "musicabstractdjradiorequest.h"

MusicDJRadioProgramCategoryRequest::MusicDJRadioProgramCategoryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicDJRadioProgramCategoryRequest::startToSearch(QueryType type, const QString &value)
{
    if(type == MusicQuery)
    {
        startToSearch(value);
    }
    else
    {
        m_queryValue = value;
        startToPage(0);
    }
}

void MusicDJRadioProgramCategoryRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(DJ_RADIO_LIST_URL, false).arg(m_queryValue));
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicDJRadioProgramCategoryRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(DJ_DETAIL_URL, false),
                      MusicUtils::Algorithm::mdII(DJ_DETAIL_DATA_URL, false).arg(value));

    QNetworkReply *reply = m_manager.post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicDJRadioProgramCategoryRequest::queryProgramInfo(MusicResultsItem &item)
{
    TTK_LOGGER_INFO(QString("%1 queryProgramInfo %2").arg(className(), item.m_id));

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(DJ_PROGRAM_INFO_URL, false),
                      MusicUtils::Algorithm::mdII(DJ_PROGRAM_INFO_DATA_URL, false).arg(item.m_id));

    const QByteArray &bytes = MusicObject::syncNetworkQueryForPost(&request, parameter);
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

void MusicDJRadioProgramCategoryRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        m_totalSize = m_pageSize;

        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("djRadios"))
            {
                const QVariantList &datas = value["djRadios"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = QString::number(value["id"].toInt());

                    info.m_coverUrl = value["picUrl"].toString();
                    info.m_name = value["name"].toString();
                    value = value["dj"].toMap();
                    info.m_nickName = value["nickname"].toString();
                    Q_EMIT createProgramItem(info);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicDJRadioProgramCategoryRequest::downloadDetailsFinished()
{
    TTK_LOGGER_INFO(QString("%1 downloadDetailsFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("programs"))
            {
                bool categoryFound = false;
                //
                const QVariantList &datas = value["programs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt());

                    const QVariantMap &radioObject = value["radio"].toMap();
                    musicInfo.m_coverUrl = radioObject["picUrl"].toString();
                    musicInfo.m_artistId = QString::number(radioObject["id"].toInt());
                    musicInfo.m_singerName = MusicUtils::String::charactersReplaced(radioObject["name"].toString());

                    const QVariantMap &mainSongObject = value["mainSong"].toMap();
                    musicInfo.m_songId = QString::number(mainSongObject["id"].toInt());

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongProperty(&musicInfo, mainSongObject, m_queryQuality, true);
                    TTK_NETWORK_QUERY_CHECK();
                    //
                    if(!categoryFound)
                    {
                        categoryFound = true;
                        MusicResultsItem info;
                        info.m_name = musicInfo.m_songName;
                        info.m_nickName = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_coverUrl;
                        info.m_playCount = QString::number(radioObject["subCount"].toInt());
                        info.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["createTime"].toULongLong()).toString(MUSIC_YEAR_FORMAT);
                        Q_EMIT createCategoryInfoItem(info);
                    }
                    //
                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName.clear();
                    item.m_duration = musicInfo.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
}
