#include "musicfplconfigmanager.h"
#include "musiccodecutils.h"

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wunused-result"
#endif

typedef struct FPLTrackChunk{
    uint unk1;		// not sure??
    uint file_ofz;	// filename string offset
    uint subsong;	// subsong index value
    uint fsize;		// filesize
    uint unk2;		// ??
    uint unk3;		// ??
    uint unk4;		// ??
    char duration_dbl[8]; // track duration data (converted later)
    float rpg_album;	// replay gain, album
    float rpg_track;	// replay gain, track
    float rpk_album;	// replay gain, album peak
    float rpk_track;    // replay gain, track peak
    uint  keys_dex;	    // number of key/pointers that follow
    uint  key_primary;  // number of primary info keys
    uint  key_second;   // number of secondary info key combos
    uint  key_sec_offset; // index of secondary key start
} FPLTrackChunk;


MusicFPLConfigManager::MusicFPLConfigManager()
    : MusicPlaylistReader()
    , MusicPlaylistInterface()
{

}

bool MusicFPLConfigManager::readPlaylistData(MusicSongItems &items)
{
    QFileInfo info(m_file.fileName());
    MusicSongItem item;
    item.m_itemName = info.baseName();

    // read 16-byte signature
    char magic[16];
    m_file.read(magic, 16);

    // load primary data string into memory
    uint dataSize;
    m_file.read((char*)&dataSize, 4);

    char *dataPrime = new char[dataSize];
    // read in primary string to memory
    m_file.read(dataPrime, dataSize);

    // read playlist count integer
    uint plSize;
    m_file.read((char*)&plSize, 4);

    uint keyRunner[512];
    double duration = 0.0f;

    FPLTrackChunk chunkRunner;
    for(size_t i=0; i<plSize && !m_file.atEnd(); ++i)
    {
        m_file.read((char*)&chunkRunner, sizeof(FPLTrackChunk));
        // keys_dex sanity check
        if(chunkRunner.keys_dex > 512)
        {
            delete[] dataPrime;
            m_file.close();
            return false;
        }

        m_file.read((char*)&keyRunner, sizeof(uint) * (chunkRunner.keys_dex - 3));
        memcpy((void*)&duration, chunkRunner.duration_dbl, 8);

        QString path = dataPrime + chunkRunner.file_ofz;
        path.remove("file://");
        path = info.absolutePath() + "/" + path;
        item.m_songs << MusicSong(path, 0, MusicTime::msecTime2LabelJustified(duration * MT_S2MS), QString());
    }

    delete[] dataPrime;
    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicFPLConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    Q_UNUSED(items);
    Q_UNUSED(path);
    return false;
}
