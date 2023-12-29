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
    const QString &suffix = TTK_FILE_SUFFIX(fin);

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

void MusicPlaylistManager::readSongItems(const QStringList &paths, MusicSongItemList &items)
{
    for(const QString &path : qAsConst(paths))
    {
        const QFileInfo fin(path);
        const QString & suffix = TTK_FILE_SUFFIX(fin);

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
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeTKPLConfig(const QString &path, const MusicSongItem &item)
{
    MusicTKPLConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readM3UConfig(const QString &path, MusicSongItemList &items)
{
    MusicM3UConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeM3UConfig(const QString &path, const MusicSongItem &item)
{
    MusicM3UConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readPLSConfig(const QString &path, MusicSongItemList &items)
{
    MusicPLSConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writePLSConfig(const QString &path, const MusicSongItem &item)
{
    MusicPLSConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readWPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicWPLConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeWPLConfig(const QString &path, const MusicSongItem &item)
{
    MusicWPLConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readXSPFConfig(const QString &path, MusicSongItemList &items)
{
    MusicXSPFConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeXSPFConfig(const QString &path, const MusicSongItem &item)
{
    MusicXSPFConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readJSPFConfig(const QString &path, MusicSongItemList &items)
{
    MusicJSPFConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeJSPFConfig(const QString &path, const MusicSongItem &item)
{
    MusicJSPFConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readASXConfig(const QString &path, MusicSongItemList &items)
{
    MusicASXConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeASXConfig(const QString &path, const MusicSongItem &item)
{
    MusicASXConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readFPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicFPLConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::readDBPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicDBPLConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::readCSVConfig(const QString &path, MusicSongItemList &items)
{
    MusicCSVConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeCSVConfig(const QString &path, const MusicSongItem &item)
{
    MusicCSVConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}

bool MusicPlaylistManager::readTXTConfig(const QString &path, MusicSongItemList &items)
{
    MusicTXTConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

bool MusicPlaylistManager::writeTXTConfig(const QString &path, const MusicSongItem &item)
{
    MusicTXTConfigManager manager;
    return manager.load(path) ? manager.writeBuffer({item}) : false;
}
