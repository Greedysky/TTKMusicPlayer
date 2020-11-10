#include "musicabstractxml.h"

#ifdef Q_CC_GNU
#   pragma GCC diagnostic ignored "-Wswitch"
#endif

MusicXmlNodeHelper::MusicXmlNodeHelper(const QDomNode& root)
{
    m_root    = root;
    m_current = root;
}

void MusicXmlNodeHelper::load()
{
    do
    {
        if(!m_current.isNull() && m_current.isElement())
        {
            m_nodeNames.insert(m_current.nodeName());
        }
    } while(hasNext());
}

bool MusicXmlNodeHelper::hasNext()
{
    bool hasNext = false;
    if(m_root.isNull() || m_current.isNull())
    {
        return hasNext;
    }

    if(m_current.hasChildNodes() && !m_current.firstChildElement().isNull())
    {
        m_current = m_current.firstChildElement();
        hasNext = true;
    }
    else if(!m_current.nextSiblingElement().isNull())
    {
        m_current = m_current.nextSiblingElement();
        hasNext = true;
    }
    else
    {
        while(m_current != m_root && m_current.nextSiblingElement().isNull())
        {
            m_current = m_current.parentNode();
        }

        if(m_current != m_root)
        {
            m_current = m_current.nextSiblingElement();
            hasNext = true;
        }
        else
        {
            hasNext = false;
        }
    }

    return hasNext;
}

QString MusicXmlNodeHelper::nodeName(const QString &name) const
{
    for(const QString &value : qAsConst(m_nodeNames))
    {
        if(value.toLower() == name.toLower())
        {
            return value;
        }
    }
    return name;
}

QDomNode MusicXmlNodeHelper::next() const
{
    return m_current;
}



MusicAbstractXml::MusicAbstractXml(QObject *parent)
    : QObject(parent)
{
    m_file = nullptr;
    m_document = nullptr;
    m_nodeHelper = nullptr;
}

MusicAbstractXml::~MusicAbstractXml()
{
    delete m_file;
    delete m_document;
    delete m_nodeHelper;
}

bool MusicAbstractXml::readConfig(const QString &name)
{
    delete m_file;
    delete m_document;
    m_file = new QFile(name);
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

    m_nodeHelper = new MusicXmlNodeHelper(m_document->documentElement());
    m_nodeHelper->load();

    return true;
}

bool MusicAbstractXml::writeConfig(const QString &name)
{
    delete m_file;
    delete m_document;
    m_file = new QFile(name);
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

    m_nodeHelper = new MusicXmlNodeHelper(m_document->documentElement());
    m_nodeHelper->load();

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

    m_nodeHelper = new MusicXmlNodeHelper(m_document->documentElement());
    m_nodeHelper->load();

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

TTKVariantMap MusicAbstractXml::readXmlAttributesByTagName(const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return TTKVariantMap();
    }

    const QDomNamedNodeMap &nodes = nodeList.at(0).toElement().attributes();
    TTKVariantMap maps;
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
    m_document->appendChild(node);
}

QDomElement MusicAbstractXml::createRoot(const QString &node)
{
    const QDomElement &domElement = m_document->createElement(node);
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement MusicAbstractXml::createRoot(const QString &node, const MusicXmlAttribute &attr)
{
    QDomElement domElement = m_document->createElement(node);
    writeAttribute(domElement, attr);
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement MusicAbstractXml::createRoot(const QString &node, const MusicXmlAttributes &attrs)
{
    QDomElement domElement = m_document->createElement(node);
    for(const MusicXmlAttribute &attr : qAsConst(attrs))
    {
        writeAttribute(domElement, attr);
    }
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement MusicAbstractXml::writeDomNode(QDomElement &element, const QString &node)
{
    const QDomElement &domElement = m_document->createElement(node);
    element.appendChild(domElement);
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
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}

QDomElement MusicAbstractXml::writeDomElementMutilText(QDomElement &element, const QString &node, const MusicXmlAttributes &attrs, const QString &text)
{
    if(attrs.isEmpty())
    {
        QDomElement();
    }

    QDomElement domElement = writeDomElementMutil(element, node, attrs);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}

QDomElement MusicAbstractXml::writeDomText(QDomElement &element, const QString &node, const QString &text)
{
    QDomElement domElement = writeDomNode(element, node);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}
