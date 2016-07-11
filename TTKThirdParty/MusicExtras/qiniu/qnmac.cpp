#include "qnmac.h"
#include "qnutils.h"

// Use access key and secret key from https://portal.qiniu.com
QNMac::QNMac(const QString &accesKey, const QByteArray &secretKey)
    : m_accessKey(accesKey), m_secretKey(secretKey)
{

}

QNMac::~QNMac()
{

}

// Sign data
// Mainly used to create the private bucket download token

// 1. Use hmac sha1 algorithm to sign data with secret key
// 2. Use url safe base64 encoding to encode the result data from step1
// 3. Join access key and the result string from step2 with ':'
QString QNMac::sign(const QByteArray &data) const
{
    QByteArray signedData = QNUtils::hmacSha1(data, m_secretKey);
    QString retStr = m_accessKey;
    retStr.append(":");
    retStr.append(QNUtils::urlSafeBase64Encode(signedData));
    return retStr;
}

// Encode data and sign it
// Mainly used to create the encoded and signed put policy token

// 1. First encode the data with url safe base64
// 2. Use hmac sha1 algorithm to sign the result data from step1 with secret key
// 3. Use url safe base64 encoding to encode the result data from step2
// 4. Join access key and the result string from step3 and the result string from
// step1 with ':'
QString QNMac::signWithData(const QByteArray &data) const
{
    QString encodedData = QNUtils::urlSafeBase64Encode(data);
    QByteArray signedData = QNUtils::hmacSha1(encodedData.toLocal8Bit(), m_secretKey);
    QString encodedSignedData = QNUtils::urlSafeBase64Encode(signedData);
    QString retStr = m_accessKey;
    retStr.append(":").append(encodedSignedData);
    retStr.append(":").append(encodedData);
    return retStr;
}

QString QNMac::signWithData2(const QByteArray &data) const
{
    QByteArray signedData = QNUtils::hmacSha1(data, m_secretKey);
    QString encodedSignedData = QNUtils::urlSafeBase64Encode(signedData);
    QString retStr = m_accessKey;
    retStr.append(":").append(encodedSignedData);
    return retStr;
}

// Sign http request and body
// Mainly used to create the resource management auth token

// 1. Create url to sign, format is <path>\n or <path>?<query>\n or <path>?<query>\nbody
// 2. Use hmac sha1 algorithm to sign the url in step1
// 3. Use url safe base64 encoding to encode the result data from step2
// 4. Join access key and the result string from step3 with ':'
QString QNMac::signRequest(const QUrl &requestUrl, const QByteArray &bodyData) const
{
    QString path = requestUrl.path();
    QString query = requestUrl.query();
    QByteArray dataToSign = path.toUtf8();
    // check query whether empty
    if(!query.isEmpty())
    {
        dataToSign.append("?").append(query);
    }
    dataToSign.append("\n");
    // check whether with body
    if(!bodyData.isEmpty())
    {
        dataToSign.append(bodyData);
    }
    QByteArray signedData = QNUtils::hmacSha1(dataToSign,m_secretKey);
    QString encodedSignedData = QNUtils::urlSafeBase64Encode(signedData);
    QString retStr = m_accessKey;
    retStr.append(":").append(encodedSignedData);
    return retStr;
}

QString QNMac::signRequest(const QNetworkRequest &request,const QByteArray &bodyData) const
{
     QUrl requestUrl = request.url();
     return signRequest(requestUrl, bodyData);
}
