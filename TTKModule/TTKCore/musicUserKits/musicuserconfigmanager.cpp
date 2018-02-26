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
    QDomElement musicPlayer = createRoot(APPNAME);

    foreach(const MusicUserRecord &record, records)
    {
        writeDomElementMutilText(musicPlayer, "userName", MusicXmlAttributes() <<
                                 MusicXmlAttribute("name", record.m_userUID) <<
                                 MusicXmlAttribute("remember", record.m_rememberFlag ? 1 : 0) <<
                                 MusicXmlAttribute("auto", record.m_autoFlag ? 1 : 0) <<
                                 MusicXmlAttribute("type", record.m_type),
                                 record.m_password);
    }

    //Write to file
    QTextStream out(m_file);
    m_document->save(out, 4);
}

void MusicUserConfigManager::readUserConfig(MusicUserRecords &records)
{
    QDomNodeList nodelist = m_document->elementsByTagName("userName");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicUserRecord record;
        QDomElement element = nodelist.at(i).toElement();
        record.m_userUID = element.attribute("name");
        record.m_rememberFlag = (element.attribute("remember") == "1");
        record.m_autoFlag = (element.attribute("auto") == "1");
        record.m_type = element.attribute("type").toInt();
        record.m_password =  element.text();
        records << record;
    }
}
