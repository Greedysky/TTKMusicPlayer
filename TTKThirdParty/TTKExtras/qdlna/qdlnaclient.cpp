#include "qdlnaclient.h"
#include "qdlnaxml.h"

#include <QMap>
#include <unistd.h>
#include <QFileInfo>
#include <QStringList>

static constexpr const char *AVTRANSPORT = "avtransport";

static const QString XML_HEAD = "<?xml version=\"1.0\"?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n<SOAP-ENV:Body>\n";
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
    QDlnaClientPrivate();

    void initialize(const QString &data);
    bool connect();

    bool m_isConnected;

    QString m_serverIP, m_serverPort;
    QString m_smp, m_controlURL;
    QString m_friendlyName;
    QMap<QString, QDlnaService> m_services;

};

QDlnaClientPrivate::QDlnaClientPrivate()
    : m_isConnected(false)
{

}

void QDlnaClientPrivate::initialize(const QString &data)
{
    const QStringList &list = data.split(TTK_WLINEFEED);
    for(QString str : qAsConst(list))
    {
        QStringList data_list;
        if(str.contains("LOCATION:"))
        {
            data_list = str.remove("LOCATION: http://").split(":");
        }
        else if(str.contains("Location:"))
        {
            data_list = str.remove("Location: http://").split(":");
        }
        else if(str.contains("location:"))
        {
            data_list = str.remove("location: http://").split(":");
        }

        if(data_list.count() >= 2)
        {
            m_serverIP = data_list[0];
            const QStringList &info_list = data_list[1].split(TTK_SEPARATOR);

            if(info_list.count() >= 2)
            {
                m_serverPort = info_list[0];
                m_smp = info_list[1];
            }
        }
    }
}

bool QDlnaClientPrivate::connect()
{
    const QString &request = QDlna::makeRequest("GET", m_smp, 0, {}, m_serverIP, m_serverPort);
    const QString &response = QDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
    TTK_INFO_STREAM(m_serverIP << m_serverPort << m_smp << response);
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
    TTK_D(QDlnaClient);
    return d->m_serverIP;
}

QString QDlnaClient::serverName() const
{
    TTK_D(QDlnaClient);
    return d->m_friendlyName;
}

bool QDlnaClient::connect() const
{
    TTK_D(QDlnaClient);
    return d->connect();
}

bool QDlnaClient::isConnected() const
{
    TTK_D(QDlnaClient);
    return d->m_isConnected;
}

bool QDlnaClient::open(const QString &url) const
{
    TTK_D(QDlnaClient);
    //Later we will send a message to the DLNA server to start the file playing
    QFileInfo fin(url);
    QString play_url = url;
    QString body = XML_HEAD;
    play_url.replace(TTK_SPACE, "%20");

    body += "<u:SetAVTransportURI xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\n";
    body += "<InstanceID>0</InstanceID>\n";
    body += "<CurrentURI>" + play_url + "</CurrentURI>\n";
    body += "<CurrentURIMetaData>" + META_DATA.arg(fin.baseName(), fin.owner(), "audio", "audio/mp3", play_url) + "</CurrentURIMetaData>\n";
    body += "</u:SetAVTransportURI>\n";
    body += XML_FOOT + "\n";
    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#SetAVTransportURI", d->m_serverIP,d->m_serverPort) + body;
    TTK_INFO_STREAM(request);
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request));
}

bool QDlnaClient::play(int instance) const
{
    TTK_D(QDlnaClient);
    //Start playing the new upload film or music track
    QString body = XML_HEAD;
    body += "<u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>"+ QString::number(instance) + "</InstanceID><Speed>1</Speed></u:Play>\n";
    body += XML_FOOT + "\n";
    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Play", d->m_serverIP, d->m_serverPort) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request));
}

bool QDlnaClient::pause(int instance) const
{
    TTK_D(QDlnaClient);
    //Called to pause playing a movie or a music track
    QString body = XML_HEAD;
    body += "<u:Pause xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Pause>\n";
    body += XML_FOOT + "\n";
    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Pause", d->m_serverIP, d->m_serverPort) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request));
}

bool QDlnaClient::stop(int instance) const
{
    TTK_D(QDlnaClient);
    //Called to stop playing a movie or a music track
    QString body = XML_HEAD;
    body += "<u:Stop xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Stop>\n";
    body += XML_FOOT + "\n";
    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Stop", d->m_serverIP, d->m_serverPort) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request));
}

bool QDlnaClient::remove(int instance) const
{
    TTK_D(QDlnaClient);
    //Called to remove queue track
    QString body = XML_HEAD;
    body += "<u:RemoveAllTracksFromQueue xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:RemoveAllTracksFromQueue>\n";
    body += XML_FOOT + "\n";
    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#RemoveAllTracksFromQueue", d->m_serverIP, d->m_serverPort) + body;
    return QDlna::isValid(QDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request));
}

static qint64 valueToSecond(const QString &value)
{
    return QDateTime::fromString(value, TTK_TIMES_FORMAT).toMSecsSinceEpoch() / 1000;
}

bool QDlnaClient::position(qint64 &position, qint64 &duration, int instance) const
{
    TTK_D(QDlnaClient);
    //Returns the current position for the track that is playing on the DLNA server
    QString body = XML_HEAD;
    body += "<u:GetPositionInfo xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:GetPositionInfo>\n";
    body += XML_FOOT + "\n";
    const QString &request = QDlna::makeRequest("POST", d->m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#GetPositionInfo", d->m_serverIP, d->m_serverPort) + body;
    const QString &response =  QDlna::makeSocketGetReply(d->m_serverIP, d->m_serverPort, request);
    if(!QDlna::isValid(response))
    {
        return false;
    }

    QDlnaXml xml;
    if(!xml.fromString(QDlna::removeHttpHeader(response)))
    {
        return false;
    }

    position = valueToSecond(xml.readTagNameValue("Relime"));
    duration = valueToSecond(xml.readTagNameValue("TrackDuration"));
    return true;
}
