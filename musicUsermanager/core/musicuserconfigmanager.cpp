#include "musicuserconfigmanager.h"

MusicUserConfigManager::MusicUserConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicUserConfigManager::writeUserXMLConfig(const MusicUserRecord &record)
{
    if( !writeConfig( USERPATH_AL ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("QMusicPlayer");

    for(int i=0; i<record.m_names.count(); ++i)
    {
        writeDomElementMutilText(musicPlayer, "username", QStringList()<<"name"<<"userRp"<<"userAl",
                                 QList<QVariant>()<<record.m_names[i]<<record.m_rps[i]<<record.m_als[i],
                                 record.m_pwds[i]);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicUserConfigManager::readUserConfig(MusicUserRecord &record)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("username");
    QStringList names, rps, als, pwds;
    for(int i=0; i<nodelist.count(); ++i)
    {
        names << nodelist.at(i).toElement().attribute("name");
        rps << nodelist.at(i).toElement().attribute("userRp");
        als << nodelist.at(i).toElement().attribute("userAl");
        pwds << nodelist.at(i).toElement().text();
    }
    record.m_names = names;
    record.m_rps = rps;
    record.m_als = als;
    record.m_pwds = pwds;
}
