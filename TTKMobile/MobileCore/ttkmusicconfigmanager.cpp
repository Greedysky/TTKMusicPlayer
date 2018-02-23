#include "ttkmusicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicversion.h"

TTKMusicConfigManager::TTKMusicConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void TTKMusicConfigManager::writeXMLConfig()
{
    int downloadServer = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    ///////////////////////////////////////////////////////////////////////////
    //Open wirte file
    if( !writeConfig(COFIGPATH_FULL) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    //Class A
    QDomElement music = writeDom(musicPlayer, "music");
    QDomElement downloads = writeDom(musicPlayer, "downloads");
    //Class B
    writeDomElement(music, "ver", MusicXmlAttribute("value", TTKMUSIC_VERSION_STR));
    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(downloads, "downloadServer", MusicXmlAttribute("value", downloadServer));
    ///////////////////////////////////////////////////////////////////////////
    //Write to file
    QTextStream out(m_file);
    m_document->save(out, 4);
}

bool TTKMusicConfigManager::readNeedUpdateConfig()
{
    QString v = readXmlAttributeByTagNameValue("ver");
    return v.isEmpty() || v != TTKMUSIC_VERSION_STR;
}

void TTKMusicConfigManager::readOtherLoadConfig() const
{
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced,
                   readXmlAttributeByTagNameValue("downloadServer").toInt());
}
