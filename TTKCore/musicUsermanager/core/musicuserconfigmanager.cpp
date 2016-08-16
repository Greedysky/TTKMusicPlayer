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
        writeDomElementMutilText(musicPlayer, "username", QStringList() << "name" << "userRp" << "userAl",
                                 QVariantList() << record.m_name << record.m_rp << record.m_al,
                                 record.m_pwd);
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
        record.m_name = nodelist.at(i).toElement().attribute("name");
        record.m_rp = nodelist.at(i).toElement().attribute("userRp");
        record.m_al = nodelist.at(i).toElement().attribute("userAl");
        record.m_pwd =  nodelist.at(i).toElement().text();
        records << record;
    }
}
