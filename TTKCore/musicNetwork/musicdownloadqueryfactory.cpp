#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"

#include "musicdownloadquerymultiplethread.h"
#include "musicdownloadqueryttthread.h"
#include "musicdownloadquerywythread.h"

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
    MusicDownLoadQueryThreadAbstract *downLoadManager = nullptr;
#ifndef USE_MULTIPLE_QUERY
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: downLoadManager = new MusicDownLoadQueryWYThread(parent); break;
        case 4: downLoadManager = new MusicDownLoadQueryTTThread(parent); break;
    }
#else
    downLoadManager = new MusicDownLoadQueryMultipleThread(parent);
#endif
    return downLoadManager;
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadSmallPic(const QString &url, const QString &save,
                                                                            MusicDownLoadThreadAbstract::Download_Type type,
                                                                            QObject *parent)
{
#ifndef USE_MULTIPLE_QUERY
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicDataDownloadThread(url, save, type, parent));
        case 4: return (new MusicTTDataDownloadThread(url, save, type, parent));
    }
#endif
    return (new MusicDataDownloadThread(url, save, type, parent));
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadLrc(const QString &url, const QString &save,
                                                                       MusicDownLoadThreadAbstract::Download_Type type,
                                                                       QObject *parent)
{
#ifndef USE_MULTIPLE_QUERY
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicWYTextDownLoadThread(url, save, type, parent));
        case 4: return (new MusicTTTextDownLoadThread(url, save, type, parent));
    }
#endif
    return (new MusicTextDownLoadThread(url, save, type, parent));
}
