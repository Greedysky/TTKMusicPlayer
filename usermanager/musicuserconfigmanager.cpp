#include "musicuserconfigmanager.h"

MusicUserConfigManager::MusicUserConfigManager(QObject *parent) :
    MusicXmlAbstract(parent)
{

}

void MusicUserConfigManager::writeUserXMLConfig(const QMap<QString,QStringList>& par)
{
    if( !writeConfig( USERPATH ) ) return;
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
