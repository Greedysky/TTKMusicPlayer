#include "dlnaclient.h"

#include <unistd.h>
#include <QStringList>

#define AVTRANSPORT "avtransport"
#include <QDebug>
const QString XML_HEAD = "<?xml version=\"1.0\"?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n<SOAP-ENV:Body>\n";
const QString XML_FOOT = "</SOAP-ENV:Body>\n</SOAP-ENV:Envelope>\n";
const QStringList FRIENS_NAMES = QStringList() << "friendlyname" << "friendlyName" << "FriendlyName" << "FriendlyName";
const QStringList AVT_NAMES = QStringList() << "avtransport" << "AVTransport";

DlnaClient::DlnaClient(const QString &data)
{
    m_isConnected = false;
    m_xml = new DlnaXml;

    const QStringList& list = data.split("\r\n");
    foreach(QString str, list)
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

        if(data_list.size() >= 2)
        {
            m_serverIP = data_list[0];
            const QStringList& info_list = data_list[1].split("/");
            if(info_list.size() >= 2)
            {
                m_serverPort = info_list[0];
                m_smp = info_list[1];
            }
        }
    }
}

DlnaClient::~DlnaClient()
{
    delete m_xml;
}

QString DlnaClient::server()
{
    return m_serverIP;
}

QString DlnaClient::serverName()
{
    return m_friendlyName;
}

bool DlnaClient::connect()
{
    const QString &request = HelperDlna::MakeRequest("GET", m_smp, 0, QString(), m_serverIP, m_serverPort);
    const QString &response = HelperDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
    qDebug() << m_serverIP <<m_serverPort <<m_smp << response;
    const int code = HelperDlna::GetResponseCode(response);
    if(code != 200)
    {
        return false;
    }

    const QString &body = HelperDlna::removeHttpHeader(response);
    if(!m_xml->fromString(m_xml->tagNameToLower(body)))
    {
        return false;
    }
    //
    foreach(const QString &name, FRIENS_NAMES)
    {
        m_friendlyName = m_xml->readTagNameValue(name);
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
    foreach(const QString &name, AVT_NAMES)
    {
        DlnaService server = m_xml->readServiceTag(name, "server");
        if(!server.isEmpty())
        {
            m_services.insert(AVTRANSPORT, server);
            break;
        }
    }
    //
    foreach(const QString &name, AVT_NAMES)
    {
        DlnaService server = m_xml->readServiceTag(name, "service");
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
            m_controlURL = it.value().getControlURL();
            m_isConnected = true;
            return true;
        }
    }
    return false;
}

bool DlnaClient::isConnected() const
{
    return m_isConnected;
}

QString DlnaClient::tryToPlayFile(const QString &url)
{
    qDebug() << url << m_controlURL << m_isConnected;
    const QString &uploadState = uploadFileToPlay(url);
    const QString &playState = startPlay(0);
    qDebug() << uploadState << "playState " << playState;
    return playState;
}

QString DlnaClient::uploadFileToPlay(const QString &url)
{
    //Later we will send a message to the DLNA server to start the file playing
    QString play_url = url;
    QString body = XML_HEAD;
    body += "<u:SetAVTransportURI xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\n";
    body += "<InstanceID>0</InstanceID>\n";
    body += "<CurrentURI>" + play_url.replace(" ", "%20") + "</CurrentURI>\n";
    body += "</u:SetAVTransportURI>\n";
    body += XML_FOOT + "\n";
    const QString &request = HelperDlna::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#SetAVTransportURI", m_serverIP, m_serverPort) + body;
    qDebug() << request;
    return HelperDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DlnaClient::startPlay(int instance)
{
    //Start playing the new upload film or music track
    QString body = XML_HEAD;
    body += "<u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>"+ QString::number(instance) + "</InstanceID><Speed>1</Speed></u:Play>\n";
    body += XML_FOOT + "\n";
    const QString &request = HelperDlna::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Play", m_serverIP, m_serverPort) + body;
    return HelperDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DlnaClient::stopPlay(int instance)
{
    //Called to stop playing a movie or a music track
    QString body = XML_HEAD;
    body += "<u:Stop xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Stop>\n";
    body += XML_FOOT + "\n";
    const QString &request = HelperDlna::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Stop", m_serverIP, m_serverPort) + body;
    return HelperDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DlnaClient::pause(int instance)
{
    //Called to pause playing a movie or a music track
    QString body = XML_HEAD;
    body += "<u:Pause xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Pause>\n";
    body += XML_FOOT + "\n";
    const QString &request = HelperDlna::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Pause", m_serverIP, m_serverPort) + body;
    return HelperDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DlnaClient::getPosition()
{
    //Returns the current position for the track that is playing on the DLNA server
    const QString &body = XML_HEAD + "<m:GetPositionInfo xmlns:m=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui4\">0</InstanceID></m:GetPositionInfo>" + XML_FOOT + "\n";
    const QString &request = HelperDlna::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#GetPositionInfo", m_serverIP, m_serverPort) + body;
    return HelperDlna::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

int DlnaClient::totalSeconds(const QString &value)
{
    //Convert the time left for the track to play back to seconds
    const QStringList& data_list = value.split(".");
    if(data_list.size() >= 2)
    {
        const QStringList& info_list = data_list[1].split(":");
        if(info_list.size() >= 2)
        {
            const int mins = info_list[0].toInt();
            const int secs = info_list[1].toInt();
            return mins * 60 + secs;
        }
    }
    return 0;
}
