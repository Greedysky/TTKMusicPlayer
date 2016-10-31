#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"

#include "musicdownloadquerymultiplethread.h"
#include "musicdownloadquerymultiplevipthread.h"
#include "musicdownloadqueryttthread.h"
#include "musicdownloadquerywythread.h"

#include "musicdownloadqueryalbumthread.h"
#include "musicdownloadqueryalbumvipthread.h"
#include "musicdownloadqueryalbumttthread.h"
#include "musicdownloadqueryalbumwythread.h"

#include "musicdownloadqueryartistthread.h"
#include "musicdownloadqueryartistvipthread.h"

#include "musicttdatadownloadthread.h"
#include "musicdatadownloadthread.h"

#include "musictextdownloadthread.h"
#include "musictttextdownloadthread.h"
#include "musicwytextdownloadthread.h"

QString MusicDownLoadQueryFactory::getClassName()
{
    return "MusicDownLoadQueryFactory";
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getQueryThread(QObject *parent)
{
    int type = M_SETTING_PTR->value(MusicSettingManager::DownloadServerMultipleChoiced).toInt();
    if(type == 0)
    {
        int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
        switch( index )
        {
            case 0: return (new MusicDownLoadQueryWYThread(parent));
            case 4: return (new MusicDownLoadQueryTTThread(parent));
        }
    }
    else if(type == 2)
    {
         return (new MusicDownLoadQueryMultipleVipThread(parent));
    }

    return (new MusicDownLoadQueryMultipleThread(parent));
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getAlbumThread(QObject *parent)
{
    int type = M_SETTING_PTR->value(MusicSettingManager::DownloadServerMultipleChoiced).toInt();
    if(type == 0)
    {
        int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
        switch( index )
        {
            case 0: return (new MusicDownLoadQueryAlbumWYThread(parent));
            case 4: return (new MusicDownLoadQueryAlbumTTThread(parent));
        }
    }
    else if(type == 2)
    {
        return (new MusicDownLoadQueryAlbumVipThread(parent));
    }

    return (new MusicDownLoadQueryAlbumThread(parent));
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getArtistThread(QObject *parent)
{
    int type = M_SETTING_PTR->value(MusicSettingManager::DownloadServerMultipleChoiced).toInt();
    if(type == 2)
    {
        return (new MusicDownLoadQueryArtistVipThread(parent));
    }

    return (new MusicDownLoadQueryArtistThread(parent));
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadSmallPic(const QString &url, const QString &save,
                                                                            MusicDownLoadThreadAbstract::Download_Type type,
                                                                            QObject *parent)
{
    if(M_SETTING_PTR->value(MusicSettingManager::DownloadServerMultipleChoiced).toInt() == 0)
    {
        int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
        switch( index )
        {
            case 0: return (new MusicDataDownloadThread(url, save, type, parent));
            case 4: return (new MusicDataDownloadThread(url, save, type, parent));
        }
    }
    return (new MusicDataDownloadThread(url, save, type, parent));
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadLrc(const QString &url, const QString &save,
                                                                       MusicDownLoadThreadAbstract::Download_Type type,
                                                                       QObject *parent)
{
    if(M_SETTING_PTR->value(MusicSettingManager::DownloadServerMultipleChoiced).toInt() == 0)
    {
        int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
        switch( index )
        {
            case 0: return (new MusicWYTextDownLoadThread(url, save, type, parent));
            case 4: return (new MusicTTTextDownLoadThread(url, save, type, parent));
        }
    }
    return (new MusicTextDownLoadThread(url, save, type, parent));
}
