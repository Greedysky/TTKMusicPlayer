#include "qdlnahelper.h"

#include <QSysInfo>
#include <QTcpSocket>
#include <QStringList>

namespace QDlnaHelper
{
QString MakeRequest(const QString &methord, const QString &url, int length, const QString &soapAction, const QString &ip, const QString &port)
{
    //Make a request that is sent out to the DLNA server on the LAN using TCP
    QString request = methord.toUpper() + " /" + url + " HTTP/1.1" + "\n";
    request += "Cache-Control: no-cache\n";
    request += "Connection: Close\n";
    request += "Pragma: no-cache\n";
    request += "Host: " + ip + ":" + port + "\n";
    request += "User-Agent: Microsoft-Windows/6.3 UPnP/1.0 Microsoft-DLNA DLNADOC/1.50\n";
    request += "FriendlyName.DLNA.ORG: ";
#if TTK_QT_VERSION_CHECK(5,6,0)
    request += QSysInfo::machineHostName();
#else
    request += "Greedysky";
#endif
    request += "\n";

    if(length > 0)
    {
        request += "Content-Length: " + QString::number(length) + "\n";
        request += "Content-Type: text/xml; charset=\"utf-8\"\n";
    }

    if(soapAction.length() > 0)
    {
        request += "SOAPAction: \"" + soapAction + "\"\n";
    }
    request += "\n";
    return request;
}

QString makeSocketGetReply(const QString &ip, const QString &port, const QString &data)
{
    QTcpSocket sock;
    sock.connectToHost(ip, port.toUShort());
    constexpr int timeout = 5000 / 2;

    if(sock.waitForConnected(timeout))
    {
        sock.write(data.toLatin1());
    }
    else
    {
        return "Couldn't connect";
    }

    if(!sock.waitForBytesWritten(timeout))
    {
        return "Couldn't write";
    }

    if(sock.waitForReadyRead(timeout))
    {
        return QString::fromUtf8(sock.readAll());
    }
    else
    {
        return "Couldn't get an answer";
    }
}

QString removeHttpHeader(const QString &data)
{
    const QStringList &data_list = data.split("\r\n\r\n");
    return (data_list.count() >= 2) ? data_list[1] : QString();
}

int GetResponseCode(const QString &data)
{
    const QStringList &data_list = data.split(TTK_SPACE);
    if(data_list.count() >= 2)
    {
        bool ok = false;
        const int code = data_list[1].toInt(&ok);
        if(ok)
        {
            return code;
        }
    }
    return -1;
}
}
