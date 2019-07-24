#include "dlnaservice.h"

DLNAService::DLNAService()
{

}

DLNAService::DLNAService(const QString &control, const QString &scpd, const QString &eventSub, const QString &type, const QString &id)
{
    m_controlURL = control;
    m_scpdurl = scpd;
    m_eventSubURL = eventSub;
    m_serviceType = type;
    m_serviceID = id;
}

QString DLNAService::getServiceID() const
{
    return m_serviceID;
}

QString DLNAService::getControlURL() const
{
    return m_controlURL;
}
