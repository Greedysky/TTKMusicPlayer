#include "qdlnaclient.h"
#include "qdlnaxml.h"

#include <QMap>
#include <QUrl>
#include <unistd.h>
#include <QFileInfo>
#include <QStringList>

static constexpr const char *AVTRANSPORT = "avtransport";
static const QString AVTSCHEMA = "urn:schemas-upnp-org:service:AVTransport:1";

static const QString XML_HEAD = "<?xml version='1.0' encoding='UTF-8'?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n<SOAP-ENV:Body>\n";
static const QString XML_FOOT = "</SOAP-ENV:Body>\n</SOAP-ENV:Envelope>\n";
static const QString META_DATA = "&lt;DIDL-Lite xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\""
                                 " xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\""
                                 " xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
                                 " xmlns:sec=\"http://www.sec.co.kr/\"&gt;&lt;item id=\"f-0\" parentID=\"0\" restricted=\"0\"&gt;"
                                 "&lt;dc:title&gt;%1&lt;/dc:title&gt;"
                                 "&lt;dc:creator&gt;%2&lt;/dc:creator&gt;"
                                 "&lt;upnp:class&gt;object.item.%3Item&lt;/upnp:class&gt;"
                                 "&lt;res protocolInfo=\"http-get:*:%4:DLNA.ORG_OP=01;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000\" &gt;"
                                 "%5&lt;/res&gt;&lt;/item&gt;&lt;/DIDL-Lite&gt;";
static const QStringList AVT_NAMES = {"avtransport", "AVTransport"};
static const QStringList FRIENS_NAMES = {"friendlyname", "friendlyName", "FriendlyName", "FriendlyName"};

/*! @brief The class of the dlna client private.
 * @author Greedysky <greedysky@163.com>
 */
class QDlnaClientPrivate : public TTKPrivate<QDlnaClient>
{
public:
    QDlnaClientPrivate() noexcept;

    void initialize(const QString &data);
    bool connect();

    bool m_isConnected;

    QString m_address, m_port;
    QString m_smp, m_controlURL;
    QString m_friendlyName;
    QMap<QString, QDlnaService> m_services;

};

QDlnaClientPrivate::QDlnaClientPrivate() noexcept
    : m_isConnected(false)
{

}

void QDlnaClientPrivate::initialize(const QString &data)
{
    const QStringList &datas = data.split(TTK_WLINEFEED);
    for(QString str : qAsConst(datas))
    {
        QStringList locations;
        if(str.contains("LOCATION:"))
        {
            locations = str.remove("LOCATION: http://").split(":");
        }
        else if(str.contains("Location:"))
        {
            locations = str.remove("Location: http://").split(":");
        }
        else if(str.contains("location:"))
        {
            locations = str.remove("location: http://").split(":");
        }

        if(locations.count() >= 2)
        {
            m_address = locations[0];
            const QStringList &infos = locations[1].split(TTK_SEPARATOR);

            if(infos.count() >= 2)
            {
                m_port = infos[0];
                m_smp = infos[1];
            }
        }
    }
}

bool QDlnaClientPrivate::connect()
{
    const QString &request = QDlna::makeRequest("GET", m_smp, 0, {}, m_address, m_port);
    const QString &response = QDlna::makeSocketGetReply(m_address, m_port, request);
    TTK_INFO_STREAM(m_address << m_port << m_smp << response);

    if(!QDlna::isValid(response))
    {
        return false;
    }

    QDlnaXml xml;
    if(!xml.fromString(xml.tagNameToLower(QDlna::removeHttpHeader(response))))
    {
        return false;
    }
    //
    for(const QString &name : qAsConst(FRIENS_NAMES))
    {
        m_friendlyName = xml.readTagNameValue(name);
        if(!m_friendlyName.isEmpty())
        {
            break;
        }
    }
    //
    if(m_friendlyName.isEmpty())
    {
        return false;
    }
    //
    for(const QString &name : qAsConst(AVT_NAMES))
    {
        const QDlnaService &server = xml.readServiceTag(name, "server");
        if(!server.isEmpty())
        {
            m_services.insert(AVTRANSPORT, server);
            break;
        }
    }
    //
    for(const QString &name : qAsConst(AVT_NAMES))
    {
        const QDlnaService &server = xml.readServiceTag(name, "service");
        if(!server.isEmpty())
        {
            m_services.insert(AVTRANSPORT, server);
            break;
        }
    }
    //
    for(auto it = m_services.begin(); it != m_services.end(); ++it)
    {
        if(it.key().contains(AVTRANSPORT))
        {
            m_controlURL = it.value().m_controlURL;
            m_isConnected = true;
            return true;
        }
    }
    return false;
}



QDlnaClient::QDlnaClient(const QString &data)
{
    TTK_INIT_PRIVATE(QDlnaClient);
    TTK_D(QDlnaClient);
    d->initialize(data);
}

QString QDlnaClient::server() const
{
    TTK_D(const QDlnaClient);
    return d->m_address;
}

QString QDlnaClient::serverName() const
{
    TTK_D(const QDlnaClient);
    return d->m_friendlyName;
}

bool QDlnaClient::connect() const
{
    TTK_D(QDlnaClient);
    return d->connect();
}

bool QDlnaClient::isConnected() const
{
    TTK_D(const QDlnaClient);
    return d->m_isConnected;
}

bool QDlnaClient::openUri(const QString &url, const QString &name, int instance) const
{
    TTK_D(const QDlnaClient);
    //Later we will send a message to the DLNA server to start the file playing
    const QFileInfo fin(url + name);
    QString play_url = url + QUrl::toPercentEncoding(name);
    QString body = XML_HEAD;

    body += "<u:SetAVTransportURI xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "<CurrentURI>" + play_url + "</CurrentURI>\n";
    body += "<CurrentURIMetaData>" + META_DATA.arg(fin.baseName(), fin.owner(), "audio", "audio/mp3", play_url) + "</CurrentURIMetaData>\n";
    body += "</u:SetAVTransportURI>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#SetAVTransportURI", d->m_address,d->m_port) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

bool QDlnaClient::openUris(const QString &url, const QString &name, int instance) const
{
    TTK_D(const QDlnaClient);
    //Later we will send a message to the DLNA server to start the file playing
    const QFileInfo fin(url + name);
    QString play_url = url + QUrl::toPercentEncoding(name);
    QString body = XML_HEAD;

    body += "<u:AddURIToQueue xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "<EnqueuedURI>" + play_url + "</EnqueuedURI>\n";
    body += "<EnqueuedURIMetaData>" + META_DATA.arg(fin.baseName(), fin.owner(), "audio", "audio/mp3", play_url) + "</EnqueuedURIMetaData>\n";
    body += "<DesiredFirstTrackNumberEnqueued>0</DesiredFirstTrackNumberEnqueued>\n";
    body += "<EnqueueAsNext>1</EnqueueAsNext>\n";
    body += "</u:AddURIToQueue>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#AddURIToQueue", d->m_address,d->m_port) + body;
      TTK_INFO_STREAM(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

bool QDlnaClient::play(int instance) const
{
    TTK_D(const QDlnaClient);
    //Start playing the new upload track
    QString body = XML_HEAD;
    body += "<u:Play xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "<Speed>1</Speed>\n";
    body += "</u:Play>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#Play", d->m_address, d->m_port) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

bool QDlnaClient::pause(int instance) const
{
    TTK_D(const QDlnaClient);
    //Called to pause playing track
    QString body = XML_HEAD;
    body += "<u:Pause xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:Pause>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#Pause", d->m_address, d->m_port) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

bool QDlnaClient::stop(int instance) const
{
    TTK_D(const QDlnaClient);
    //Called to stop playing track
    QString body = XML_HEAD;
    body += "<u:Stop xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:Stop>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#Stop", d->m_address, d->m_port) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

bool QDlnaClient::remove(int instance) const
{
    TTK_D(const QDlnaClient);
    //Called to remove queue track
    QString body = XML_HEAD;
    body += "<u:RemoveAllTracksFromQueue xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:RemoveAllTracksFromQueue>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#RemoveAllTracksFromQueue", d->m_address, d->m_port) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

bool QDlnaClient::previous(int instance) const
{
    TTK_D(const QDlnaClient);
    //Called to previous queue track
    QString body = XML_HEAD;
    body += "<u:Previous xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:Previous>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#Previous", d->m_address, d->m_port) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

bool QDlnaClient::next(int instance) const
{
    TTK_D(const QDlnaClient);
    //Called to next queue track
    QString body = XML_HEAD;
    body += "<u:Next xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:Next>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#Next", d->m_address, d->m_port) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_address, d->m_port, request));
}

static qint64 valueToSecond(const QString &value)
{
    return QDateTime::fromString(value, TTK_TIMES_FORMAT).toMSecsSinceEpoch() / 1000;
}

bool QDlnaClient::positionInfo(QDlna::PositionInfo &info, int instance) const
{
    TTK_D(const QDlnaClient);
    //Returns the current position info
    QString body = XML_HEAD;
    body += "<u:GetPositionInfo xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:GetPositionInfo>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#GetPositionInfo", d->m_address, d->m_port) + body;
    const QString &response =  QDlna::makeSocketGetReply(d->m_address, d->m_port, request);
    if(!QDlna::isValid(response))
    {
        return false;
    }

    QDlnaXml xml;
    if(!xml.fromString(QDlna::removeHttpHeader(response)))
    {
        return false;
    }

    info.position = valueToSecond(xml.readTagNameValue("Relime"));
    info.duration = valueToSecond(xml.readTagNameValue("TrackDuration"));
    return true;
}

bool QDlnaClient::transportInfo(QDlna::TransportInfo &info, int instance) const
{
    TTK_D(const QDlnaClient);
    //Returns the current transport info
    QString body = XML_HEAD;
    body += "<u:GetTransportInfo xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:GetTransportInfo>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#GetTransportInfo", d->m_address, d->m_port) + body;
    const QString &response =  QDlna::makeSocketGetReply(d->m_address, d->m_port, request);
    if(!QDlna::isValid(response))
    {
        return false;
    }

    QDlnaXml xml;
    if(!xml.fromString(QDlna::removeHttpHeader(response)))
    {
        return false;
    }

    info.state = valueToSecond(xml.readTagNameValue("CurrentTransportState"));
    info.status = valueToSecond(xml.readTagNameValue("CurrentTransportStatus"));
    info.speed = valueToSecond(xml.readTagNameValue("CurrentSpeed"));
    return true;
}

bool QDlnaClient::mediaInfo(int instance) const
{
    TTK_D(const QDlnaClient);
    //Returns the current transport info
    QString body = XML_HEAD;
    body += "<u:GetMediaInfo xmlns:u=\""+ AVTSCHEMA + "\">\n";
    body += "<InstanceID>" + QString::number(instance) + "</InstanceID>\n";
    body += "</u:GetMediaInfo>\n";
    body += XML_FOOT + "\n";

    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), AVTSCHEMA + "#GetMediaInfo", d->m_address, d->m_port) + body;
    const QString &response =  QDlna::makeSocketGetReply(d->m_address, d->m_port, request);
    if(!QDlna::isValid(response))
    {
        return false;
    }

    TTK_INFO_STREAM(response);

    QDlnaXml xml;
    if(!xml.fromString(QDlna::removeHttpHeader(response)))
    {
        return false;
    }
    return true;
}
