#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"
#include "musicdownloadquerymultiplethread.h"
#include "musicdownloadqueryttthread.h"
#include "musicdownloadquerywythread.h"

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getQueryThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *downLoadManager = nullptr;
#ifndef USE_MULTIPLE_QUERY
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  downLoadManager = new MusicDownLoadQueryWYThread(parent); break;
//        case 1:  downLoadManager = new MusicDownLoadQueryWYThread(parent);
//        case 2:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 3:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 4:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 5:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 6:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 7:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 8:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 9:  downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 10: downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 11: downLoadManager = new MusicDownLoadQueryTTThread(parent);
//        case 12: downLoadManager = new MusicDownLoadQueryTTThread(parent);
    }
#else
    downLoadManager = new MusicDownLoadQueryMultipleThread(parent);
#endif
    return downLoadManager;
}

QString MusicDownLoadQueryFactory::getClassName()
{
    return "MusicDownLoadQueryFactory";
}
