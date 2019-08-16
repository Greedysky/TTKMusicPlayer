#include "dlnaclient.h"

#include <unistd.h>
#include <QStringList>

const QString XMLHead = "<?xml version=\"1.0\"?>\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n<SOAP-ENV:Body>\n";
const QString XMLFoot = "</SOAP-ENV:Body>\n</SOAP-ENV:Envelope>\n";

DLNAClient::DLNAClient(const QString &data)
{
    m_isConnected = false;
    m_xml = new DLNAXml;

    const QStringList& list = data.split("\r\n");
    foreach(const QString &str, list)
    {
        if(str.contains("LOCATION:"))
        {
            QString data = str;
            const QStringList& data_list = data.remove("LOCATION: http://").split(":");
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
}

DLNAClient::~DLNAClient()
{
    delete m_xml;
}

QString DLNAClient::server()
{
    return m_serverIP;
}

QString DLNAClient::serverName()
{
    return m_friendlyName;
}

bool DLNAClient::connect()
{
    const QString &request = HelperDLNA::MakeRequest("GET", m_smp, 0, QString(), m_serverIP, m_serverPort);
    const QString &response = HelperDLNA::makeSocketGetReply(m_serverIP, m_serverPort, request);
    const int code = HelperDLNA::GetResponseCode(response);
    if(code != 200)
    {
        return false;
    }

    const QString &body = HelperDLNA::removeHttpHeader(response);
    if(!m_xml->fromString(m_xml->tagNameToLower(body)))
    {
        return false;
    }

    m_friendlyName = m_xml->readTagNameValue("friendlyname");
    if(m_friendlyName.isEmpty())
    {
        return false;
    }

    m_services.insert("avtransport", m_xml->readServiceTag("avtransport", "server"));
    m_services.insert("avtransport", m_xml->readServiceTag("avtransport", "server"));
    m_services.insert("avtransport", m_xml->readServiceTag("avtransport", "server"));
    m_services.insert("avtransport", m_xml->readServiceTag("avtransport", "server"));

    for(auto it = m_services.begin(); it != m_services.end(); ++it)
    {
        if(it.key().contains("avtransport"))
        {
            m_controlURL = it.value().getControlURL();
            m_isConnected = true;
            return true;
        }
    }
    return false;
}

bool DLNAClient::isConnected() const
{
    return m_isConnected;
}

QString DLNAClient::tryToPlayFile(const QString &url)
{
    const QString &uploadState = uploadFileToPlay(url);
    const QString &playState = startPlay(0);
    return playState;
}

QString DLNAClient::uploadFileToPlay(const QString &url)
{
    //Later we will send a message to the DLNA server to start the file playing
    QString play_url = url;
    QString body = XMLHead;
    body += "<u:SetAVTransportURI xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\n";
    body += "<InstanceID>0</InstanceID>\n";
    body += "<CurrentURI>" + play_url.replace(" ", "%20") + "</CurrentURI>\n";
    body += "</u:SetAVTransportURI>\n";
    body += XMLFoot + "\n";
    const QString &request = HelperDLNA::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#SetAVTransportURI", m_serverIP, m_serverPort) + body;
    return HelperDLNA::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DLNAClient::startPlay(int instance)
{
    //Start playing the new upload film or music track
    QString body = XMLHead;
    body += "<u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>"+ QString::number(instance) + "</InstanceID><Speed>1</Speed></u:Play>\n";
    body += XMLFoot + "\n";
    const QString &request = HelperDLNA::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Play", m_serverIP, m_serverPort) + body;
    return HelperDLNA::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DLNAClient::stopPlay(int instance)
{
    //Called to stop playing a movie or a music track
    QString body = XMLHead;
    body += "<u:Stop xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Stop>\n";
    body += XMLFoot + "\n";
    const QString &request = HelperDLNA::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Stop", m_serverIP, m_serverPort) + body;
    return HelperDLNA::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DLNAClient::pause(int instance)
{
    //Called to pause playing a movie or a music track
    QString body = XMLHead;
    body += "<u:Pause xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID>" + QString::number(instance) + "</InstanceID></u:Pause>\n";
    body += XMLFoot + "\n";
    const QString &request = HelperDLNA::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#Pause", m_serverIP, m_serverPort) + body;
    return HelperDLNA::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

QString DLNAClient::getPosition()
{
    //Returns the current position for the track that is playing on the DLNA server
    const QString &body = XMLHead + "<m:GetPositionInfo xmlns:m=\"urn:schemas-upnp-org:service:AVTransport:1\"><InstanceID xmlns:dt=\"urn:schemas-microsoft-com:datatypes\" dt:dt=\"ui4\">0</InstanceID></m:GetPositionInfo>" + XMLFoot + "\n";
    const QString &request = HelperDLNA::MakeRequest("POST", m_controlURL, body.length(), "urn:schemas-upnp-org:service:AVTransport:1#GetPositionInfo", m_serverIP, m_serverPort) + body;
    return HelperDLNA::makeSocketGetReply(m_serverIP, m_serverPort, request);
}

int DLNAClient::totalSeconds(const QString &value)
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
