#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"

#include "musicdownloadqueryttthread.h"
#include "musicdownloadquerywythread.h"
#include "musicdownloadqueryxmthread.h"
#include "musicdownloadquerykgthread.h"
#include "musicdownloadquerykwthread.h"
#include "musicdownloadquerybdthread.h"
#include "musicdownloadqueryqqthread.h"

#include "musicdownloadqueryttalbumthread.h"
#include "musicdownloadquerywyalbumthread.h"
#include "musicdownloadqueryxmalbumthread.h"
#include "musicdownloadquerykgalbumthread.h"
#include "musicdownloadquerykwalbumthread.h"
#include "musicdownloadquerybdalbumthread.h"
#include "musicdownloadqueryqqalbumthread.h"

#include "musicdownloadqueryttartistthread.h"
#include "musicdownloadquerywyartistthread.h"
#include "musicdownloadqueryxmartistthread.h"
#include "musicdownloadquerykgartistthread.h"
#include "musicdownloadquerykwartistthread.h"
#include "musicdownloadquerybdartistthread.h"
#include "musicdownloadqueryqqartistthread.h"

#include "musicttdatadownloadthread.h"
#include "musicdatadownloadthread.h"

#include "musictextdownloadthread.h"
#include "musictttextdownloadthread.h"
#include "musicwytextdownloadthread.h"
#include "musicxmtextdownloadthread.h"
#include "musickwtextdownloadthread.h"
#include "musicqqtextdownloadthread.h"

QString MusicDownLoadQueryFactory::getClassName()
{
    return "MusicDownLoadQueryFactory";
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getQueryThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicDownLoadQueryWYThread(parent); break;
        case 1:  thread = new MusicDownLoadQueryQQThread(parent); break;
        case 2:  thread = new MusicDownLoadQueryXMThread(parent); break;
        case 3:  thread = new MusicDownLoadQueryTTThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryBDThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKWThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryKGThread(parent); break;
        default: thread = new MusicDownLoadQueryWYThread(parent);
    }
    M_LOGGER_INFO(QString("getQueryThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getAlbumThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicDownLoadQueryWYAlbumThread(parent); break;
        case 1:  thread = new MusicDownLoadQueryQQAlbumThread(parent); break;
        case 2:  thread = new MusicDownLoadQueryXMAlbumThread(parent); break;
        case 3:  thread = new MusicDownLoadQueryTTAlbumThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryBDAlbumThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKWAlbumThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryKGAlbumThread(parent); break;
        default: thread = new MusicDownLoadQueryWYAlbumThread(parent);
    }
    M_LOGGER_INFO(QString("getAlbumThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getArtistThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicDownLoadQueryWYArtistThread(parent); break;
        case 1:  thread = new MusicDownLoadQueryQQArtistThread(parent); break;
        case 2:  thread = new MusicDownLoadQueryXMArtistThread(parent); break;
        case 3:  thread = new MusicDownLoadQueryTTArtistThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryBDArtistThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKWArtistThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryKGArtistThread(parent); break;
        default: thread = new MusicDownLoadQueryWYArtistThread(parent);
    }
    M_LOGGER_INFO(QString("getArtistThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadSmallPic(const QString &url, const QString &save,
                                                                            MusicDownLoadThreadAbstract::Download_Type type,
                                                                            QObject *parent)
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicDataDownloadThread(url, save, type, parent));
        case 1: return (new MusicDataDownloadThread(url, save, type, parent));
        case 2: return (new MusicDataDownloadThread(url, save, type, parent));
        case 3: return (new MusicDataDownloadThread(url, save, type, parent));
        case 4: return (new MusicDataDownloadThread(url, save, type, parent));
        case 5: return (new MusicDataDownloadThread(url, save, type, parent));
        case 6: return (new MusicDataDownloadThread(url, save, type, parent));
    }
    return (new MusicDataDownloadThread(url, save, type, parent));
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadLrc(const QString &url, const QString &save,
                                                                       MusicDownLoadThreadAbstract::Download_Type type,
                                                                       QObject *parent)
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicWYTextDownLoadThread(url, save, type, parent));
        case 1: return (new MusicQQTextDownLoadThread(url, save, type, parent));
        case 2: return (new MusicXMTextDownLoadThread(url, save, type, parent));
        case 3: return (new MusicTTTextDownLoadThread(url, save, type, parent));
        case 4: return (new MusicTextDownLoadThread(url, save, type, parent));
        case 5: return (new MusicKWTextDownLoadThread(url, save, type, parent));
        case 6: return (new MusicTextDownLoadThread(url, save, type, parent));
    }
    return (new MusicTextDownLoadThread(url, save, type, parent));
}
