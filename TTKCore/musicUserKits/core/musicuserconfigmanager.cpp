#include "musicuserconfigmanager.h"

MusicUserConfigManager::MusicUserConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicUserConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicUserConfigManager::writeUserXMLConfig(const MusicUserRecords &records)
{
    if( !writeConfig( USERPATH_FULL ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");

    foreach(const MusicUserRecord &record, records)
    {
        writeDomElementMutilText(musicPlayer, "username", XmlAttributes() << XmlAttribute("name", record.m_userName)
                                   << XmlAttribute("userRp", record.m_rememberPWD) << XmlAttribute("userAl", record.m_autoLogin),
                                   record.m_password);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicUserConfigManager::readUserConfig(MusicUserRecords &records)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("username");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicUserRecord record;
        record.m_userName = nodelist.at(i).toElement().attribute("name");
        record.m_rememberPWD = nodelist.at(i).toElement().attribute("userRp");
        record.m_autoLogin = nodelist.at(i).toElement().attribute("userAl");
        record.m_password =  nodelist.at(i).toElement().text();
        records << record;
    }
}
