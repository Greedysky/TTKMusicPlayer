#include "musicplaylistmanager.h"
#include "musicasxconfigmanager.h"
#include "musicm3uconfigmanager.h"
#include "musicplsconfigmanager.h"
#include "musicwplconfigmanager.h"
#include "musicxspfconfigmanager.h"
#include "musictkplconfigmanager.h"
#include "musicdbplconfigmanager.h"
#include "musicfplconfigmanager.h"
#include "musiccsvconfigmanager.h"
#include "musictxtconfigmanager.h"

void MusicPlaylistManager::setMusicSongItem(const QString &path, const MusicSongItem &item)
{
    const QFileInfo info(path);
    const QString &suffix = info.suffix().toLower();

    if(suffix == LST_FILE_PREFIX)
    {
        writeTKPLConfig(path, item);
    }
    else if(suffix == M3U_FILE_PREFIX || suffix == M3U8_FILE_PREFIX)
    {
        writeM3UConfig(path, item);
    }
    else if(suffix == PLS_FILE_PREFIX)
    {
        writePLSConfig(path, item);
    }
    else if(suffix == WPL_FILE_PREFIX)
    {
        writeWPLConfig(path, item);
    }
    else if(suffix == XSPF_FILE_PREFIX)
    {
        writeXSPFConfig(path, item);
    }
    else if(suffix == ASX_FILE_PREFIX)
    {
        writeASXConfig(path, item);
    }
    else if(suffix == CSV_FILE_PREFIX)
    {
        writeCSVConfig(path, item);
    }
    else if(suffix == TXT_FILE_PREFIX)
    {
        writeTXTConfig(path, item);
    }
}

void MusicPlaylistManager::getMusicSongItems(const QStringList& paths, MusicSongItems &items)
{
    for(const QString &path : qAsConst(paths))
    {
        const QFileInfo info(path);
        const QString & suffix = info.suffix().toLower();

        if(suffix == LST_FILE_PREFIX)
        {
            readTKPLConfig(path, items);
        }
        else if(suffix == M3U_FILE_PREFIX || suffix == M3U8_FILE_PREFIX)
        {
            readM3UConfig(path, items);
        }
        else if(suffix == PLS_FILE_PREFIX)
        {
            readPLSConfig(path, items);
        }
        else if(suffix == WPL_FILE_PREFIX)
        {
            readWPLConfig(path, items);
        }
        else if(suffix == XSPF_FILE_PREFIX)
        {
            readXSPFConfig(path, items);
        }
        else if(suffix == ASX_FILE_PREFIX)
        {
            readASXConfig(path, items);
        }
        else if(suffix == CSV_FILE_PREFIX)
        {
            readCSVConfig(path, items);
        }
        else if(suffix == TXT_FILE_PREFIX)
        {
            readTXTConfig(path, items);
        }
        else if(suffix == FPL_FILE_PREFIX)
        {
            readFPLConfig(path, items);
        }
        else if(suffix == DBPL_FILE_PREFIX)
        {
            readDBPLConfig(path, items);
        }
    }
}

bool MusicPlaylistManager::readTKPLConfig(const QString &path, MusicSongItems &items)
{
    MusicTKPLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeTKPLConfig(const QString &path, const MusicSongItem &item)
{
    MusicTKPLConfigManager manager;
    return manager.writePlaylistData({item}, path);
}

bool MusicPlaylistManager::readM3UConfig(const QString &path, MusicSongItems &items)
{
    MusicM3UConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeM3UConfig(const QString &path, const MusicSongItem &item)
{
    MusicM3UConfigManager manager;
    return manager.writePlaylistData({item}, path);
}

bool MusicPlaylistManager::readPLSConfig(const QString &path, MusicSongItems &items)
{
    MusicPLSConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writePLSConfig(const QString &path, const MusicSongItem &item)
{
    MusicPLSConfigManager manager;
    return manager.writePlaylistData({item}, path);
}

bool MusicPlaylistManager::readWPLConfig(const QString &path, MusicSongItems &items)
{
    MusicWPLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeWPLConfig(const QString &path, const MusicSongItem &item)
{
    MusicWPLConfigManager manager;
    return manager.writePlaylistData({item}, path);
}

bool MusicPlaylistManager::readXSPFConfig(const QString &path, MusicSongItems &items)
{
    MusicXSPFConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeXSPFConfig(const QString &path, const MusicSongItem &item)
{
    MusicXSPFConfigManager manager;
    return manager.writePlaylistData({item}, path);
}

bool MusicPlaylistManager::readASXConfig(const QString &path, MusicSongItems &items)
{
    MusicASXConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeASXConfig(const QString &path, const MusicSongItem &item)
{
    MusicASXConfigManager manager;
    return manager.writePlaylistData({item}, path);
}

bool MusicPlaylistManager::readFPLConfig(const QString &path, MusicSongItems &items)
{
    MusicFPLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::readDBPLConfig(const QString &path, MusicSongItems &items)
{
    MusicDBPLConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::readCSVConfig(const QString &path, MusicSongItems &items)
{
    MusicCSVConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeCSVConfig(const QString &path, const MusicSongItem &item)
{
    MusicCSVConfigManager manager;
    return manager.writePlaylistData({item}, path);
}

bool MusicPlaylistManager::readTXTConfig(const QString &path, MusicSongItems &items)
{
    MusicTXTConfigManager manager;
    if(manager.readConfig(path))
    {
        return manager.readPlaylistData(items);
    }
    return false;
}

bool MusicPlaylistManager::writeTXTConfig(const QString &path, const MusicSongItem &item)
{
    MusicTXTConfigManager manager;
    return manager.writePlaylistData({item}, path);
}
