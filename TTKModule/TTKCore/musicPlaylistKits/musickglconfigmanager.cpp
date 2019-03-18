#include "musickglconfigmanager.h"
#include "musictime.h"

#include <QTextCodec>

MusicKGLConfigManager::MusicKGLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicKGLConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    QTextCodec *codec = QTextCodec::codecForName("windows-1252");
    const QDomNodeList &nodes = m_document->elementsByTagName("File");
    for(int i=0; i<nodes.count(); ++i)
    {
        MusicSong song;
        const QDomNodeList &cNodes = nodes.at(i).childNodes();
        for(int i=0; i<cNodes.count(); ++i)
        {
            const QDomNode &cNode = cNodes.at(i);
            if(cNode.nodeName() == "Duration")
            {
                song.setMusicPlayTime(MusicTime::msecTime2LabelJustified(cNode.toElement().text().toULongLong()));
            }
            else if(cNode.nodeName() == "FileName")
            {
                const QFileInfo info(codec->fromUnicode(cNode.toElement().text()));
                song.setMusicName(info.baseName());
                song.setMusicType(info.suffix());

                if(song.getMusicPath().isEmpty())
                {
                    song.setMusicPath(info.fileName());
                }
                else
                {
                    song.setMusicPath(song.getMusicPath() + info.fileName());
                }
            }
            else if(cNode.nodeName() == "FilePath")
            {
                const QString &path = codec->fromUnicode(cNode.toElement().text());
                if(song.getMusicName().isEmpty())
                {
                    song.setMusicPath(path);
                }
                else
                {
                    song.setMusicPath(path + song.getMusicName() + '.' + song.getMusicType());
                }
            }
            else if(cNode.nodeName() == "FileSize")
            {
                song.setMusicSize(cNode.toElement().text().toLongLong());
            }
        }
        item.m_songs << song;
    }

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
}

void MusicKGLConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    Q_UNUSED(items);
    Q_UNUSED(path);
}
