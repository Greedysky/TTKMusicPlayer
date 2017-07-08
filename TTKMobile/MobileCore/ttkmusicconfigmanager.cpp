#include "ttkmusicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicversion.h"

TTKMusicConfigManager::TTKMusicConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void TTKMusicConfigManager::writeXMLConfig()
{
    int playModeChoiced = M_SETTING_PTR->value(MusicSettingManager::PlayModeChoiced).toInt();
    int volumeChoiced = M_SETTING_PTR->value(MusicSettingManager::VolumeChoiced).toInt();
    QStringList lastPlayIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    int mobileWifiConnectChoiced = M_SETTING_PTR->value(MusicSettingManager::MobileWifiConnectChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int enhancedMusicChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt();
    int equalizerEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerEnableChoiced).toInt();
    QString equalizerValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerValueChoiced).toString();
    int equalizerIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int lrcColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt();
    int lrcSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
    int lrcTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcTypeChoiced).toInt();
    QString lrcFgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcFgColorChoiced).toString();

    ///////////////////////////////////////////////////////////////////////////
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
    QDomElement equalizer = writeDom(musicPlayer, "equalizer");
    QDomElement showLrc = writeDom(musicPlayer, "inlineLrc");
    QDomElement downloads = writeDom(musicPlayer, "downloads");
    //Class B
    writeDomElement(music, "ver", MusicXmlAttribute("value", TTKMUSIC_VERSION_STR));
    writeDomElement(music, "playMode", MusicXmlAttribute("value", playModeChoiced));
    writeDomElement(music, "playVolume", MusicXmlAttribute("value", volumeChoiced));
    writeDomElementText(music, "lastPlayIndex", MusicXmlAttribute("value", lastPlayIndexChoiced[0]),
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));
    writeDomElement(music, "wifiConnect", MusicXmlAttribute("value", mobileWifiConnectChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(equalizer, "enhancedMusic", MusicXmlAttribute("value", enhancedMusicChoiced));
    writeDomElement(equalizer, "equalizerEnable", MusicXmlAttribute("value", equalizerEnableChoiced));
    writeDomElement(equalizer, "equalizerIndex", MusicXmlAttribute("value", equalizerIndexChoiced));
    writeDomElement(equalizer, "equalizerValue", MusicXmlAttribute("value", equalizerValueChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(showLrc, "lrcColor", MusicXmlAttribute("value", lrcColorChoiced));
    writeDomElement(showLrc, "lrcSize", MusicXmlAttribute("value", lrcSizeChoiced));
    writeDomElement(showLrc, "lrcType", MusicXmlAttribute("value", lrcTypeChoiced));
    writeDomElement(showLrc, "lrcFgColor", MusicXmlAttribute("value", lrcFgColorChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(downloads, "downloadServer", MusicXmlAttribute("value", downloadServer));
    ///////////////////////////////////////////////////////////////////////////
    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void TTKMusicConfigManager::writeMusicSongsConfig(const MusicSongItems &musics, const QString &path)
{
    if( musics.isEmpty() )
    {
        return;
    }
    //Open wirte file
    if( !writeConfig( path ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    for(int i=0; i<musics.count(); ++i)
    {

        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList",
                                                   MusicXmlAttributes() << MusicXmlAttribute("name", musics[i].m_itemName) << MusicXmlAttribute("index", i)
                                                   << MusicXmlAttribute("count", musics[i].m_songs.count()));
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutilText(pathDom, "value", MusicXmlAttributes() << MusicXmlAttribute("name", song.getMusicName())
                                     << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                     << MusicXmlAttribute("time", song.getMusicTime()), song.getMusicPath());
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void TTKMusicConfigManager::readMusicSongsConfig(MusicSongItems &musics)
{
    QDomNodeList nodes = m_ddom->elementsByTagName("musicList");
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        QDomElement element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");
        musics << item;
    }
}

bool TTKMusicConfigManager::readNeedUpdateConfig()
{
    QString v = readXmlAttributeByTagNameValue("ver");
    return v.isEmpty() || v != TTKMUSIC_VERSION_STR;
}

void TTKMusicConfigManager::readSystemLastPlayIndexConfig(QStringList &key) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("lastPlayIndex");
    if(nodelist.isEmpty())
    {
        key << "0" << "0" << "-1";
        return;
    }

    QDomElement element = nodelist.at(0).toElement();
    key << element.attribute("value") << element.text().split(',');
    if(key.count() != 3)
    {
        key.clear();
        key << "0" << "0" << "-1";
    }
}

void TTKMusicConfigManager::readOtherLoadConfig() const
{
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced,
                     readXmlAttributeByTagNameValue("downloadServer").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::MobileWifiConnectChoiced,
                     readXmlAttributeByTagNameValue("wifiConnect").toInt());
}

MusicSongs TTKMusicConfigManager::readMusicFilePath(const QDomNode &node) const
{
    QDomNodeList nodelist = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodelist.count(); i++)
    {
        QDomElement element = nodelist.at(i).toElement();
        songs << MusicSong(element.text(),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}
