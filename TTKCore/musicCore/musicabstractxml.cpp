#include "musicabstractxml.h"

#ifdef Q_CC_GNU
#   pragma GCC diagnostic ignored "-Wswitch"
#endif

MusicAbstractXml::MusicAbstractXml(QObject *parent)
    : QObject(parent)
{
    m_file = nullptr;
    m_ddom = nullptr;
}

MusicAbstractXml::~MusicAbstractXml()
{
    delete m_file;
    delete m_ddom;
}

QString MusicAbstractXml::getClassName()
{
    return staticMetaObject.className();
}

bool MusicAbstractXml::readConfig(const QString &name)
{
    delete m_file;
    delete m_ddom;
    m_file = new QFile( name );
    m_ddom = new QDomDocument;
    if( !m_file->open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        return false;
    }
    if( !m_ddom->setContent(m_file) )
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
    delete m_ddom;
    m_file = new QFile( name );
    m_ddom = new QDomDocument;
    if( !m_file->open(QFile::WriteOnly | QFile::Text) )
    {
        return false;
    }
    return true;
}

QString MusicAbstractXml::readXmlAttributeByTagNameValue(const QString &tagName) const
{
    return readXmlAttributeByTagName(tagName, "value");
}

QString MusicAbstractXml::readXmlAttributeByTagName(const QString &tagName,
                                                    const QString &attrName) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(tagName);
    if(nodelist.isEmpty())
    {
        return QString();
    }
    return nodelist.at(0).toElement().attribute(attrName);
}

QString MusicAbstractXml::readXmlTextByTagName(const QString &tagName) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(tagName);
    if(nodelist.isEmpty())
    {
        return QString();
    }
    return nodelist.at(0).toElement().text();
}

MusicObject::MStriantMap MusicAbstractXml::readXmlAttributesByTagName(const QString &tagName) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(tagName);
    if(nodelist.isEmpty())
    {
        return MusicObject::MStriantMap();
    }
    QDomNamedNodeMap nodes = nodelist.at(0).toElement().attributes();
    MusicObject::MStriantMap maps;
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomAttr attr = nodes.item(i).toAttr();
        maps[attr.name()] = attr.value();
    }
    return maps;
}

void MusicAbstractXml::createProcessingInstruction()
{
    QDomNode node = m_ddom->createProcessingInstruction("xml",
                            "version='1.0' encoding='UTF-8'");
    m_ddom->appendChild( node );
}

QDomElement MusicAbstractXml::createRoot(const QString &node)
{
    QDomElement domElement = m_ddom->createElement( node );
    m_ddom->appendChild( domElement );
    return domElement;
}

QDomElement MusicAbstractXml::writeDom(QDomElement &element, const QString &node)
{
    QDomElement domElement = m_ddom->createElement( node );
    element.appendChild( domElement );
    return domElement;
}

QDomElement MusicAbstractXml::writeDomElement(QDomElement &element, const QString &node,
                                              const QString &key, const QVariant &value)
{
    QDomElement domElement = writeDom(element, node);
    writeAttribute(domElement, key, value);
    return domElement;
}

QDomElement MusicAbstractXml::writeDomElementMutil(QDomElement &element, const QString &node,
                                                   const QStringList &keys,
                                                   const QVariantList &values)
{
    if(keys.isEmpty() || values.isEmpty())
    {
        QDomElement();
    }

    QDomElement domElement = writeDomElement(element, node, keys.front(), values.front());
    for(int i=1; i<keys.count(); ++i)
    {
        writeAttribute(domElement, keys[i], values[i]);
    }
    return domElement;
}

void MusicAbstractXml::writeAttribute(QDomElement &element, const QString &key,
                                      const QVariant &value)
{
    switch(value.type())
    {
        case QVariant::Int :
            element.setAttribute(key, value.toInt()); break;
        case QVariant::String :
            element.setAttribute(key, value.toString()); break;
        case QVariant::LongLong :
            element.setAttribute(key, value.toLongLong()); break;
        case QVariant::ULongLong :
            element.setAttribute(key, value.toULongLong()); break;
        case QVariant::Double :
            element.setAttribute(key, value.toDouble()); break;
        case QVariant::UInt :
            element.setAttribute(key, value.toUInt()); break;
    }
}

QDomElement MusicAbstractXml::writeDomElementText(QDomElement &element, const QString &node,
                                                  const QString &key, const QVariant &value,
                                                  const QString &text)
{
    QDomElement domElement = writeDomElement(element, node, key, value);
    QDomText domText = m_ddom->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}

QDomElement MusicAbstractXml::writeDomElementMutilText(QDomElement &element, const QString &node,
                                                       const QStringList &keys, const QVariantList &values,
                                                       const QString &text)
{
    if(keys.isEmpty() || values.isEmpty())
    {
        QDomElement();
    }

    QDomElement domElement = writeDomElementMutil(element, node, keys, values);
    QDomText domText = m_ddom->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}

QDomElement MusicAbstractXml::writeDomText(QDomElement &element, const QString &node,
                                           const QString &text)
{
    QDomElement domElement = writeDom(element, node);
    QDomText domText = m_ddom->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}
