#include "ossdatainterface.h"
#include "ossdatainterface_p.h"

OSSDataInterface::OSSDataInterface(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(OSSDataInterface);
    d->m_networkManager = networkManager;
}

void OSSDataInterface::handleError(QNetworkReply::NetworkError error)
{
    M_LOGGER_ERROR("error" << error);
}
