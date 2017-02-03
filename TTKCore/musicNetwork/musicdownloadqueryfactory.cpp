#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"

#include "musicdownloadqueryttthread.h"
#include "musicdownloadquerywythread.h"
#include "musicdownloadqueryxmthread.h"
#include "musicdownloadquerykgthread.h"
#include "musicdownloadquerykwthread.h"

#include "musicdownloadqueryttalbumthread.h"
#include "musicdownloadquerywyalbumthread.h"
#include "musicdownloadqueryxmalbumthread.h"
#include "musicdownloadquerykgalbumthread.h"
#include "musicdownloadquerykwalbumthread.h"

#include "musicdownloadqueryttartistthread.h"
#include "musicdownloadquerywyartistthread.h"
#include "musicdownloadqueryxmartistthread.h"
#include "musicdownloadquerykgartistthread.h"
#include "musicdownloadquerykwartistthread.h"

#include "musicttdatadownloadthread.h"
#include "musicdatadownloadthread.h"

#include "musictextdownloadthread.h"
#include "musictttextdownloadthread.h"
#include "musicwytextdownloadthread.h"
#include "musicxmtextdownloadthread.h"
#include "musickwtextdownloadthread.h"

QString MusicDownLoadQueryFactory::getClassName()
{
    return "MusicDownLoadQueryFactory";
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getQueryThread(QObject *parent)
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicDownLoadQueryWYThread(parent));
        case 2: return (new MusicDownLoadQueryXMThread(parent));
        case 3: return (new MusicDownLoadQueryTTThread(parent));
        case 5: return (new MusicDownLoadQueryKWThread(parent));
        case 6: return (new MusicDownLoadQueryKGThread(parent));
    }
    return (new MusicDownLoadQueryWYThread(parent));
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getAlbumThread(QObject *parent)
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicDownLoadQueryWYAlbumThread(parent));
        case 2: return (new MusicDownLoadQueryXMAlbumThread(parent));
        case 3: return (new MusicDownLoadQueryTTAlbumThread(parent));
        case 5: return (new MusicDownLoadQueryKWAlbumThread(parent));
        case 6: return (new MusicDownLoadQueryKGAlbumThread(parent));
    }
    return (new MusicDownLoadQueryWYAlbumThread(parent));
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getArtistThread(QObject *parent)
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicDownLoadQueryWYArtistThread(parent));
        case 2: return (new MusicDownLoadQueryXMArtistThread(parent));
        case 3: return (new MusicDownLoadQueryTTArtistThread(parent));
        case 5: return (new MusicDownLoadQueryKWArtistThread(parent));
        case 6: return (new MusicDownLoadQueryKGArtistThread(parent));
    }
    return (new MusicDownLoadQueryWYArtistThread(parent));
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadSmallPic(const QString &url, const QString &save,
                                                                            MusicDownLoadThreadAbstract::Download_Type type,
                                                                            QObject *parent)
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicDataDownloadThread(url, save, type, parent));
        case 2: return (new MusicDataDownloadThread(url, save, type, parent));
        case 3: return (new MusicDataDownloadThread(url, save, type, parent));
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
        case 2: return (new MusicXMTextDownLoadThread(url, save, type, parent));
        case 3: return (new MusicTTTextDownLoadThread(url, save, type, parent));
        case 5: return (new MusicKWTextDownLoadThread(url, save, type, parent));
        case 6: return (new MusicTextDownLoadThread(url, save, type, parent));
    }
    return (new MusicTextDownLoadThread(url, save, type, parent));
}
