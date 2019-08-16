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
#include "musicmessagebox.h"

void MusicPlaylistManager::setMusicSongItem(const QString &path, const MusicSongItem& item)
{
    const QFileInfo info(path);
    const QString &suffix = info.suffix().toLower();

    if(suffix == LST_FILE_PREFIX)
    {
        writeLisList(path, item);
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
}

void MusicPlaylistManager::getMusicSongItems(const QStringList& paths, MusicSongItems& items)
{
    foreach(const QString &path, paths)
    {
        const QFileInfo info(path);
        const QString & suffix = info.suffix().toLower();

        if(suffix == LST_FILE_PREFIX)
        {
            readLisList(path, items);
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
    }
}

void MusicPlaylistManager::readLisList(const QString &path, MusicSongItems& items)
{
    MusicTKPLConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::writeLisList(const QString &path, const MusicSongItem& item)
{
    MusicTKPLConfigManager manager;
    manager.writePlaylistData(MusicSongItems() << item, path);
}

void MusicPlaylistManager::readM3UList(const QString &path, MusicSongItems& items)
{
    MusicM3UConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::writeM3UList(const QString &path, const MusicSongItem& item)
{
    MusicM3UConfigManager manager;
    manager.writePlaylistData(MusicSongItems() << item, path);
}

void MusicPlaylistManager::readPLSList(const QString &path, MusicSongItems& items)
{
    MusicPLSConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::writePLSList(const QString &path, const MusicSongItem& item)
{
    MusicPLSConfigManager manager;
    manager.writePlaylistData(MusicSongItems() << item, path);
}

void MusicPlaylistManager::readWPLList(const QString &path, MusicSongItems& items)
{
    MusicWPLConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::writeWPLList(const QString &path, const MusicSongItem& item)
{
    MusicWPLConfigManager manager;
    manager.writePlaylistData(MusicSongItems() << item, path);
}

void MusicPlaylistManager::readXSPFList(const QString &path, MusicSongItems& items)
{
    MusicXSPFConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::writeXSPFList(const QString &path, const MusicSongItem& item)
{
    MusicXSPFConfigManager manager;
    manager.writePlaylistData(MusicSongItems() << item, path);
}

void MusicPlaylistManager::readASXList(const QString &path, MusicSongItems& items)
{
    MusicASXConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::writeASXList(const QString &path, const MusicSongItem& item)
{
    MusicASXConfigManager manager;
    manager.writePlaylistData(MusicSongItems() << item, path);
}

void MusicPlaylistManager::readKWLList(const QString &path, MusicSongItems& items)
{
    MusicKWLConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::readKGLList(const QString &path, MusicSongItems& items)
{
    MusicKGLConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}

void MusicPlaylistManager::readFPLList(const QString &path, MusicSongItems& items)
{
    MusicFPLConfigManager manager;
    if(manager.readConfig(path))
    {
        manager.readPlaylistData(items);
    }
}
