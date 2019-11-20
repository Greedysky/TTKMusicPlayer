#include "dlnaxml.h"

#include <QtXml/QDomDocument>

class DlnaXmlPrivate : public TTKPrivate<DlnaXml>
{
public:
    DlnaXmlPrivate();
    ~DlnaXmlPrivate();

    QDomDocument *m_document;

};

DlnaXmlPrivate::DlnaXmlPrivate()
{
    m_document = new QDomDocument;
}

DlnaXmlPrivate::~DlnaXmlPrivate()
{
    delete m_document;
}



DlnaXml::DlnaXml()
{
    TTK_INIT_PRIVATE;
}

bool DlnaXml::fromString(const QString &data)
{
    TTK_D(DlnaXml);
    if(!d->m_document->setContent(data))
    {
        return false;
    }
    return true;
}

QString DlnaXml::toString() const
{
    TTK_D(DlnaXml);
    if(!d->m_document)
    {
        return QString();
    }
    return d->m_document->toString();
}

QString DlnaXml::tagNameToLower(const QString &data) const
{
    QString body = data;
    int left = body.indexOf("<");
    while(left != -1)
    {
      const int right = body.indexOf(">", left + 1);
      if(right == -1)
      {
        break;
      }

      const QString &sub = body.mid(left + 1, right - left - 1);
      body.replace(left + 1, right - left - 1, sub.toLower());
      left = body.indexOf("<", left + 1);
    }
    return body;
}

QString DlnaXml::readTagNameValue(const QString &tagName) const
{
    TTK_D(DlnaXml);
    const QDomNodeList &nodeList = d->m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return QString();
    }
    return nodeList.at(0).toElement().text();
}

DlnaService DlnaXml::readServiceTag(const QString &type, const QString &tagName) const
{
    TTK_D(DlnaXml);
    const QDomNodeList &nodeList = d->m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return DlnaService();
    }

    DlnaService service;
    for(int i=0; i<nodeList.count(); ++i)
    {
        const QDomNode &node = nodeList.at(i);
        const QDomNodeList &paramNodes = node.childNodes();

        for(int j=0; j<paramNodes.count(); ++j)
        {
            const QDomNode &paramNode = paramNodes.at(j);
            QString text = paramNode.toElement().text();
            if(text.contains(type, Qt::CaseInsensitive))
            {
                const QString &nodeName = paramNode.nodeName();
                if(text.startsWith("/"))
                {
                    text.remove(0, 1);
                }
                if(nodeName.contains("servicetype", Qt::CaseInsensitive))
                    service.m_serviceType = text;
                else if(nodeName.contains("serviceid", Qt::CaseInsensitive))
                    service.m_serviceID = text;
                else if(nodeName.contains("scpdurl", Qt::CaseInsensitive))
                    service.m_scpdURL = text;
                else if(nodeName.contains("controlurl", Qt::CaseInsensitive))
                    service.m_controlURL = text;
                else if(nodeName.contains("eventsuburl", Qt::CaseInsensitive))
                    service.m_eventSubURL = text;
            }
        }
    }
    return service;
}
