#include "musicuserconfigmanager.h"

MusicUserConfigManager::MusicUserConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicUserConfigManager::readUserData(MusicUserRecords &records)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("userName");
    for(int i=0; i<nodeList.count(); ++i)
    {
        MusicUserRecord record;
        const QDomElement &element = nodeList.at(i).toElement();
        record.m_uid = element.attribute("name");
        record.m_remember = (element.attribute("remember") == "1");
        record.m_auto = (element.attribute("auto") == "1");
        record.m_server = element.attribute("type").toInt();
        record.m_password =  element.text();
        records << record;
    }
}

void MusicUserConfigManager::writeUserData(const MusicUserRecords &records)
{
    if(!writeConfig(USERPATH_FULL))
    {
        return;
    }
    //
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APP_NAME);

    for(const MusicUserRecord &record : qAsConst(records))
    {
        writeDomElementMutilText(musicPlayer, "userName", MusicXmlAttributes() <<
                                 MusicXmlAttribute("name", record.m_uid) <<
                                 MusicXmlAttribute("remember", record.m_remember ? 1 : 0) <<
                                 MusicXmlAttribute("auto", record.m_auto ? 1 : 0) <<
                                 MusicXmlAttribute("type", record.m_server), record.m_password);
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
