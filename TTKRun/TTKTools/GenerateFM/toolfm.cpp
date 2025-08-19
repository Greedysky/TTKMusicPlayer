#include "toolfm.h"

static constexpr const char *QT_BASE_URL = "TDNBRFd3S3lvRnVBNzFCblJSQmRHRFhrdVNnQk9qMlBEbzdjWUpSdS95VT0=";
static constexpr const char *QT_CATEGORY_URL = "N2VwdWNwUGxOL0ZWZCtrN3V6ekd4SUZqUXVQZ3p1Y2pyeEpsc01RZzR6blNnK0o5L3Q4NWRPRCswVzkrTURKV1VvdzMvQkJ4VEVrPQ==";
static constexpr const char *QT_CHANNEL_URL = "UHJCNEJXWlZhQVowVE1ndXZnajFKQ2tsb1c1SDdlaEE0a0pDWEM2RGJMaktmWXpNZFR6WTcxbmRNb0ZTRTVwQXo5cE92REJOMlhjPQ==";
static constexpr const char *QT_SONG_URL = "OTB3emRpcnNRVDI3eVBtZ2Nwbk9jZXQyQ2hUZjJ1ZjdFdEJIN1V4WHU1WnUvMjk4MEtNWFYrUXRwUlZkWG96TA==";
static constexpr const char *XI_SONG_URL = "ZE1EK2xZcVdTd0hpaksxQXFob0Jjdk8zTSswSU5lRnlZTzRNZ3dkNUNXND0=";

ToolQTFM::ToolQTFM(QObject *parent)
    : MusicPageQueryRequest(parent)
{
    m_pageSize = 12;
}

void ToolQTFM::startToRequest()
{
    TTK_INFO_STREAM("Start to request QingTing fm data");

    deleteAll();
    m_pageIndex = -1;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(QT_BASE_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request, "application/json;charset=utf-8");

    m_reply = m_manager.post(request, TTK::Algorithm::mdII(QT_CATEGORY_URL, false).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void ToolQTFM::downLoadFinished()
{
    MusicFMCategoryList items;
    MusicAbstractNetwork::downLoadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["radioPage"].toMap();
                const QVariantList &datas = value["classes"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    m_pageIndex = -1;
                    MusicFMCategory category;
                    category.m_category = value["title"].toString();
                    startToListRequest(value["id"].toInt(), &category.m_items);
                    items << category;
                }
            }
        }
    }

    MusicFMConfigManager manager;
    if(manager.load("qingtingfm.xml"))
    {
        manager.writeBuffer(items);
        TTK_INFO_STREAM("Write QingTing fm data done");
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void ToolQTFM::startToListRequest(int id, MusicFMChannelList *channels)
{
    do
    {
        QNetworkRequest request;
        request.setUrl(TTK::Algorithm::mdII(QT_BASE_URL, false));
        TTK::setSslConfiguration(&request);
        TTK::makeContentTypeHeader(&request, "application/json;charset=utf-8");

        const QByteArray &parameter = TTK::Algorithm::mdII(QT_CHANNEL_URL, false).arg(id).arg(++m_pageIndex + 1).toUtf8();
        const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
        if(bytes.isEmpty())
        {
            return;
        }

        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["radioPage"].toMap();
                value = value["contents"].toMap();

                m_totalSize = value["count"].toInt();

                const QVariantList &datas = value["items"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicFMChannel channel;
                    channel.m_url = TTK::Algorithm::mdII(QT_SONG_URL, false).arg( value["id"].toInt());
                    channel.m_name = value["title"].toString();
                    channels->append(channel);
                }
            }
        }
    } while(pageValid());
}


ToolXIFM::ToolXIFM(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void ToolXIFM::startToRequest()
{
    TTK_INFO_STREAM("Start to request xiph fm data");

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(XI_SONG_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request, "charset=utf-8");

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void ToolXIFM::downLoadFinished()
{
    MusicFMCategoryList items;
    MusicAbstractNetwork::downLoadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        TTKAbstractXml xml;
        if(xml.fromByteArray(m_reply->readAll()))
        {
            MusicFMCategory category;
            category.m_category = "foreign";

            const QDomNodeList &nodes = xml.findDomNodes("entry");
            for(int i = 0; i < nodes.count(); ++i)
            {
                const QDomNodeList &chnNodes = nodes.item(i).childNodes();

                MusicFMChannel item;
                for(int j = 0; j < chnNodes.count(); ++j)
                {
                    const QDomElement &element = chnNodes.item(j).toElement();
                    if(element.nodeName() == "server_name")
                    {
                        item.m_name = element.text();
                    }
                    else if(element.nodeName() == "listen_url")
                    {
                        item.m_url = element.text();
                    }
                }

                if(!item.m_name.isEmpty() && !item.m_url.isEmpty())
                {
                    category.m_items << item;
                }
            }

            items.append(category);
        }
    }

    MusicFMConfigManager manager;
    if(manager.load("xiph.xml"))
    {
        manager.writeBuffer(items);
        TTK_INFO_STREAM("Write xiph fm data done");
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();

    qApp->exit();
}
