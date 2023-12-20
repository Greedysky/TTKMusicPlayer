#include "musicdjradioprogramcategoryrequest.h"
#include "musicabstractdjradiorequest.h"

MusicDJRadioProgramCategoryRequest::MusicDJRadioProgramCategoryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicDJRadioProgramCategoryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(DJ_RADIO_LIST_URL, false).arg(m_queryValue));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicDJRadioProgramCategoryRequest::startToSearch(QueryType type, const QString &value)
{
    if(type == QueryType::Music)
    {
        startToSearch(value);
    }
    else
    {
        m_queryValue = value;
        startToPage(0);
    }
}

void MusicDJRadioProgramCategoryRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(DJ_DETAIL_URL, false),
                      TTK::Algorithm::mdII(DJ_DETAIL_DATA_URL, false).arg(value));

    QNetworkReply *reply = m_manager.post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError);
}

void MusicDJRadioProgramCategoryRequest::queryProgramInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(QString("%1 queryProgramInfo %2").arg(className(), item.m_id));

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(DJ_PROGRAM_INFO_URL, false),
                      TTK::Algorithm::mdII(DJ_PROGRAM_INFO_DATA_URL, false).arg(item.m_id));

    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok = false;
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
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        m_totalSize = m_pageSize;

        QJson::Parser json;
        bool ok = false;
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

                    MusicResultDataItem result;
                    result.m_id = value["id"].toString();
                    result.m_coverUrl = value["picUrl"].toString();
                    result.m_name = value["name"].toString();
                    value = value["dj"].toMap();
                    result.m_nickName = value["nickname"].toString();
                    Q_EMIT createProgramItem(result);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicDJRadioProgramCategoryRequest::downloadDetailsFinished()
{
    TTK_INFO_STREAM(QString("%1 downloadDetailsFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("programs"))
            {
                bool categoryFound = false;

                const QVariantList &datas = value["programs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt());

                    const QVariantMap &radioObject = value["radio"].toMap();
                    info.m_coverUrl = radioObject["picUrl"].toString();
                    info.m_artistId = radioObject["id"].toString();
                    info.m_singerName = TTK::String::charactersReplace(radioObject["name"].toString());

                    const QVariantMap &mainSongObject = value["mainSong"].toMap();
                    info.m_songId = mainSongObject["id"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    MusicWYInterface::parseFromSongProperty(&info, mainSongObject, true);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!categoryFound)
                    {
                        categoryFound = true;
                        MusicResultDataItem result;
                        result.m_name = info.m_songName;
                        result.m_nickName = info.m_singerName;
                        result.m_coverUrl = info.m_coverUrl;
                        result.m_playCount = radioObject["subCount"].toString();
                        result.m_updateTime = TTKDateTime::format(value["createTime"].toULongLong(), TTK_YEAR_FORMAT);
                        Q_EMIT createCategoryItem(result);
                    }

                    if(info.m_songProps.isEmpty())
                    {
                        continue;
                    }

                    MusicResultInfoItem item;
                    item.m_songName = info.m_songName;
                    item.m_singerName = info.m_singerName;
                    item.m_albumName.clear();
                    item.m_duration = info.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
}
