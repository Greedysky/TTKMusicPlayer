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

QString MusicXmlAbstract::readXmlByTagNameAndAttribute(const QString &tagName)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(tagName);
    return nodelist.at(0).toElement().attribute("value");
}
