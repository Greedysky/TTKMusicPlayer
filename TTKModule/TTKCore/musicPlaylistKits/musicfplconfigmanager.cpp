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
    : MusicPlaylistInterface()
{

}

bool MusicFPLConfigManager::readConfig(const QString &name)
{
    m_fileName = name;
    return true;
}

bool MusicFPLConfigManager::readPlaylistData(MusicSongItems &items)
{
    QFileInfo info(m_fileName);
    MusicSongItem item;
    item.m_itemName = info.baseName();

    FILE *fp = nullptr;
    if((fp = fopen(qPrintable(m_fileName), "rb")) == nullptr)
    {
        return false;
    }

    // read 16-byte signature
    char magic[16];
    fread(magic, 16, 1, fp);

    // load primary data string into memory
    uint dataSize;
    fread(&dataSize, 4, 1, fp);

    char *dataPrime = (char*)malloc(dataSize);
    // read in primary string to memory
    fread(dataPrime, dataSize, 1, fp);

    // read playlist count integer
    uint plSize;
    fread(&plSize, 4, 1, fp);

    uint keyRunner[512];
    double duration = 0.0f;

    FPLTrackChunk chunkRunner;
    for(size_t i=0; i<plSize && !feof(fp); ++i)
    {
        fread((void*)&chunkRunner, sizeof(FPLTrackChunk), 1, fp);
        // keys_dex sanity check
        if(chunkRunner.keys_dex > 512)
        {
            free(dataPrime);
            fclose(fp);
            return false;
        }

        fread((void*)&keyRunner, sizeof(uint), chunkRunner.keys_dex - 3, fp);
        memcpy((void*)&duration, chunkRunner.duration_dbl, 8);

        QString path = dataPrime + chunkRunner.file_ofz;
        path.remove("file:/");
        path = info.absolutePath() + path;
        item.m_songs << MusicSong(path, 0, MusicTime::msecTime2LabelJustified(duration * MT_S2MS), QString());
    }

    free(dataPrime);
    fclose(fp);

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
