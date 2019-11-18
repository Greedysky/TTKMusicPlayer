#include "ossdatainterface.h"

OSSDataInterface::OSSDataInterface(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    m_networkManager = networkManager;
}

void OSSDataInterface::handleError(QNetworkReply::NetworkError error)
{
    M_LOGGER_ERROR("error" << error);
}

void OSSDataInterface::insertAuthorization(const QString &method, MStringMap &headers, const QString &resource)
{
    if(!OSSConf::ACCESS_KEY.isEmpty() && !OSSConf::SECRET_KEY.isEmpty())
    {
        headers.insert("Authorization", OSSUtils::createSignForNormalAuth(method, OSSConf::ACCESS_KEY, OSSConf::SECRET_KEY, headers, resource));
    }
    else if(!OSSConf::ACCESS_KEY.isEmpty())
    {
        headers.insert("Authorization", OSSConf::ACCESS_KEY);
    }
}
