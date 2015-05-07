#include "musicuserconfigmanager.h"
#include <QtXml/QDomDocument>
#include <QFile>
#include <QTextStream>

MusicUserConfigManager::MusicUserConfigManager(QObject *parent) :
    QObject(parent)
{
    m_file = NULL;
    m_ddom = NULL;
}

MusicUserConfigManager::~MusicUserConfigManager()
{
    delete m_file;
    delete m_ddom;
}

bool MusicUserConfigManager::readConfig(const QString& type)
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

void MusicUserConfigManager::writeUserXMLConfig(const QMap<QString,QStringList>& par)
{
    m_file = new QFile( USERPATH );
    m_ddom = new QDomDocument;
    if( !m_file->open(QFile::WriteOnly | QFile::Text) )
      return;

    ///////////////////////////////////////////////////////
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement QMusicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(QMusicPlayer);

    QMapIterator<QString, QStringList> p(par);
    while(p.hasNext())
    {
        p.next();
        //Class A
        QDomElement userName = m_ddom->createElement("username");
        userName.setAttribute("value",p.key());
        QMusicPlayer.appendChild(userName);
        QDomElement autoLogin = m_ddom->createElement("userRp");
        autoLogin.setAttribute("value",p.value()[0]);
        userName.appendChild(autoLogin);
        QDomElement userLogin = m_ddom->createElement("userAl");
        userLogin.setAttribute("value",p.value()[1]);
        userName.appendChild(userLogin);
        QDomElement userPwdM = m_ddom->createElement("userWd");
        userPwdM.setAttribute("value",p.value()[2]);
        userName.appendChild(userPwdM);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

void MusicUserConfigManager::readUserConfig(QMap<QString, QStringList> &name)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("username");
    QDomNodeList childlist;
    for(int i=0; i<nodelist.count(); ++i)
    {
        childlist = nodelist.at(i).childNodes();
        name.insert(nodelist.at(i).toElement().attribute("value"),
                    QStringList()<<childlist.at(0).toElement().attribute("value")
                                 <<childlist.at(1).toElement().attribute("value")
                                 <<childlist.at(2).toElement().attribute("value"));
    }
}
