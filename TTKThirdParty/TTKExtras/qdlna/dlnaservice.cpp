#include "dlnaservice.h"

DlnaService::DlnaService()
{

}

DlnaService::DlnaService(const QString &control, const QString &scpd, const QString &eventSub, const QString &type, const QString &id)
{
    m_controlURL = control;
    m_scpdURL = scpd;
    m_eventSubURL = eventSub;
    m_serviceType = type;
    m_serviceID = id;
}

QString DlnaService::getServiceID() const
{
    return m_serviceID;
}

QString DlnaService::getControlURL() const
{
    return m_controlURL;
}

bool DlnaService::isEmpty() const
{
    return m_controlURL.isEmpty() && m_scpdURL.isEmpty() && m_eventSubURL.isEmpty() && m_serviceType.isEmpty() && m_serviceID.isEmpty();
}
