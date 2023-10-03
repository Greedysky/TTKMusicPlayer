#include "qdlnaxml.h"

#include <QtXml/QDomDocument>

/*! @brief The class of the dlna xml private.
 * @author Greedysky <greedysky@163.com>
 */
class QDlnaXmlPrivate : public TTKPrivate<QDlnaXml>
{
public:
    QDlnaXmlPrivate();
    ~QDlnaXmlPrivate();

    QDomDocument *m_document;

};

QDlnaXmlPrivate::QDlnaXmlPrivate()
    : m_document(new QDomDocument)
{

}

QDlnaXmlPrivate::~QDlnaXmlPrivate()
{
    delete m_document;
}



QDlnaXml::QDlnaXml()
{
    TTK_INIT_PRIVATE(QDlnaXml);
}

bool QDlnaXml::fromString(const QString &data)
{
    TTK_D(QDlnaXml);
    if(!d->m_document->setContent(data))
    {
        return false;
    }
    return true;
}

QString QDlnaXml::toString() const
{
    TTK_D(QDlnaXml);
    if(!d->m_document)
    {
        return {};
    }
    return d->m_document->toString();
}

QString QDlnaXml::tagNameToLower(const QString &data) const
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

QString QDlnaXml::readTagNameValue(const QString &tagName) const
{
    TTK_D(QDlnaXml);
    const QDomNodeList &nodes = d->m_document->elementsByTagName(tagName);
    return nodes.isEmpty() ? QString() : nodes.item(0).toElement().text();
}

QDlnaService QDlnaXml::readServiceTag(const QString &type, const QString &tagName) const
{
    TTK_D(QDlnaXml);
    const QDomNodeList &nodes = d->m_document->elementsByTagName(tagName);
    if(nodes.isEmpty())
    {
        return QDlnaService();
    }

    QDlnaService service;
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomNodeList &paramNodes = nodes.item(i).childNodes();

        for(int j = 0; j < paramNodes.count(); ++j)
        {
            const QDomNode &paramNode = paramNodes.item(j);
            QString text = paramNode.toElement().text();

            if(text.contains(type, Qt::CaseInsensitive))
            {
                const QString &nodeName = paramNode.nodeName();
                if(text.startsWith(TTK_SEPARATOR))
                {
                    text.remove(0, 1);
                }

                if(nodeName.contains("servicetype", Qt::CaseInsensitive))
                {
                    service.m_serviceType = text;
                }
                else if(nodeName.contains("serviceid", Qt::CaseInsensitive))
                {
                    service.m_serviceID = text;
                }
                else if(nodeName.contains("scpdurl", Qt::CaseInsensitive))
                {
                    service.m_scpdURL = text;
                }
                else if(nodeName.contains("controlurl", Qt::CaseInsensitive))
                {
                    service.m_controlURL = text;
                }
                else if(nodeName.contains("eventsuburl", Qt::CaseInsensitive))
                {
                    service.m_eventSubURL = text;
                }
            }
        }
    }
    return service;
}
