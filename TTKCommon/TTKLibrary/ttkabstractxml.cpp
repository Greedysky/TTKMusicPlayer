#include "ttkabstractxml.h"

TTKXmlNodeHelper::TTKXmlNodeHelper(const QDomNode& root)
    : m_root(root),
      m_current(root)
{

}

void TTKXmlNodeHelper::load()
{
    do
    {
        if(!m_current.isNull() && m_current.isElement())
        {
            m_nodeNames.insert(m_current.nodeName());
        }
    } while(hasNext());
}

bool TTKXmlNodeHelper::hasNext()
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

QDomNode TTKXmlNodeHelper::next() const
{
    return m_current;
}

QString TTKXmlNodeHelper::nodeName(const QString &name) const
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



TTKAbstractXml::TTKAbstractXml(QObject *parent)
    : QObject(parent),
      m_file(nullptr),
      m_document(nullptr)
{

}

TTKAbstractXml::~TTKAbstractXml()
{
    delete m_file;
    delete m_document;
}

bool TTKAbstractXml::fromFile(const QString &name)
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
    return true;
}

bool TTKAbstractXml::fromString(const QString &data)
{
    delete m_file;
    delete m_document;

    m_file = nullptr;
    m_document = new QDomDocument;
#if TTK_QT_VERSION_CHECK(6,5,0)
    return TTKStatic_cast(bool, m_document->setContent(data));
#else
    return m_document->setContent(data);
#endif
}

bool TTKAbstractXml::fromByteArray(const QByteArray &data)
{
    delete m_file;
    delete m_document;

    m_file = nullptr;
    m_document = new QDomDocument;
#if TTK_QT_VERSION_CHECK(6,5,0)
    return TTKStatic_cast(bool, m_document->setContent(data));
#else
    return m_document->setContent(data);
#endif
}

bool TTKAbstractXml::toFile(const QString &name)
{
    delete m_file;
    delete m_document;

    m_file = new QFile(name);
    m_document = new QDomDocument;
    return m_file->open(QIODevice::WriteOnly | QIODevice::Text);
}

QString TTKAbstractXml::toString() const
{
    return m_document ? m_document->toString() : QString();
}

QByteArray TTKAbstractXml::toByteArray() const
{
    return m_document ? m_document->toByteArray() : QByteArray();
}

QString TTKAbstractXml::readXmlAttributeByTagNameValue(const QString &tagName) const
{
    return readXmlAttributeByTagName(tagName, "value");
}

QString TTKAbstractXml::readXmlAttributeByTagName(const QString &tagName, const QString &attrName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return QString();
    }
    return nodeList.at(0).toElement().attribute(attrName);
}

QString TTKAbstractXml::readXmlTextByTagName(const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return QString();
    }
    return nodeList.at(0).toElement().text();
}

TTKVariantMap TTKAbstractXml::readXmlAttributesByTagName(const QString &tagName) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName(tagName);
    if(nodeList.isEmpty())
    {
        return TTKVariantMap();
    }

    const QDomNamedNodeMap &nodes = nodeList.at(0).toElement().attributes();
    TTKVariantMap maps;
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomAttr &attr = nodes.item(i).toAttr();
        maps[attr.name()] = attr.value();
    }
    return maps;
}

void TTKAbstractXml::createProcessingInstruction()
{
    const QDomNode &node = m_document->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    m_document->appendChild(node);
}

QDomElement TTKAbstractXml::createRoot(const QString &node)
{
    const QDomElement &domElement = m_document->createElement(node);
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::createRoot(const QString &node, const TTKXmlAttribute &attr)
{
    QDomElement domElement = m_document->createElement(node);
    writeAttribute(domElement, attr);
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::createRoot(const QString &node, const TTKXmlAttributeList &attrs)
{
    QDomElement domElement = m_document->createElement(node);
    for(const TTKXmlAttribute &attr : qAsConst(attrs))
    {
        writeAttribute(domElement, attr);
    }
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomNode(QDomElement &element, const QString &node)
{
    const QDomElement &domElement = m_document->createElement(node);
    element.appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomElement(QDomElement &element, const QString &node, const TTKXmlAttribute &attr)
{
    QDomElement domElement = writeDomNode(element, node);
    writeAttribute(domElement, attr);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomMutilElement(QDomElement &element, const QString &node, const TTKXmlAttributeList &attrs)
{
    if(attrs.isEmpty())
    {
        QDomElement();
    }

    QDomElement domElement = writeDomElement(element, node, attrs.front());
    for(int i = 1; i < attrs.count(); ++i)
    {
        writeAttribute(domElement, attrs[i]);
    }
    return domElement;
}

void TTKAbstractXml::writeAttribute(QDomElement &element, const TTKXmlAttribute &attr)
{
    switch(QtVariantType(attr.m_value))
    {
        case QVariant::Int: element.setAttribute(attr.m_key, attr.m_value.toInt()); break;
        case QVariant::String: element.setAttribute(attr.m_key, attr.m_value.toString()); break;
        case QVariant::LongLong: element.setAttribute(attr.m_key, attr.m_value.toLongLong()); break;
        case QVariant::ULongLong: element.setAttribute(attr.m_key, attr.m_value.toULongLong()); break;
        case QVariant::Double: element.setAttribute(attr.m_key, attr.m_value.toDouble()); break;
        case QVariant::UInt: element.setAttribute(attr.m_key, attr.m_value.toUInt()); break;
        default: break;
    }
}

QDomElement TTKAbstractXml::writeDomElementText(QDomElement &element, const QString &node, const TTKXmlAttribute &attr, const QString &text)
{
    QDomElement domElement = writeDomElement(element, node, attr);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomMutilElementText(QDomElement &element, const QString &node, const TTKXmlAttributeList &attrs, const QString &text)
{
    if(attrs.isEmpty())
    {
        QDomElement();
    }

    QDomElement domElement = writeDomMutilElement(element, node, attrs);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomText(QDomElement &element, const QString &node, const QString &text)
{
    QDomElement domElement = writeDomNode(element, node);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}
