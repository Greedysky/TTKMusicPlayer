#include "dlnaxml.h"
#include <QDebug>

DLNAXml::DLNAXml()
{
    m_document = nullptr;
}

bool DLNAXml::fromString(const QString &data)
{
    delete m_document;
    m_document = new QDomDocument;

    if(!m_document->setContent(data))
    {
        return false;
    }
    return true;
}

QString DLNAXml::toString() const
{
    if(!m_document)
    {
        return QString();
    }
    return m_document->toString();
}

QString DLNAXml::readTagNameValue(const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return QString();
    }
    return nodeList.at(0).toElement().text();
}

DLNAService DLNAXml::readServiceTag(const QString &type, const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return DLNAService();
    }

    for(int i=0; i<nodeList.count(); ++i)
    {
        const QDomNode &node = nodeList.at(i);
        const QDomNodeList &paramNodes = node.childNodes();

        for(int j=0; j<paramNodes.count(); ++j)
        {
            const QDomNode &paramNode = paramNodes.at(j);
            const QString &text = paramNode.toElement().text();
            qDebug() << text << text.contains(type, Qt::CaseInsensitive);
        }
    }
    return DLNAService();
}
