#include "musicuserconfigmanager.h"

MusicUserConfigManager::MusicUserConfigManager(QObject *parent) :
    MusicAbstractXml(parent)
{

}

void MusicUserConfigManager::writeUserXMLConfig(const MStringsListMap& par)
{
    if( !writeConfig( USERPATH ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("QMusicPlayer");

    MStringsListMapIt p(par);
    while(p.hasNext())
    {
        p.next();
        QDomElement name = writeDomElement(musicPlayer, "username", "value", p.key());
        writeDomElement(name, "userRp", "value", p.value()[0]);
        writeDomElement(name, "userAl", "value", p.value()[1]);
        writeDomElement(name, "userWd", "value", p.value()[2]);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

void MusicUserConfigManager::readUserConfig(MStringsListMap &name)
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
