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
#include "musicjspfconfigmanager.h"

void MusicPlaylistManager::writeSongItem(const QString &path, const MusicSongItem &item)
{
    const QFileInfo fin(path);
    const QString &suffix = FILE_SUFFIX(fin);

    if(suffix == TPL_FILE_SUFFIX)
    {
        writeTKPLConfig(path, item);
    }
    else if(suffix == M3U_FILE_SUFFIX || suffix == M3U8_FILE_SUFFIX)
    {
        writeM3UConfig(path, item);
    }
    else if(suffix == PLS_FILE_SUFFIX)
    {
        writePLSConfig(path, item);
    }
    else if(suffix == WPL_FILE_SUFFIX)
    {
        writeWPLConfig(path, item);
    }
    else if(suffix == XSPF_FILE_SUFFIX)
    {
        writeXSPFConfig(path, item);
    }
    else if(suffix == JSPF_FILE_SUFFIX)
    {
        writeJSPFConfig(path, item);
    }
    else if(suffix == ASX_FILE_SUFFIX)
    {
        writeASXConfig(path, item);
    }
    else if(suffix == CSV_FILE_SUFFIX)
    {
        writeCSVConfig(path, item);
    }
    else if(suffix == TXT_FILE_SUFFIX)
    {
        writeTXTConfig(path, item);
    }
}

void MusicPlaylistManager::readSongItems(const QStringList& paths, MusicSongItemList &items)
{
    for(const QString &path : qAsConst(paths))
    {
        const QFileInfo fin(path);
        const QString & suffix = FILE_SUFFIX(fin);

        if(suffix == TPL_FILE_SUFFIX)
        {
            readTKPLConfig(path, items);
        }
        else if(suffix == M3U_FILE_SUFFIX || suffix == M3U8_FILE_SUFFIX)
        {
            readM3UConfig(path, items);
        }
        else if(suffix == PLS_FILE_SUFFIX)
        {
            readPLSConfig(path, items);
        }
        else if(suffix == WPL_FILE_SUFFIX)
        {
            readWPLConfig(path, items);
        }
        else if(suffix == XSPF_FILE_SUFFIX)
        {
            readXSPFConfig(path, items);
        }
        else if(suffix == JSPF_FILE_SUFFIX)
        {
            readJSPFConfig(path, items);
        }
        else if(suffix == ASX_FILE_SUFFIX)
        {
            readASXConfig(path, items);
        }
        else if(suffix == CSV_FILE_SUFFIX)
        {
            readCSVConfig(path, items);
        }
        else if(suffix == TXT_FILE_SUFFIX)
        {
            readTXTConfig(path, items);
        }
        else if(suffix == FPL_FILE_SUFFIX)
        {
            readFPLConfig(path, items);
        }
        else if(suffix == DBPL_FILE_SUFFIX)
        {
            readDBPLConfig(path, items);
        }
    }
}

bool MusicPlaylistManager::readTKPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicTKPLConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeTKPLConfig(const QString &path, const MusicSongItem &item)
{
    MusicTKPLConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readM3UConfig(const QString &path, MusicSongItemList &items)
{
    MusicM3UConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeM3UConfig(const QString &path, const MusicSongItem &item)
{
    MusicM3UConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readPLSConfig(const QString &path, MusicSongItemList &items)
{
    MusicPLSConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writePLSConfig(const QString &path, const MusicSongItem &item)
{
    MusicPLSConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readWPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicWPLConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeWPLConfig(const QString &path, const MusicSongItem &item)
{
    MusicWPLConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readXSPFConfig(const QString &path, MusicSongItemList &items)
{
    MusicXSPFConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeXSPFConfig(const QString &path, const MusicSongItem &item)
{
    MusicXSPFConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readJSPFConfig(const QString &path, MusicSongItemList &items)
{
    MusicJSPFConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeJSPFConfig(const QString &path, const MusicSongItem &item)
{
    MusicJSPFConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readASXConfig(const QString &path, MusicSongItemList &items)
{
    MusicASXConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeASXConfig(const QString &path, const MusicSongItem &item)
{
    MusicASXConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readFPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicFPLConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::readDBPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicDBPLConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::readCSVConfig(const QString &path, MusicSongItemList &items)
{
    MusicCSVConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeCSVConfig(const QString &path, const MusicSongItem &item)
{
    MusicCSVConfigManager manager;
    return manager.writeBuffer({item}, path);
}

bool MusicPlaylistManager::readTXTConfig(const QString &path, MusicSongItemList &items)
{
    MusicTXTConfigManager manager;
    if(manager.fromFile(path))
    {
        return manager.readBuffer(items);
    }
    return false;
}

bool MusicPlaylistManager::writeTXTConfig(const QString &path, const MusicSongItem &item)
{
    MusicTXTConfigManager manager;
    return manager.writeBuffer({item}, path);
}
