#include "dlnaxml.h"
#include <QDebug>

DlnaXml::DlnaXml()
{
    m_document = new QDomDocument;
}

DlnaXml::~DlnaXml()
{
    delete m_document;
}

bool DlnaXml::fromString(const QString &data)
{
    if(!m_document->setContent(data))
    {
        return false;
    }
    return true;
}

QString DlnaXml::toString() const
{
    if(!m_document)
    {
        return QString();
    }
    return m_document->toString();
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
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return QString();
    }
    return nodeList.at(0).toElement().text();
}

DlnaService DlnaXml::readServiceTag(const QString &type, const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
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
            const QString &text = paramNode.toElement().text();
            qDebug() << text << text.contains(type, Qt::CaseInsensitive);
        }
    }
    return service;
}
