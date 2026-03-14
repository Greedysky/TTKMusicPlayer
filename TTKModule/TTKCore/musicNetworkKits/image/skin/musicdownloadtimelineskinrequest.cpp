#include "musicdownloadtimelineskinrequest.h"

#include "qalgorithm/aeswrapper.h"

static constexpr const char *QUERY_URL = "M1JaOE5SaGtNaUlXTFpWaEFXR3RqRXdPYTJLT2JTbDh3c1JEeE1JbUxaSWJKTzRCaHVyaXVKTkVFWkh1UURXam04Zkl5YVBsc1FqbVJXL1VxNXJkd2g4M3F5dXV1Qmc4NEVXVkpRPT0=";

MusicDownloadTimeLineSkinRequest::MusicDownloadTimeLineSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownloadTimeLineSkinRequest::startToRequest()
{

}

void MusicDownloadTimeLineSkinRequest::startToRequest(const QString &id)
{
    m_topic = id;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false).arg(id).arg(TTKDateTime::currentTimestamp()));
    request.setRawHeader(TTK::Algorithm::mdII("MzVmOTV3TEdxd2QyUkNNeFQyVTZrQ0tTRU4wPQ==", false).toUtf8(), {});
    request.setRawHeader(TTK::Algorithm::mdII("UVBiT0kwemtON3E5d3RCUFlsemo5Zz09", false).toUtf8(), {});
    request.setRawHeader(TTK::Algorithm::mdII("QkpiVE1qLzVMOStsT2hmdVJSZnAvRlRHeWRVPQ==", false).toUtf8(), TTK::Algorithm::mdII("Z1d3dnM2dkxhWHNxdkFQbFFRTjVVUT09", false).toUtf8());
    request.setRawHeader(TTK::Algorithm::mdII("TUF0cmhudmg3ZGU0ZXgxSmRRK2E0QUpOSWdFPQ==", false).toUtf8(), TTK::Algorithm::mdII("MHljMDBxdThkNzU3SWNWZDhqNzlrL1BnVXYvbUtHdzIzWTJQSE11WlhOblJRY09X", false).toUtf8());

    TTK::setUserAgentHeader(&request);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDownloadTimeLineSkinRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicSkinRemoteGroupList groups;
    MusicAbstractDownloadSkinRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["status"].toInt() == 1 && value.contains("data"))
            {
                MusicSkinRemoteGroup group;
                group.m_id = m_topic;
                group.m_name = m_topic;
                group.m_type = MusicSkinRemoteGroup::TimeLine;

                int index = 0;
                const QVariantList &datas = value["data"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    const QString &imgurl = value["imgurl"].toString();
                    const QString &provider = value["rawprovider"].toString();
                    const QString &ext = TTK::String::suffix(imgurl);
                    const QString &basename = TTK::String::splitToken(imgurl, TTK_SEPARATOR, TTK_DOT);

                    if(basename.length() < 32)
                    {
                        continue;
                    }

                    const QString &cipher = basename.left(32);
                    const QString &rest = basename.mid(32);

                    QString key = provider;
                    for(int i = 0; i < 16; ++i)
                    {
                        key.append(provider);
                    }

                    key = key.right(16);
                    const QString &iv = TTK::Algorithm::md5(key.toUtf8()).mid(8, 16);

                    QAlgorithm::Aes aes;
                    const QByteArray &name = aes.decryptCBC(cipher.toUtf8(), key.toUtf8(), iv.toUtf8(), true);

                    MusicSkinRemoteItem item;
                    item.m_index = index++;
                    item.m_useCount = value["score"].toInt();
                    item.m_url = imgurl.left(imgurl.lastIndexOf(TTK_SEPARATOR)) + TTK_SEPARATOR + name + rest + "." + ext;

                    TTK_INFO_STREAM( item.m_url);

                    if(item.isValid())
                    {
                        group.m_items << item;
                    }
                }

                if(group.isValid())
                {
                    groups << group;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged(groups);
    deleteAll();
}
