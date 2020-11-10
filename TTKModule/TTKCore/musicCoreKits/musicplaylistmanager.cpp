#include "musicplaylistmanager.h"
#include "musicasxconfigmanager.h"
#include "musickglconfigmanager.h"
#include "musickwlconfigmanager.h"
#include "musicm3uconfigmanager.h"
#include "musicplsconfigmanager.h"
#include "musicwplconfigmanager.h"
#include "musicxspfconfigmanager.h"
#include "musictkplconfigmanager.h"
#include "musicfplconfigmanager.h"
#include "musiccsvconfigmanager.h"
#include "musictxtconfigmanager.h"
#include "musicnfnconfigmanager.h"

void MusicPlaylistManager::setMusicSongItem(const QString &path, const MusicSongItem& item)
{
    const QFileInfo info(path);
    const QString &suffix = info.suffix().toLower();

    if(suffix == LST_FILE_PREFIX)
    {
        writeTKPLList(path, item);
    }
    else if(suffix == M3U_FILE_PREFIX || suffix == M3U8_FILE_PREFIX)
    {
        writeM3UList(path, item);
    }
    else if(suffix == PLS_FILE_PREFIX)
    {
        writePLSList(path, item);
    }
    else if(suffix == WPL_FILE_PREFIX)
    {
        writeWPLList(path, item);
    }
    else if(suffix == XSPF_FILE_PREFIX)
    {
        writeXSPFList(path, item);
    }
    else if(suffix == ASX_FILE_PREFIX)
    {
        writeASXList(path, item);
    }
    else if(suffix == CSV_FILE_PREFIX)
    {
        writeCSVList(path, item);
    }
    else if(suffix == TXT_FILE_PREFIX)
    {
        writeTXTList(path, item);
    }
    else if(suffix == NFN_FILE_PREFIX)
    {
        writeNFNList(path, item);
    }
}

void MusicPlaylistManager::getMusicSongItems(const QStringList& paths, MusicSongItems& items)
{
    for(const QString &path : qAsConst(paths))
    {
        const QFileInfo info(path);
        const QString & suffix = info.suffix().toLower();

        if(suffix == LST_FILE_PREFIX)
        {
            readTKPLList(path, items);
        }
        else if(suffix == M3U_FILE_PREFIX || suffix == M3U8_FILE_PREFIX)
        {
            readM3UList(path, items);
        }
        else if(suffix == PLS_FILE_PREFIX)
        {
            readPLSList(path, items);
        }
        else if(suffix == WPL_FILE_PREFIX)
        {
            readWPLList(path, items);
        }
        else if(suffix == XSPF_FILE_PREFIX)
        {
            readXSPFList(path, items);
        }
        else if(suffix == ASX_FILE_PREFIX)
        {
            readASXList(path, items);
        }
        else if(suffix == KWL_FILE_PREFIX)
        {
            readKWLList(path, items);
        }
        else if(suffix == KGL_FILE_PREFIX)
        {
            readKGLList(path, items);
        }
        else if(suffix == FPL_FILE_PREFIX)
        {
            readFPLList(path, items);
        }
        else if(suffix == CSV_FILE_PREFIX)
        {
            readCSVList(path, items);
        }
        else if(suffix == TXT_FILE_PREFIX)
        {
            readTXTList(path, items);
        }
    }
}

bool MusicPlaylistManager::readTKPLList(const QString &path, MusicSongItems& items)
{
    MusicTKPLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeTKPLList(const QString &path, const MusicSongItem& item)
{
    MusicTKPLConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::readM3UList(const QString &path, MusicSongItems& items)
{
    MusicM3UConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeM3UList(const QString &path, const MusicSongItem& item)
{
    MusicM3UConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::readPLSList(const QString &path, MusicSongItems& items)
{
    MusicPLSConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writePLSList(const QString &path, const MusicSongItem& item)
{
    MusicPLSConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::readWPLList(const QString &path, MusicSongItems& items)
{
    MusicWPLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeWPLList(const QString &path, const MusicSongItem& item)
{
    MusicWPLConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::readXSPFList(const QString &path, MusicSongItems& items)
{
    MusicXSPFConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeXSPFList(const QString &path, const MusicSongItem& item)
{
    MusicXSPFConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::readASXList(const QString &path, MusicSongItems& items)
{
    MusicASXConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeASXList(const QString &path, const MusicSongItem& item)
{
    MusicASXConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::readKWLList(const QString &path, MusicSongItems& items)
{
    MusicKWLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::readKGLList(const QString &path, MusicSongItems& items)
{
    MusicKGLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::readFPLList(const QString &path, MusicSongItems& items)
{
    MusicFPLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::readCSVList(const QString &path, MusicSongItems &items)
{
    MusicCSVConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeCSVList(const QString &path, const MusicSongItem &item)
{
    MusicCSVConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::readTXTList(const QString &path, MusicSongItems &items)
{
    MusicTXTConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeTXTList(const QString &path, const MusicSongItem &item)
{
    MusicTXTConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}

bool MusicPlaylistManager::writeNFNList(const QString &path, const MusicSongItem &item)
{
    MusicNFNConfigManager manager;
    return manager.writePlaylistData(MusicSongItems() << item, path);
}
