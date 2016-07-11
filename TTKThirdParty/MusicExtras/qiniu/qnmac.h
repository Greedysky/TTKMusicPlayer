#ifndef QNMAC_H
#define QNMAC_H

#include <QNetworkRequest>
#include "musicextrasglobaldefine.h"

class MUSIC_EXTRAS_EXPORT QNMac
{
public:
    QNMac(const QString &accessKey, const QByteArray &secretKey);
    ~QNMac();

    QString sign(const QByteArray &data) const;
    QString signWithData(const QByteArray &data) const;
    QString signWithData2(const QByteArray &data) const;
    QString signRequest(const QUrl &reqestUrl, const QByteArray &bodyData = 0) const;
    QString signRequest(const QNetworkRequest &request, const QByteArray &bodyData = 0) const;

private:
    QString m_accessKey;
    QByteArray m_secretKey;

};

#endif // QNMAC_H
