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

void DlnaService::setControlURL(const QString &control)
{
    m_controlURL = control;
}

void DlnaService::setScpdlURL(const QString &scpd)
{
    m_scpdURL = scpd;
}

void DlnaService::setEventSublURL(const QString &eventSub)
{
    m_eventSubURL = eventSub;
}

void DlnaService::setServiceType(const QString &type)
{
    m_serviceType = type;
}

void DlnaService::setServiceID(const QString &id)
{
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
