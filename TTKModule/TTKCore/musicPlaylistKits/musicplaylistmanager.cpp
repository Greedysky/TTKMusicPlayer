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

static bool readTKPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicTKPLConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeTKPLConfig(const QString &path, const MusicSongItemList &item)
{
    MusicTKPLConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readM3UConfig(const QString &path, MusicSongItemList &items)
{
    MusicM3UConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeM3UConfig(const QString &path, const MusicSongItemList &item)
{
    MusicM3UConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readPLSConfig(const QString &path, MusicSongItemList &items)
{
    MusicPLSConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writePLSConfig(const QString &path, const MusicSongItemList &item)
{
    MusicPLSConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readWPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicWPLConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeWPLConfig(const QString &path, const MusicSongItemList &item)
{
    MusicWPLConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readXSPFConfig(const QString &path, MusicSongItemList &items)
{
    MusicXSPFConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeXSPFConfig(const QString &path, const MusicSongItemList &item)
{
    MusicXSPFConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readJSPFConfig(const QString &path, MusicSongItemList &items)
{
    MusicJSPFConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeJSPFConfig(const QString &path, const MusicSongItemList &item)
{
    MusicJSPFConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readASXConfig(const QString &path, MusicSongItemList &items)
{
    MusicASXConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeASXConfig(const QString &path, const MusicSongItemList &item)
{
    MusicASXConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readFPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicFPLConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool readDBPLConfig(const QString &path, MusicSongItemList &items)
{
    MusicDBPLConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool readCSVConfig(const QString &path, MusicSongItemList &items)
{
    MusicCSVConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeCSVConfig(const QString &path, const MusicSongItemList &item)
{
    MusicCSVConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}

static bool readTXTConfig(const QString &path, MusicSongItemList &items)
{
    MusicTXTConfigManager manager;
    return manager.fromFile(path) ? manager.readBuffer(items) : false;
}

static bool writeTXTConfig(const QString &path, const MusicSongItemList &item)
{
    MusicTXTConfigManager manager;
    return manager.load(path) ? manager.writeBuffer(item) : false;
}


void MusicPlaylistManager::writeSongItem(const QString &path, const MusicSongItemList &item)
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
