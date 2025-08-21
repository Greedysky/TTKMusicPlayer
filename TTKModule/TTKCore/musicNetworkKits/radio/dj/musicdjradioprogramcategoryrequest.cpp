#include "musicdjradioprogramcategoryrequest.h"
#include "musicabstractdjradiorequest.h"

MusicDJRadioProgramCategoryRequest::MusicDJRadioProgramCategoryRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{
    m_pageSize = SONG_PAGE_SIZE;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicDJRadioProgramCategoryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(DJ_RADIO_LIST_URL, false).arg(m_queryValue));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDJRadioProgramCategoryRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(DJ_DETAIL_URL, false),
                      TTK::Algorithm::mdII(DJ_DETAIL_DATA_URL, false).arg(value));

    QNetworkReply *reply = m_manager.post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicDJRadioProgramCategoryRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    m_queryValue = value;
    startToPage(0);
}

void MusicDJRadioProgramCategoryRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicDJRadioProgramCategoryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        m_totalSize = m_pageSize;

        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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

                    MusicResultDataItem item;
                    item.m_id = value["id"].toString();
                    item.m_coverUrl = value["picUrl"].toString();
                    item.m_name = value["name"].toString();

                    value = value["dj"].toMap();
                    item.m_nickName = value["nickname"].toString();
                    Q_EMIT createProgramItem(item);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicDJRadioProgramCategoryRequest::downloadDetailsFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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
                    const QVariantMap &mainSongObject = value["mainSong"].toMap();
                    info.m_songId = mainSongObject["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    const QVariantMap &radioObject = value["radio"].toMap();
                    info.m_artistId = radioObject["id"].toString();
                    info.m_artistName = ReqWYInterface::makeSongArtist(info.m_artistName, radioObject["name"].toString());

                    info.m_coverUrl = radioObject["picUrl"].toString();
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt());

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, mainSongObject);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!categoryFound)
                    {
                        categoryFound = true;
                        MusicResultDataItem item;
                        item.m_name = info.m_songName;
                        item.m_nickName = info.m_artistName;
                        item.m_coverUrl = info.m_coverUrl;
                        item.m_count = radioObject["subCount"].toString();
                        item.m_time = TTKDateTime::format(value["createTime"].toULongLong(), TTK_DATE_FORMAT);
                        Q_EMIT createCategoryItem(item);
                    }

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
}
