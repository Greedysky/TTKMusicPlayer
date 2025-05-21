#include "musicdbplconfigmanager.h"

static constexpr int PLAYLIST_MAJOR_VER = 1;
static constexpr int PLAYLIST_MINOR_VER = 1;

MusicDBPLConfigManager::MusicDBPLConfigManager()
    : MusicPlaylistRenderer()
    , MusicPlaylistInterface()
{

}

bool MusicDBPLConfigManager::readBuffer(MusicSongItemList &items)
{
    const QFileInfo fin(m_file.fileName());
    MusicSongItem item;
    item.m_itemName = fin.baseName();

    // read 4-byte signature
    char magic[5] = {0};
    m_file.read(magic, 4);
    if(strncmp(magic, "DBPL", 4))
    {
        return false;
    }

    TTKUInt8 majorver = 0;
    m_file.read((char*)&majorver, 1);
    if(majorver != PLAYLIST_MAJOR_VER)
    {
        return false;
    }

    TTKUInt8 minorver = 0;
    m_file.read((char*)&minorver, 1);
    if(minorver < PLAYLIST_MINOR_VER)
    {
        return false;
    }

    TTKUInt32 cnt = 0;
    m_file.read((char*)&cnt, 4);
    for(TTKUInt32 i = 0; i < cnt; ++i)
    {
        TTKUInt32 v = 0;
        QString filePath;

        if(minorver <= 2)
        {
            // fname
            if(m_file.read((char*)&v, 2) != 2)
            {
                return false;
            }

            char uri[v + 1];
            memset(uri, 0, sizeof(char) * (v + 1));

            if(m_file.read(uri, v) != v)
            {
                return false;
            }

            filePath = uri;
            // decoder
            if(m_file.read((char*)&v, 1)  != 1)
            {
                return false;
            }

            if(v >= 20)
            {
                return false;
            }

            char decoder[20] = {0};
            if(v && m_file.read((char*)&decoder, v) != v)
            {
                return false;
            }
            // tracknum
            if(m_file.read((char*)&v, 2) != 2)
            {
                return false;
            }
        }

        if(m_file.read((char*)&v, 4) != 4)
        {
            return false;
        }

        if(m_file.read((char*)&v, 4) != 4)
        {
            return false;
        }

        float duration = 0;
        if(m_file.read((char*)&duration, 4) != 4)
        {
            return false;
        }

        item.m_songs << MusicSong(filePath, TTKTime::formatDuration(TTKStaticCast(int, duration) * TTK_DN_S2MS));
        if(minorver <= 2)
        {
            // legacy filetype support
            if(m_file.read((char*)&v, 1) != 1)
            {
                return false;
            }

            if(v)
            {
                char ftype[v + 1];
                memset(ftype, 0, sizeof(char) * (v + 1));

                if(m_file.read(ftype, v) != v)
                {
                    return false;
                }
            }

            if(m_file.read((char*)&v, 4) != 4)
            {
                return false;
            }

            if(m_file.read((char*)&v, 4) != 4)
            {
                return false;
            }

            if(m_file.read((char*)&v, 4) != 4)
            {
                return false;
            }

            if(m_file.read((char*)&v, 4) != 4)
            {
                return false;
            }
        }

        if(minorver >= 2 && m_file.read((char*)&v, 4) != 4)
        {
            return false;
        }

        TTKUInt16 nm = 0;
        if(m_file.read((char*)&nm, 2) != 2)
        {
            return false;
        }

        for(int j = 0; j < nm; ++j)
        {
            if(m_file.read((char*)&v, 2) != 2)
            {
                return false;
            }

            if(v >= 20000)
            {
                return false;
            }

            char key[v + 1];
            memset(key, 0, sizeof(char) * (v + 1));

            if(m_file.read(key, v) != v)
            {
                return false;
            }

            if(m_file.read((char*)&v, 2) != 2)
            {
                return false;
            }

            if(v >= 20000)
            {
                continue;
            }

            char value[v + 1];
            memset(value, 0, sizeof(char) * (v + 1));

            if(m_file.read((char*)&value, v) != v)
            {
                return false;
            }
        }
    }

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicDBPLConfigManager::writeBuffer(const MusicSongItemList &items)
{
    Q_UNUSED(items);
    return false;
}
