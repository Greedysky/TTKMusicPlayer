#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"
///////////////////////////////////////////////////////
#include "musicdownloadquerywythread.h"
#include "musicdownloadqueryxmthread.h"
#include "musicdownloadquerykgthread.h"
#include "musicdownloadquerykwthread.h"
#include "musicdownloadquerybdthread.h"
#include "musicdownloadqueryqqthread.h"
#include "musicdownloadquerymgthread.h"
#include "musicdownloadquerywsthread.h"
///////////////////////////////////////////////////////
#include "musicdownloadquerywymoviethread.h"
#include "musicdownloadqueryxmmoviethread.h"
#include "musicdownloadquerykgmoviethread.h"
#include "musicdownloadquerykwmoviethread.h"
#include "musicdownloadquerybdmoviethread.h"
#include "musicdownloadqueryqqmoviethread.h"
#include "musicdownloadquerymgmoviethread.h"
#include "musicdownloadquerywsmoviethread.h"
///////////////////////////////////////////////////////
#include "musicdownloadquerywyalbumthread.h"
#include "musicdownloadqueryxmalbumthread.h"
#include "musicdownloadquerykgalbumthread.h"
#include "musicdownloadquerykwalbumthread.h"
#include "musicdownloadquerybdalbumthread.h"
#include "musicdownloadqueryqqalbumthread.h"
#include "musicdownloadquerymgalbumthread.h"
#include "musicdownloadquerywsalbumthread.h"
///////////////////////////////////////////////////////
#include "musicdownloadquerywyartistthread.h"
#include "musicdownloadqueryxmartistthread.h"
#include "musicdownloadquerykgartistthread.h"
#include "musicdownloadquerykwartistthread.h"
#include "musicdownloadquerybdartistthread.h"
#include "musicdownloadqueryqqartistthread.h"
#include "musicdownloadquerymgartistthread.h"
#include "musicdownloadquerywsartistthread.h"
///////////////////////////////////////////////////////
#include "musicdownloadquerywytoplistthread.h"
#include "musicdownloadqueryqqtoplistthread.h"
#include "musicdownloadqueryxmtoplistthread.h"
#include "musicdownloadquerybdtoplistthread.h"
#include "musicdownloadquerykgtoplistthread.h"
#include "musicdownloadquerykwtoplistthread.h"
#include "musicdownloadquerymgtoplistthread.h"
#include "musicdownloadquerywstoplistthread.h"
///////////////////////////////////////////////////////
#include "musicdownloadquerywyplaylistthread.h"
#include "musicdownloadqueryqqplaylistthread.h"
#include "musicdownloadquerybdplaylistthread.h"
#include "musicdownloadqueryxmplaylistthread.h"
#include "musicdownloadquerykgplaylistthread.h"
#include "musicdownloadquerykwplaylistthread.h"
#include "musicdownloadquerymgplaylistthread.h"
#include "musicdownloadquerywsplaylistthread.h"
///////////////////////////////////////////////////////
#include "musicdownloadquerywyrecommendthread.h"
#include "musicdownloadqueryqqrecommendthread.h"
#include "musicdownloadquerybdrecommendthread.h"
#include "musicdownloadqueryxmrecommendthread.h"
#include "musicdownloadquerykgrecommendthread.h"
#include "musicdownloadquerykwrecommendthread.h"
#include "musicdownloadquerymgrecommendthread.h"
#include "musicdownloadquerywsrecommendthread.h"
///////////////////////////////////////////////////////
#include "musicwycommentsthread.h"
#include "musicxmcommentsthread.h"
#include "musicbdcommentsthread.h"
#include "musickgcommentsthread.h"
#include "musickwcommentsthread.h"
#include "musicqqcommentsthread.h"
#include "musicmgcommentsthread.h"
#include "musicwscommentsthread.h"
///////////////////////////////////////////////////////
#include "musicwydiscoverlistthread.h"
#include "musicxmdiscoverlistthread.h"
#include "musicbddiscoverlistthread.h"
#include "musicqqdiscoverlistthread.h"
#include "musickgdiscoverlistthread.h"
#include "musickwdiscoverlistthread.h"
#include "musicmgdiscoverlistthread.h"
#include "musicwsdiscoverlistthread.h"
///////////////////////////////////////////////////////
#include "musicwysongsuggestthread.h"
#include "musicxmsongsuggestthread.h"
#include "musicbdsongsuggestthread.h"
#include "musicqqsongsuggestthread.h"
#include "musickgsongsuggestthread.h"
#include "musickwsongsuggestthread.h"
#include "musicmgsongsuggestthread.h"
#include "musicwssongsuggestthread.h"
///////////////////////////////////////////////////////
#include "musicwyartistsimilarthread.h"
#include "musicxmartistsimilarthread.h"
#include "musicbdartistsimilarthread.h"
#include "musicqqartistsimilarthread.h"
#include "musickgartistsimilarthread.h"
#include "musickwartistsimilarthread.h"
#include "musicmgartistsimilarthread.h"
#include "musicwsartistsimilarthread.h"
///////////////////////////////////////////////////////
#include "musicdatadownloadthread.h"
///////////////////////////////////////////////////////
#include "musictextdownloadthread.h"
#include "musicwytextdownloadthread.h"
#include "musicxmtextdownloadthread.h"
#include "musickwtextdownloadthread.h"
#include "musicqqtextdownloadthread.h"
#include "musicmgtextdownloadthread.h"
#include "musicwstextdownloadthread.h"
///////////////////////////////////////////////////////
#include "musickwbackgroundthread.h"
#include "musicqqbackgroundthread.h"

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
        case 3:  thread = new MusicDownLoadQueryBDThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryKWThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKGThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryMGThread(parent); break;
        case 7:  thread = new MusicDownLoadQueryWSThread("yc", parent); break;
        case 8:  thread = new MusicDownLoadQueryWSThread("fc", parent); break;
        case 9:  thread = new MusicDownLoadQueryWSThread("bz", parent); break;
        default: thread = new MusicDownLoadQueryWYThread(parent);
    }
    M_LOGGER_INFO(QString("getQueryThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getMovieThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicDownLoadQueryWYMovieThread(parent); break;
        case 1:  thread = new MusicDownLoadQueryQQMovieThread(parent); break;
        case 2:  thread = new MusicDownLoadQueryXMMovieThread(parent); break;
        case 3:  thread = new MusicDownLoadQueryBDMovieThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryKWMovieThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKGMovieThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryMGMovieThread(parent); break;
        case 7:  thread = new MusicDownLoadQueryWSMovieThread(parent); break;
        case 8:  thread = new MusicDownLoadQueryWSMovieThread(parent); break;
        case 9:  thread = new MusicDownLoadQueryWSMovieThread(parent); break;
        default: thread = new MusicDownLoadQueryWYMovieThread(parent);
    }
    M_LOGGER_INFO(QString("getMovieThread server: %1").arg(thread->getQueryServer()));
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
        case 3:  thread = new MusicDownLoadQueryBDAlbumThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryKWAlbumThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKGAlbumThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryMGAlbumThread(parent); break;
        case 7:  thread = new MusicDownLoadQueryWSAlbumThread("yc", parent); break;
        case 8:  thread = new MusicDownLoadQueryWSAlbumThread("fc", parent); break;
        case 9:  thread = new MusicDownLoadQueryWSAlbumThread("bz", parent); break;
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
        case 3:  thread = new MusicDownLoadQueryBDArtistThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryKWArtistThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKGArtistThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryMGArtistThread(parent); break;
        case 7:  thread = new MusicDownLoadQueryWSArtistThread("yc", parent); break;
        case 8:  thread = new MusicDownLoadQueryWSArtistThread("fc", parent); break;
        case 9:  thread = new MusicDownLoadQueryWSArtistThread("bz", parent); break;
        default: thread = new MusicDownLoadQueryWYArtistThread(parent);
    }
    M_LOGGER_INFO(QString("getArtistThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getToplistThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicDownLoadQueryWYToplistThread(parent); break;
        case 1:  thread = new MusicDownLoadQueryQQToplistThread(parent); break;
        case 2:  thread = new MusicDownLoadQueryXMToplistThread(parent); break;
        case 3:  thread = new MusicDownLoadQueryBDToplistThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryKWToplistThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKGToplistThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryMGToplistThread(parent); break;
        case 7:  thread = new MusicDownLoadQueryWSToplistThread(parent); break;
        case 8:  thread = new MusicDownLoadQueryWSToplistThread(parent); break;
        case 9:  thread = new MusicDownLoadQueryWSToplistThread(parent); break;
        default: thread = new MusicDownLoadQueryWYToplistThread(parent);
    }
    M_LOGGER_INFO(QString("getPlaylistThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getPlaylistThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicDownLoadQueryWYPlaylistThread(parent); break;
        case 1:  thread = new MusicDownLoadQueryQQPlaylistThread(parent); break;
        case 2:  thread = new MusicDownLoadQueryXMPlaylistThread(parent); break;
        case 3:  thread = new MusicDownLoadQueryBDPlaylistThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryKWPlaylistThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKGPlaylistThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryMGPlaylistThread(parent); break;
        case 7:  thread = new MusicDownLoadQueryWSPlaylistThread(parent); break;
        case 8:  thread = new MusicDownLoadQueryWSPlaylistThread(parent); break;
        case 9:  thread = new MusicDownLoadQueryWSPlaylistThread(parent); break;
        default: thread = new MusicDownLoadQueryWYPlaylistThread(parent);
    }
    M_LOGGER_INFO(QString("getPlaylistThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getRecommendThread(QObject *parent)
{
    MusicDownLoadQueryThreadAbstract *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicDownLoadQueryWYRecommendThread(parent); break;
        case 1:  thread = new MusicDownLoadQueryQQRecommendThread(parent); break;
        case 2:  thread = new MusicDownLoadQueryXMRecommendThread(parent); break;
        case 3:  thread = new MusicDownLoadQueryBDRecommendThread(parent); break;
        case 4:  thread = new MusicDownLoadQueryKWRecommendThread(parent); break;
        case 5:  thread = new MusicDownLoadQueryKGRecommendThread(parent); break;
        case 6:  thread = new MusicDownLoadQueryMGRecommendThread(parent); break;
        case 7:  thread = new MusicDownLoadQueryWSRecommendThread(parent); break;
        case 8:  thread = new MusicDownLoadQueryWSRecommendThread(parent); break;
        case 9:  thread = new MusicDownLoadQueryWSRecommendThread(parent); break;
        default: thread = new MusicDownLoadQueryWYRecommendThread(parent);
    }
    M_LOGGER_INFO(QString("getPlaylistThread server: %1").arg(thread->getQueryServer()));
    return thread;
}

MusicDownLoadQueryThreadAbstract *MusicDownLoadQueryFactory::getSimilarSongThread(QObject *parent)
{
    return getQueryThread(parent);
}

MusicDownLoadSimilarThread *MusicDownLoadQueryFactory::getSimilarArtistThread(QObject *parent)
{
    MusicDownLoadSimilarThread *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicWYArtistSimilarThread(parent); break;
        case 1:  thread = new MusicQQArtistSimilarThread(parent); break;
        case 2:  thread = new MusicXMArtistSimilarThread(parent); break;
        case 3:  thread = new MusicBDArtistSimilarThread(parent); break;
        case 4:  thread = new MusicKWArtistSimilarThread(parent); break;
        case 5:  thread = new MusicKGArtistSimilarThread(parent); break;
        case 6:  thread = new MusicMGArtistSimilarThread(parent); break;
        case 7:  thread = new MusicWSArtistSimilarThread(parent); break;
        case 8:  thread = new MusicWSArtistSimilarThread(parent); break;
        case 9:  thread = new MusicWSArtistSimilarThread(parent); break;
        default: thread = new MusicWYArtistSimilarThread(parent);
    }
    return thread;
}

MusicDownLoadSongSuggestThread *MusicDownLoadQueryFactory::getSuggestThread(QObject *parent)
{
    MusicDownLoadSongSuggestThread *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicWYSongSuggestThread(parent); break;
        case 1:  thread = new MusicQQSongSuggestThread(parent); break;
        case 2:  thread = new MusicXMSongSuggestThread(parent); break;
        case 3:  thread = new MusicBDSongSuggestThread(parent); break;
        case 4:  thread = new MusicKWSongSuggestThread(parent); break;
        case 5:  thread = new MusicKGSongSuggestThread(parent); break;
        case 6:  thread = new MusicMGSongSuggestThread(parent); break;
        case 7:  thread = new MusicWSSongSuggestThread(parent); break;
        case 8:  thread = new MusicWSSongSuggestThread(parent); break;
        case 9:  thread = new MusicWSSongSuggestThread(parent); break;
        default: thread = new MusicWYSongSuggestThread(parent);
    }
    return thread;
}

MusicDownLoadCommentsThread *MusicDownLoadQueryFactory::getSongCommentThread(QObject *parent)
{
    MusicDownLoadCommentsThread *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicWYSongCommentsThread(parent); break;
        case 1:  thread = new MusicQQSongCommentsThread(parent); break;
        case 2:  thread = new MusicXMSongCommentsThread(parent); break;
        case 3:  thread = new MusicBDSongCommentsThread(parent); break;
        case 4:  thread = new MusicKWSongCommentsThread(parent); break;
        case 5:  thread = new MusicKGSongCommentsThread(parent); break;
        case 6:  thread = new MusicMGSongCommentsThread(parent); break;
        case 7:  thread = new MusicWSSongCommentsThread("yc", parent); break;
        case 8:  thread = new MusicWSSongCommentsThread("fc", parent); break;
        case 9:  thread = new MusicWSSongCommentsThread("bz", parent); break;
        default: thread = new MusicWYSongCommentsThread(parent);
    }
    return thread;
}

MusicDownLoadCommentsThread *MusicDownLoadQueryFactory::getPlaylistCommentThread(QObject *parent)
{
    MusicDownLoadCommentsThread *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicWYPlaylistCommentsThread(parent); break;
        case 1:  thread = new MusicQQPlaylistCommentsThread(parent); break;
        case 2:  thread = new MusicXMPlaylistCommentsThread(parent); break;
        case 3:  thread = new MusicBDPlaylistCommentsThread(parent); break;
        case 4:  thread = new MusicKWPlaylistCommentsThread(parent); break;
        case 5:  thread = new MusicKGPlaylistCommentsThread(parent); break;
        case 6:  thread = new MusicMGPlaylistCommentsThread(parent); break;
        case 7:  thread = new MusicWSPlaylistCommentsThread(parent); break;
        case 8:  thread = new MusicWSPlaylistCommentsThread(parent); break;
        case 9:  thread = new MusicWSPlaylistCommentsThread(parent); break;
        default: thread = new MusicWYPlaylistCommentsThread(parent);
    }
    return thread;
}

MusicDownLoadDiscoverListThread *MusicDownLoadQueryFactory::getDiscoverListThread(QObject *parent)
{
    MusicDownLoadDiscoverListThread *thread = nullptr;
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  thread = new MusicWYDiscoverListThread(parent); break;
        case 1:  thread = new MusicQQDiscoverListThread(parent); break;
        case 2:  thread = new MusicXMDiscoverListThread(parent); break;
        case 3:  thread = new MusicBDDiscoverListThread(parent); break;
        case 4:  thread = new MusicKWDiscoverListThread(parent); break;
        case 5:  thread = new MusicKGDiscoverListThread(parent); break;
        case 6:  thread = new MusicMGDiscoverListThread(parent); break;
        case 7:  thread = new MusicWSDiscoverListThread(parent); break;
        case 8:  thread = new MusicWSDiscoverListThread(parent); break;
        case 9:  thread = new MusicWSDiscoverListThread(parent); break;
        default: thread = new MusicWYDiscoverListThread(parent);
    }
    return thread;
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadSmallPicThread(const QString &url, const QString &save,
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
    }
    return (new MusicDataDownloadThread(url, save, type, parent));
}

MusicDownLoadThreadAbstract *MusicDownLoadQueryFactory::getDownloadLrcThread(const QString &url, const QString &save,
                                                                             MusicDownLoadThreadAbstract::Download_Type type,
                                                                             QObject *parent)
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0: return (new MusicWYTextDownLoadThread(url, save, type, parent));
        case 1: return (new MusicQQTextDownLoadThread(url, save, type, parent));
        case 2: return (new MusicXMTextDownLoadThread(url, save, type, parent));
        case 3: return (new MusicTextDownLoadThread(url, save, type, parent));
        case 4: return (new MusicKWTextDownLoadThread(url, save, type, parent));
        case 5: return (new MusicTextDownLoadThread(url, save, type, parent));
        case 6: return (new MusicMGTextDownLoadThread(url, save, type, parent));
        case 7: return (new MusicWSTextDownLoadThread(url, save, type, parent));
        case 8: return (new MusicWSTextDownLoadThread(url, save, type, parent));
        case 9: return (new MusicWSTextDownLoadThread(url, save, type, parent));
    }
    return (new MusicTextDownLoadThread(url, save, type, parent));
}

MusicDownloadBackgroundThread *MusicDownLoadQueryFactory::getDownloadBigPicThread(const QString &name, const QString &save,
                                                                                  QObject *parent)
{
    bool ll = M_SETTING_PTR->value(MusicSettingManager::OtherBgLosslessChoiced).toBool();
    if(ll)
    {
        return (new MusicQQBackgroundThread(name, save, parent));
    }
    else
    {
        return (new MusicKWBackgroundThread(name, save, parent));
    }
}
