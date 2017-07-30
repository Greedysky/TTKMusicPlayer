#include "musicplaylistmanager.h"
#include "musicxmlconfigmanager.h"
#include "musictime.h"
#include "musicversion.h"
#include "musicmessagebox.h"

MusicWPLConfigManager::MusicWPLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicWPLConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicWPLConfigManager::writeMusicSongsConfig(const MusicSongItems &musics, const QString &path)
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
    QDomNode node = m_ddom->createProcessingInstruction("wpl", "version='1.0' encoding='UTF-8'");
    m_ddom->appendChild( node );
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot("smil");
    //Class A
    QDomElement headSettingDom = writeDom(musicPlayerDom, "head");
    QDomElement bodySettingDom = writeDom(musicPlayerDom, "body");
    //Class B
    writeDomElementMutil(headSettingDom, "meta", MusicXmlAttributes() << MusicXmlAttribute("name", "Generator") <<
                         MusicXmlAttribute("content", QString("%1 %2").arg(APPNAME).arg(TTKMUSIC_VERSION_STR)));
//    writeDomElementMutil(headSettingDom, "meta", MusicXmlAttributes() << MusicXmlAttribute("name", "ItemCount") <<
//                         MusicXmlAttribute("content", QString("%1").arg(musics.count())));
    for(int i=0; i<musics.count(); ++i)
    {
        const MusicSongItem &item = musics[i];
        //Class C
        QDomElement seqDom = writeDomElementMutil(bodySettingDom, "seq", MusicXmlAttributes()
                                              << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                              << MusicXmlAttribute("count", item.m_songs.count())
                                              << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                              << MusicXmlAttribute("sortType", item.m_sort.m_sortType));
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutil(seqDom, "media", MusicXmlAttributes() << MusicXmlAttribute("name", song.getMusicName())
                                 << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                 << MusicXmlAttribute("time", song.getMusicTime())
                                 << MusicXmlAttribute("src", song.getMusicPath()));
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicWPLConfigManager::readMusicSongsConfig(MusicSongItems &musics)
{
    bool state = false;
    QDomNodeList nodes = m_ddom->elementsByTagName("head");
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNodeList nodelist = nodes.at(i).childNodes();
        for(int i=0; i<nodelist.count(); ++i)
        {
            QDomElement element = nodelist.at(i).toElement();
            state = element.attribute("content").contains(APPNAME);
        }
    }

    if(!state)
    {
        MusicPlayListManager::messageAlert();
        return;
    }

    nodes = m_ddom->elementsByTagName("seq");
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        QDomElement element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");
        QString string = element.attribute("sortIndex");
        item.m_sort.m_index = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        musics << item;
    }
}

MusicSongs MusicWPLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    QDomNodeList nodelist = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodelist.count(); i++)
    {
        QDomElement element = nodelist.at(i).toElement();
        songs << MusicSong(element.attribute("src"),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}



MusicXSPFConfigManager::MusicXSPFConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicXSPFConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicXSPFConfigManager::writeMusicSongsConfig(const MusicSongItems &musics, const QString &path)
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
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot("playlist");
    //Class A
    writeDomText(musicPlayerDom, "creator", APPNAME);
    for(int i=0; i<musics.count(); ++i)
    {
        const MusicSongItem &item = musics[i];
        //Class A
        QDomElement trackListDom = writeDomElementMutil(musicPlayerDom, "trackList", MusicXmlAttributes()
                                              << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                              << MusicXmlAttribute("count", item.m_songs.count())
                                              << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                              << MusicXmlAttribute("sortType", item.m_sort.m_sortType));

        foreach(const MusicSong &song, musics[i].m_songs)
        {
            //Class B
            QDomElement trackDom = writeDomElementMutil(trackListDom, "track", MusicXmlAttributes()
                                                        << MusicXmlAttribute("name", song.getMusicName())
                                                        << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                                        << MusicXmlAttribute("time", song.getMusicTime())
                                                        << MusicXmlAttribute("src", song.getMusicPath()));
            writeDomText(trackDom, "location", song.getMusicPath());
            writeDomText(trackDom, "title", song.getMusicArtistBack());
            writeDomText(trackDom, "creator", song.getMusicArtistFront());
            writeDomText(trackDom, "annotation", QString());
            writeDomText(trackDom, "album", QString());
            writeDomText(trackDom, "trackNum", QString());
            writeDomElementText(trackDom, "meta", MusicXmlAttribute("rel", "year"), QString());
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicXSPFConfigManager::readMusicSongsConfig(MusicSongItems &musics)
{
    bool state = false;
    QDomNodeList nodes = m_ddom->elementsByTagName("playlist");
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNodeList nodelist = nodes.at(i).childNodes();
        for(int i=0; i<nodelist.count(); ++i)
        {
            QDomNode node = nodelist.at(i);
            if(node.nodeName() == "creator")
            {
                QDomElement element = node.toElement();
                state = element.text().contains(APPNAME);
            }
        }
    }

    if(!state)
    {
        MusicPlayListManager::messageAlert();
        return;
    }

    nodes = m_ddom->elementsByTagName("trackList");
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        QDomElement element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");
        QString string = element.attribute("sortIndex");
        item.m_sort.m_index = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        musics << item;
    }
}

MusicSongs MusicXSPFConfigManager::readMusicFilePath(const QDomNode &node) const
{
    QDomNodeList nodelist = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodelist.count(); i++)
    {
        QDomElement element = nodelist.at(i).toElement();
        songs << MusicSong(element.attribute("src"),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}



QString MusicPlayListManager::getClassName()
{
    return "MusicPlayListManager";
}

void MusicPlayListManager::messageAlert()
{
    MusicMessageBox message;
    message.setText(QObject::tr("Unrecognized PlayList File!"));
    message.exec();
}

void MusicPlayListManager::setMusicSongItems(const QString &save, const MusicSongItem &item)
{
    QFileInfo info(save);
    QString suffix = info.suffix().toLower();
    if(suffix == "lis")
    {
        writeLisList(save, item);
    }
    else if(suffix == "m3u" || suffix == "m3u8")
    {
        writeM3UList(save, item);
    }
    else if(suffix == "pls")
    {
        writePLSList(save, item);
    }
    else if(suffix == "wpl")
    {
        writeWPLList(save, item);
    }
    else if(suffix == "xspf")
    {
        writeXSPFList(save, item);
    }
}

void MusicPlayListManager::getMusicSongItems(const QStringList &open, MusicSongItems &items)
{
    foreach(const QString &path, open)
    {
        QFileInfo info(path);
        QString suffix = info.suffix().toLower();
        if(suffix == "lis")
        {
            readLisList(path, items);
        }
        else if(suffix == "m3u" || suffix == "m3u8")
        {
            readM3UList(path, items);
        }
        else if(suffix == "pls")
        {
            readPLSList(path, items);
        }
        else if(suffix == "wpl")
        {
            readWPLList(path, items);
        }
        else if(suffix == "xspf")
        {
            readXSPFList(path, items);
        }
    }
}

void MusicPlayListManager::readLisList(const QString &path, MusicSongItems &items)
{
    MusicXMLConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readMusicSongsConfig(items);
    }
}

void MusicPlayListManager::writeLisList(const QString &path, const MusicSongItem &item)
{
    MusicXMLConfigManager manager;
    manager.writeMusicSongsConfig(MusicSongItems() << item, path);
}

void MusicPlayListManager::readM3UList(const QString &path, MusicSongItems &items)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        bool state = false;
        MusicSongItem item;
        QStringList data(QString(file.readAll()).split("\n"));
        foreach(QString str, data)
        {
            str = str.trimmed();
            if(str.isEmpty())
            {
                continue;
            }
            else if(str.startsWith("#TTKM3U"))
            {
                state = true;
                continue;
            }
            else if(str.startsWith("#TTKNAME:"))
            {
                item.m_itemName = str.remove("#TTKNAME:");
            }
            else if(str.startsWith("#TTKTIT:"))
            {
                str = str.remove("#TTKTIT:");
                QStringList dds = str.split(STRING_SPLITER);
                if(dds.count() == 3)
                {
                    item.m_itemIndex = dds[0].toInt();
                    item.m_sort.m_index = dds[1].toInt();
                    item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, dds[2].toInt());
                }
            }
            else if(str.startsWith("#TTKINF:"))
            {
                str = str.remove("#TTKINF:");
                QStringList dds = str.split(STRING_SPLITER);
                if(dds.count() == 4)
                {
                    item.m_songs << MusicSong(dds[3], dds[0].toInt(), dds[2], dds[1]);
                }
            }
        }
        file.close();

        if(state)
        {
            items << item;
        }
        else
        {
            MusicPlayListManager::messageAlert();
        }
    }
    else
    {
        MusicPlayListManager::messageAlert();
    }
}

void MusicPlayListManager::writeM3UList(const QString &path, const MusicSongItem &item)
{
    QStringList data;
    data << QString("#TTKM3U");
    data << QString("#TTKNAME:%1").arg(item.m_itemName);
    data << QString("#TTKTIT:%2%1%3%1%4").arg(STRING_SPLITER).arg(item.m_itemIndex)
                                         .arg(item.m_sort.m_index).arg(item.m_sort.m_sortType);
    foreach(const MusicSong &song, item.m_songs)
    {
        data.append(QString("#TTKINF:%2%1%3%1%4%1%5").arg(STRING_SPLITER).arg(song.getMusicPlayCount())
                                                     .arg(song.getMusicName()).arg(song.getMusicTime())
                                                     .arg(song.getMusicPath()));
        data.append(song.getMusicPath());
    }

    QFile file(path);
    if(file.open(QFile::WriteOnly))
    {
        file.write(data.join("\n").toUtf8());
        file.close();
    }
}

void MusicPlayListManager::readPLSList(const QString &path, MusicSongItems &items)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        bool state = false;
        MusicSongItem item;
        QStringList data(QString(file.readAll()).split("\n"));
        if(!data.isEmpty() && data.takeFirst().toLower().contains("[playlist]"))
        {
            foreach(QString str, data)
            {
                str = str.trimmed();
                if(str.isEmpty())
                {
                    continue;
                }
                else if(str.startsWith("#TTKPLS"))
                {
                    state = true;
                    continue;
                }
                else if(str.startsWith("#TTKNAME:"))
                {
                    item.m_itemName = str.remove("#TTKNAME:");
                }
                else if(str.startsWith("#TTKTIT:"))
                {
                    str = str.remove("#TTKTIT:");
                    QStringList dds = str.split(STRING_SPLITER);
                    if(dds.count() == 3)
                    {
                        item.m_itemIndex = dds[0].toInt();
                        item.m_sort.m_index = dds[1].toInt();
                        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, dds[2].toInt());
                    }
                }
                else if(str.startsWith("#TTKINF:"))
                {
                    str = str.remove("#TTKINF:");
                    QStringList dds = str.split(STRING_SPLITER);
                    if(dds.count() == 4)
                    {
                        item.m_songs << MusicSong(dds[3], dds[0].toInt(), dds[2], dds[1]);
                    }
                }
            }
        }
        file.close();

        if(state)
        {
            items << item;
        }
        else
        {
            MusicPlayListManager::messageAlert();
        }
    }
    else
    {
        MusicPlayListManager::messageAlert();
    }
}

void MusicPlayListManager::writePLSList(const QString &path, const MusicSongItem &item)
{
    QStringList data;
    data << QString("[playlist]");
    data << QString("#TTKPLS");
    data << QString("#TTKNAME:%1").arg(item.m_itemName);
    data << QString("#TTKTIT:%2%1%3%1%4").arg(STRING_SPLITER).arg(item.m_itemIndex)
                                         .arg(item.m_sort.m_index).arg(item.m_sort.m_sortType);
    int count = 1;
    foreach(const MusicSong &song, item.m_songs)
    {
        data.append(QString("#TTKINF:%2%1%3%1%4%1%5").arg(STRING_SPLITER).arg(song.getMusicPlayCount())
                                                     .arg(song.getMusicName()).arg(song.getMusicTime())
                                                     .arg(song.getMusicPath()));
        data.append(QString("File%1=%2").arg(count).arg(song.getMusicPath()));
        data.append(QString("Title%1=%2").arg(count).arg(song.getMusicName()));
        data.append(QString("Length%1=%2").arg(count).arg(MusicTime::fromString(song.getMusicTime(), "mm:ss")
                                                          .getTimeStamp(MusicTime::All_Sec)));
        ++count;
    }
    data << "NumberOfEntries=" + QString::number(item.m_songs.count());

    QFile file(path);
    if(file.open(QFile::WriteOnly))
    {
        file.write(data.join("\n").toUtf8());
        file.close();
    }
}

void MusicPlayListManager::readWPLList(const QString &path, MusicSongItems &items)
{
    MusicWPLConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readMusicSongsConfig(items);
    }
}

void MusicPlayListManager::writeWPLList(const QString &path, const MusicSongItem &item)
{
    MusicWPLConfigManager manager;
    manager.writeMusicSongsConfig(MusicSongItems() << item, path);
}

void MusicPlayListManager::readXSPFList(const QString &path, MusicSongItems &items)
{
    MusicXSPFConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readMusicSongsConfig(items);
    }
}

void MusicPlayListManager::writeXSPFList(const QString &path, const MusicSongItem &item)
{
    MusicXSPFConfigManager manager;
    manager.writeMusicSongsConfig(MusicSongItems() << item, path);
}
