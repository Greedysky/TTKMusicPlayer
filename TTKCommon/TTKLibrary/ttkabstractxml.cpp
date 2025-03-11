#include "ttkabstractxml.h"

#include <QStringList>

TTKXmlHelper::TTKXmlHelper(const QDomNode &root) noexcept
    : m_root(root),
      m_current(root)
{

}

void TTKXmlHelper::load()
{
    do
    {
        if(!m_current.isNull() && m_current.isElement())
        {
            m_nodeNames.insert(m_current.nodeName());
        }
    } while(hasNext());
}

bool TTKXmlHelper::hasNext()
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

QDomNode TTKXmlHelper::next() const noexcept
{
    return m_current;
}

QString TTKXmlHelper::nodeName(const QString &name) const noexcept
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



TTKAbstractXml::TTKAbstractXml()
    : m_file(nullptr),
      m_document(nullptr)
{

}

TTKAbstractXml::~TTKAbstractXml()
{
    delete m_file;
    delete m_document;
}

bool TTKAbstractXml::load(const QString &name)
{
    delete m_file;
    delete m_document;

    m_file = new QFile(name);
    m_document = new QDomDocument;
    return m_file->open(QIODevice::WriteOnly | QIODevice::Text);
}

void TTKAbstractXml::save() const
{
    if(!m_file || !m_document)
    {
        return;
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}

bool TTKAbstractXml::reset()
{
    if(!m_file || !m_document)
    {
        return false;
    }

    return load(m_file->fileName());
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
    return TTKStaticCast(bool, m_document->setContent(data));
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
    return TTKStaticCast(bool, m_document->setContent(data));
#else
    return m_document->setContent(data);
#endif
}

QString TTKAbstractXml::toString() const
{
    return m_document ? m_document->toString() : QString();
}

QByteArray TTKAbstractXml::toByteArray() const
{
    return m_document ? m_document->toByteArray() : QByteArray();
}

void TTKAbstractXml::createProcessingInstruction() const
{
    const QDomNode &node = m_document->createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
    m_document->appendChild(node);
}

QString TTKAbstractXml::readAttributeByTagName(const QString &tagName, const QString &attrName) const
{
    const QDomNodeList &nodes = m_document->elementsByTagName(tagName);
    return nodes.isEmpty() ? QString() : nodes.item(0).toElement().attribute(attrName);
}

QString TTKAbstractXml::readTextByTagName(const QString &tagName) const
{
    const QDomNodeList &nodes = m_document->elementsByTagName(tagName);
    return nodes.isEmpty() ? QString() : nodes.item(0).toElement().text();
}

TTKXmlNode TTKAbstractXml::readNodeByTagName(const QString &tagName) const
{
    const QDomNodeList &nodes = m_document->elementsByTagName(tagName);
    if(nodes.isEmpty())
    {
        return {};
    }

    TTKXmlNode v;
    const QDomElement &element = nodes.item(0).toElement();
    const QDomNamedNodeMap &nodeMap = element.attributes();

    for(int i = 0; i < nodeMap.count(); ++i)
    {
        const QDomAttr &attr = nodeMap.item(i).toAttr();
        v.m_attrs << TTKXmlAttr(attr.name(), attr.value());
    }

    v.m_text = element.text();
    return v;
}

QStringList TTKAbstractXml::readMultiAttributeByTagName(const QString &tagName, const QString &attrName) const
{
    const QDomNodeList &nodes = m_document->elementsByTagName(tagName);
    if(nodes.isEmpty())
    {
        return {};
    }

    QStringList v;
    for(int i = 0; i < nodes.count(); ++i)
    {
        v << nodes.item(i).toElement().attribute(attrName);
    }
    return v;
}

QStringList TTKAbstractXml::readMultiTextByTagName(const QString &tagName) const
{
    const QDomNodeList &nodes = m_document->elementsByTagName(tagName);
    if(nodes.isEmpty())
    {
        return {};
    }

    QStringList v;
    for(int i = 0; i < nodes.count(); ++i)
    {
        v << nodes.item(i).toElement().text();
    }
    return v;
}

TTKXmlNodeList TTKAbstractXml::readMultiNodeByTagName(const QString &tagName) const
{
    const QDomNodeList &nodes = m_document->elementsByTagName(tagName);
    if(nodes.isEmpty())
    {
        return {};
    }

    TTKXmlNodeList v;
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomElement &element = nodes.item(i).toElement();
        const QDomNamedNodeMap &nodeMap = element.attributes();

        TTKXmlNode n;
        for(int j = 0; j < nodeMap.count(); ++j)
        {
            const QDomAttr &attr = nodeMap.item(j).toAttr();
            n.m_attrs << TTKXmlAttr(attr.name(), attr.value());
        }

        n.m_text = element.text();
        v << n;
    }
    return v;
}

QDomElement TTKAbstractXml::createRoot(const QString &node) const
{
    const QDomElement &domElement = m_document->createElement(node);
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::createRoot(const QString &node, const TTKXmlAttr &attr) const
{
    QDomElement domElement = m_document->createElement(node);
    writeAttribute(domElement, attr);
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::createRoot(const QString &node, const TTKXmlAttrList &attrs) const
{
    QDomElement domElement = m_document->createElement(node);
    writeAttribute(domElement, attrs);
    m_document->appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomElement(QDomElement &element, const QString &node) const
{
    const QDomElement &domElement = m_document->createElement(node);
    element.appendChild(domElement);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomElement(QDomElement &element, const QString &node, const QString &text) const
{
    QDomElement domElement = writeDomElement(element, node);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomElement(QDomElement &element, const QString &node, const TTKXmlNode &attr) const
{
    return writeDomMultiElement(element, node, attr);
}

QDomElement TTKAbstractXml::writeDomElement(QDomElement &element, const QString &node, const TTKXmlAttr &attr) const
{
    QDomElement domElement = writeDomElement(element, node);
    writeAttribute(domElement, attr);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomElement(QDomElement &element, const QString &node, const TTKXmlAttr &attr, const QString &text) const
{
    QDomElement domElement = writeDomElement(element, node, attr);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomMultiElement(QDomElement &element, const QString &node, const TTKXmlNode &attrs) const
{
    return writeDomMultiElement(element, node, attrs.m_attrs, attrs.m_text);
}

QDomElement TTKAbstractXml::writeDomMultiElement(QDomElement &element, const QString &node, const TTKXmlAttrList &attrs) const
{
    if(attrs.isEmpty())
    {
        return {};
    }

    QDomElement domElement = writeDomElement(element, node);
    writeAttribute(domElement, attrs);
    return domElement;
}

QDomElement TTKAbstractXml::writeDomMultiElement(QDomElement &element, const QString &node, const TTKXmlAttrList &attrs, const QString &text) const
{
    if(attrs.isEmpty())
    {
        return {};
    }

    QDomElement domElement = writeDomMultiElement(element, node, attrs);
    const QDomText &domText = m_document->createTextNode(text);
    domElement.appendChild(domText);
    return domElement;
}

void TTKAbstractXml::writeAttribute(QDomElement &element, const TTKXmlAttr &attr) const
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

void TTKAbstractXml::writeAttribute(QDomElement &element, const TTKXmlAttrList &attrs) const
{
    for(const TTKXmlAttr &attr : qAsConst(attrs))
    {
        writeAttribute(element, attr);
    }
}
