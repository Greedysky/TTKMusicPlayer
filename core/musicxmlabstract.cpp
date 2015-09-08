#include "musicxmlabstract.h"

MusicXmlAbstract::MusicXmlAbstract(QObject *parent)
    : QObject(parent)
{
    m_file = NULL;
    m_ddom = NULL;
}

MusicXmlAbstract::~MusicXmlAbstract()
{
    delete m_file;
    delete m_ddom;
}

bool MusicXmlAbstract::readConfig(const QString &type)
{
    delete m_file;
    delete m_ddom;
    m_file = new QFile( type );
    m_ddom = new QDomDocument;
    if( !m_file->open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        return false;
    }
    if( !m_ddom->setContent(m_file) )
    {
        m_file->close();
        delete m_file;
        m_file = NULL;
        return false;
    }
    return true;
}

bool MusicXmlAbstract::writeConfig(const QString &type)
{
    delete m_file;
    delete m_ddom;
    m_file = new QFile( type );
    m_ddom = new QDomDocument;
    if( !m_file->open(QFile::WriteOnly | QFile::Text) )
    {
        return false;
    }
    return true;
}

QString MusicXmlAbstract::readXmlByTagNameAndAttribute(const QString &tagName) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(tagName);
    return nodelist.at(0).toElement().attribute("value");
}

void MusicXmlAbstract::createProcessingInstruction()
{
    QDomNode node = m_ddom->createProcessingInstruction("xml",
                            "version='1.0' encoding='UTF-8'");
    m_ddom->appendChild( node );
}

QDomElement MusicXmlAbstract::createRoot(const QString &node)
{
    QDomElement domElement = m_ddom->createElement( node );
    m_ddom->appendChild( domElement );
    return domElement;
}

QDomElement MusicXmlAbstract::writeDom(QDomElement &element, const QString &node)
{
    QDomElement domElement = m_ddom->createElement( node );
    element.appendChild( domElement );
    return domElement;
}

QDomElement MusicXmlAbstract::writeDomElement(QDomElement &element, const QString &node,
                                              const QString &key, const QVariant &value)
{
    QDomElement domElement = writeDom(element, node);
    switch(value.type())
    {
        case QVariant::Int :
            domElement.setAttribute(key, value.toInt()); break;
        case QVariant::String :
            domElement.setAttribute(key, value.toString()); break;
        case QVariant::LongLong :
            domElement.setAttribute(key, value.toLongLong()); break;
        case QVariant::ULongLong :
            domElement.setAttribute(key, value.toULongLong()); break;
        case QVariant::Double :
            domElement.setAttribute(key, value.toDouble()); break;
        case QVariant::UInt :
            domElement.setAttribute(key, value.toUInt()); break;
    }
    return domElement;
}

QDomElement MusicXmlAbstract::writeDomEleText(QDomElement &element, const QString &node,
                                              const QString &key, const QVariant &value,
                                              const QString &text)
{
    QDomElement domElement = writeDomElement(element, node, key, value);
    QDomText domText = m_ddom->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}

QDomElement MusicXmlAbstract::writeDomText(QDomElement &element, const QString &node,
                                           const QString &text)
{
    QDomElement domElement = writeDom(element, node);
    QDomText domText = m_ddom->createTextNode( text );
    domElement.appendChild( domText );
    return domElement;
}
