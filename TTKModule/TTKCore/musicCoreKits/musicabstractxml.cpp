#include "musicabstractxml.h"

#ifdef Q_CC_GNU
#   pragma GCC diagnostic ignored "-Wswitch"
#endif

MusicAbstractXml::MusicAbstractXml(QObject *parent)
    : QObject(parent)
{
    m_file = nullptr;
    m_document = nullptr;
}

MusicAbstractXml::~MusicAbstractXml()
{
    delete m_file;
    delete m_document;
}

bool MusicAbstractXml::readConfig(const QString &name)
{
    delete m_file;
    delete m_document;
    m_file = new QFile( name );
    m_document = new QDomDocument;

    if(!m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    if(!m_document->setContent(m_file))
    {
        m_file->close();
        delete m_file;
        m_file = nullptr;
        return false;
    }
    return true;
}

bool MusicAbstractXml::writeConfig(const QString &name)
{
    delete m_file;
    delete m_document;
    m_file = new QFile( name );
    m_document = new QDomDocument;

    if(!m_file->open(QFile::WriteOnly | QFile::Text))
    {
        return false;
    }
    return true;
}

bool MusicAbstractXml::fromString(const QString &data)
{
    delete m_file;
    delete m_document;
    m_file = nullptr;
    m_document = new QDomDocument;

    if(!m_document->setContent(data))
    {
        return false;
    }
    return true;
}

bool MusicAbstractXml::fromByteArray(const QByteArray &data)
{
    delete m_file;
    delete m_document;
    m_file = nullptr;
    m_document = new QDomDocument;

    if(!m_document->setContent(data))
    {
        return false;
    }
    return true;
}

QString MusicAbstractXml::toString() const
{
    if(!m_document)
    {
        return QString();
    }
    return m_document->toString();
}

QByteArray MusicAbstractXml::toByteArray() const
{
    if(!m_document)
    {
        return QByteArray();
    }
    return m_document->toByteArray();
}

QString MusicAbstractXml::readXmlAttributeByTagNameValue(const QString &tagName) const
{
    return readXmlAttributeByTagName(tagName, "value");
}

QString MusicAbstractXml::readXmlAttributeByTagName(const QString &tagName, const QString &attrName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return QString();
    }
    return nodeList.at(0).toElement().attribute(attrName);
}

QString MusicAbstractXml::readXmlTextByTagName(const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return QString();
    }
    return nodeList.at(0).toElement().text();
}

MVariantMap MusicAbstractXml::readXmlAttributesByTagName(const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return MVariantMap();
    }

    const QDomNamedNodeMap &nodes = nodeList.at(0).toElement().attributes();
    MVariantMap maps;
    for(int i=0; i<nodes.count(); ++i)
    {
        const QDomAttr &attr = nodes.item(i).toAttr();
        maps[attr.name()] = attr.value();
    }
    return maps;
}

void MusicAbstractXml::createProcessingInstruction()
{
    const QDomNode &node = m_document->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    m_document->appendChild( node );
}

QDomElement MusicAbstractXml::createRoot(const QString &node)
{
    const QDomElement &domElement = m_document->createElement( node );
    m_document->appendChild( domElement );
    return domElement;
}

QDomElement MusicAbstractXml::createRoot(const QString &node, const MusicXmlAttribute &attr)
{
    QDomElement domElement = m_document->createElement( node );
    writeAttribute(domElement, attr);
    m_document->appendChild( domElement );
    return domElement;
}

QDomElement MusicAbstractXml::createRoot(const QString &node, const MusicXmlAttributes &attrs)
{
    QDomElement domElement = m_document->createElement( node );
    foreach(const MusicXmlAttribute &attr, attrs)
    {
        writeAttribute(domElement, attr);
    }
    m_document->appendChild( domElement );
    return domElement;
}

QDomElement MusicAbstractXml::writeDomNode(QDomElement &element, const QString &node)
{
    const QDomElement &domElement = m_document->createElement( node );
    element.appendChild( domElement );
    return domElement;
}

QDomElement MusicAbstractXml::writeDomElement(QDomElement &element, const QString &node, const MusicXmlAttribute &attr)
{
    QDomElement domElement = writeDomNode(element, node);
    writeAttribute(domElement, attr);
    return domElement;
}

QDomElement MusicAbstractXml::writeDomElementMutil(QDomElement &element, const QString &node, const MusicXmlAttributes &attrs)
{
    if(attrs.isEmpty())
    {
        QDomElement();
    }

    QDomElement domElement = writeDomElement(element, node, attrs.front());
    for(int i=1; i<attrs.count(); ++i)
    {
        writeAttribute(domElement, attrs[i]);
    }
    return domElement;
}

void MusicAbstractXml::writeAttribute(QDomElement &element, const MusicXmlAttribute &attr)
{
    switch(attr.m_value.type())
    {
        case QVariant::Int :
            element.setAttribute(attr.m_key, attr.m_value.toInt()); break;
        case QVariant::String :
            element.setAttribute(attr.m_key, attr.m_value.toString()); break;
        case QVariant::LongLong :
            element.setAttribute(attr.m_key, attr.m_value.toLongLong()); break;
        case QVariant::ULongLong :
            element.setAttribute(attr.m_key, attr.m_value.toULongLong()); break;
        case QVariant::Double :
            element.setAttribute(attr.m_key, attr.m_value.toDouble()); break;
        case QVariant::UInt :
            element.setAttribute(attr.m_key, attr.m_value.toUInt()); break;
        default: break;
    }
}

QDomElement MusicAbstractXml::writeDomElementText(QDomElement &element, const QString &node, const MusicXmlAttribute &attr, const QString &text)
{
    QDomElement domElement = writeDomElement(element, node, attr);
    const QDomText &domText = m_document->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}

QDomElement MusicAbstractXml::writeDomElementMutilText(QDomElement &element, const QString &node, const MusicXmlAttributes &attrs, const QString &text)
{
    if(attrs.isEmpty())
    {
        QDomElement();
    }

    QDomElement domElement = writeDomElementMutil(element, node, attrs);
    const QDomText &domText = m_document->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}

QDomElement MusicAbstractXml::writeDomText(QDomElement &element, const QString &node, const QString &text)
{
    QDomElement domElement = writeDomNode(element, node);
    const QDomText &domText = m_document->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}
