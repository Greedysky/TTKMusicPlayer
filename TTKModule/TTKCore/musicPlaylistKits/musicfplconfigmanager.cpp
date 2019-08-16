#include "musicfplconfigmanager.h"
#include "musiccodecutils.h"
#include "musictime.h"

MusicFPLConfigManager::MusicFPLConfigManager()
    : MusicPlaylistInterface()
{

}

bool MusicFPLConfigManager::readConfig(const QString &name)
{
    m_fileName = name;
    return true;
}

void MusicFPLConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_fileName).baseName();

    FILE *fp = nullptr;
    if((fp = fopen(MusicUtils::Codec::toLocal8Bit(m_fileName), "rb")) == nullptr)
    {
        return;
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

    for(size_t i=0; i<plSize && !feof(fp); ++i)
    {
        fread((void*)&m_chunkrunner, sizeof(FPLTrackChunk), 1, fp);
        // keys_dex sanity check
        if(m_chunkrunner.keys_dex > 512)
        {
            return;
        }

        fread((void*)&keyRunner, sizeof(uint), m_chunkrunner.keys_dex - 3, fp);
        memcpy((void*)&duration, m_chunkrunner.duration_dbl, 8);
        item.m_songs << MusicSong(dataPrime + m_chunkrunner.file_ofz, 0, MusicTime::msecTime2LabelJustified(duration * MT_S2MS), QString());
    }

    free(dataPrime);
    fclose(fp);

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
}

void MusicFPLConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    Q_UNUSED(items);
    Q_UNUSED(path);
}
