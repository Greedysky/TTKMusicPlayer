#include "musicmydownloadrecordobject.h"
#include <QtXml/QDomDocument>
#include <QFile>
#include <QTextStream>

MusicMyDownloadRecordObject::MusicMyDownloadRecordObject(QObject *parent) :
    QObject(parent)
{
    m_file = NULL;
    m_ddom = NULL;
}

MusicMyDownloadRecordObject::~MusicMyDownloadRecordObject()
{
    delete m_file;
    delete m_ddom;
}

bool MusicMyDownloadRecordObject::readConfig(const QString& type)
{
    delete m_file;
    delete m_ddom;
    m_file = new QFile( type );
    m_ddom = new QDomDocument;
    if( !m_file->open(QIODevice::ReadOnly | QIODevice::Text) )
        return false;
    if( !m_ddom->setContent(m_file) )
    {
        m_file->close();
        delete m_file;
        m_file = NULL;
        return false;
    }
    return true;
}

void MusicMyDownloadRecordObject::writeDownloadConfig(const QStringList& names,
                                                     const QStringList& paths)
{
    delete m_file;
    delete m_ddom;
    m_file = new QFile( DOWNLOADINFO );
    m_ddom = new QDomDocument;
    if( !m_file->open(QFile::WriteOnly | QFile::Text) )
      return;

    ///////////////////////////////////////////////////////
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement QMusicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(QMusicPlayer);

    QDomElement download = m_ddom->createElement("download");
    QMusicPlayer.appendChild(download);
    for(int i=0; i<names.count(); ++i)
    {
        QDomElement value = m_ddom->createElement("value");
        value.setAttribute("value", names[i]);
        QDomText valuetext = m_ddom->createTextNode(paths[i]);
        value.appendChild(valuetext);
        download.appendChild(value);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

void MusicMyDownloadRecordObject::readDownloadConfig(QStringList &name, QStringList &path)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        name<<nodelist.at(i).toElement().attribute("value");
        path<<nodelist.at(i).toElement().text();
    }
}
