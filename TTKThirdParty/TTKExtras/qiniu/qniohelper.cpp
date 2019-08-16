#include "qniohelper.h"
#include "qnputextra.h"
#include "qnutils.h"
#include "qnmac.h"

#include <QtNetwork/QHttpPart>

QHttpMultiPart* QNIOHelper::createPutDataMultiPart(const QString &uploadToken, const QByteArray &data, const QString &key, const QString &fileName, const QNPutExtra *putExtra)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    //token
    QHttpPart tokenPart;
    tokenPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"token\""));
    tokenPart.setBody(uploadToken.toUtf8());
    //key
    QHttpPart *keyPart = nullptr;
    if(!key.isEmpty())
    {
        keyPart = new QHttpPart;
        keyPart->setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"key\""));
        keyPart->setBody(key.toUtf8());
    }
    //file
    QHttpPart filePart;
    QString filePartHeader = QString("form-data; name=\"file\"");
    QString newFileName;
    if(fileName.isEmpty())
    {
       if(!key.isEmpty())
       {
           newFileName = key;
       }
       else
       {
           newFileName = QNUtils::randomFileName();
       }
    }
    else
    {
        newFileName = fileName;
    }
    filePartHeader.append("; filename=\"").append(newFileName).append("\"");
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(filePartHeader));
    filePart.setBody(data);
    //set mime type if specified
    if(putExtra != 0)
    {
        const QString &mimeType = putExtra->getMimeType();
        if(!mimeType.isEmpty())
        {
            filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType));
        }
    }
    //crc32

    //pack multi part
    multiPart->append(tokenPart);
    if(keyPart != 0)
    {
        multiPart->append(*keyPart);
    }
    multiPart->append(filePart);
    return multiPart;
}

//Get the meta info for the file
QNetworkRequest QNIOHelper::listRequest(const QString &bucket, const QNMac *mac)
{
    QNetworkRequest request;
    QString reqUrl(QNConf::RSF_HOST);
    reqUrl.append("/list?bucket=");

    //append encoded entry
    reqUrl.append(bucket);
    request.setUrl(reqUrl);

    //set authorization header
    const QString authHeader("Authorization");
    QString authHeaderBody("QBox ");
    QString accessToken;
    if(mac != 0)
    {
        accessToken = mac->signRequest(request);
    }
    else
    {
        const QNMac macx(QNConf::ACCESS_KEY,QNConf::SECRET_KEY);
        accessToken = macx.signRequest(request);
    }
    authHeaderBody.append(accessToken);
    request.setRawHeader(authHeader.toLocal8Bit(), authHeaderBody.toLocal8Bit());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    return request;
}

//Delete a file
QNetworkRequest QNIOHelper::deleteRequest(const QString &bucket, const QString &key, const QNMac *mac)
{
    QNetworkRequest request;
    QString reqUrl(QNConf::RS_HOST);
    reqUrl.append("/delete/");

    //append encoded entry
    QString entry;
    entry.append(bucket).append(":").append(key);
    const QString &encodedEntry = QNUtils::urlSafeBase64Encode(entry.toUtf8());
    reqUrl.append(encodedEntry);
    request.setUrl(reqUrl);

    //set authorization header
    const QString authHeader("Authorization");
    QString authHeaderBody("QBox ");
    QString accessToken;
    if(mac != 0)
    {
        accessToken = mac->signRequest(request);
    }
    else
    {
        const QNMac macx(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
        accessToken = macx.signRequest(request);
    }
    authHeaderBody.append(accessToken);
    request.setRawHeader(authHeader.toLocal8Bit(), authHeaderBody.toLocal8Bit());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    return request;
}
