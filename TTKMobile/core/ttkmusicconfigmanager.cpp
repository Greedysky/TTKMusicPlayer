#include "ttkmusicconfigmanager.h"
#include "musicsettingmanager.h"

TTKMusicConfigManager::TTKMusicConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void TTKMusicConfigManager::writeXMLConfig()
{
    int playModeChoiced = M_SETTING_PTR->value(MusicSettingManager::PlayModeChoiced).toInt();
    int volumeChoiced = M_SETTING_PTR->value(MusicSettingManager::VolumeChoiced).toInt();
    QStringList lastPlayIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();

    ///////////////////////////////////////////////////////////////////////////
    int enhancedMusicChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt();
    int equalizerEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerEnableChoiced).toInt();
    QString equalizerValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerValueChoiced).toString();
    int equalizerIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt();

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
    //Class B
    writeDomElement(music, "playMode", "value", playModeChoiced);
    writeDomElement(music, "playVolume", "value", volumeChoiced);
    writeDomElementText(music, "lastPlayIndex", "value", lastPlayIndexChoiced[0],
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(equalizer, "enhancedMusic", "value", enhancedMusicChoiced);
    writeDomElement(equalizer, "equalizerEnable", "value", equalizerEnableChoiced);
    writeDomElement(equalizer, "equalizerIndex", "value", equalizerIndexChoiced);
    writeDomElement(equalizer, "equalizerValue", "value", equalizerValueChoiced);

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
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList", QStringList() << "name" << "index" << "count",
                                  QVariantList() << musics[i].m_itemName << i << musics[i].m_songs.count());
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutilText(pathDom, "value", QStringList() << "name" << "playCount" << "time",
                                     QVariantList() << song.getMusicName() << song.getMusicPlayCount()
                                                       << song.getMusicTime(), song.getMusicPath());
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
