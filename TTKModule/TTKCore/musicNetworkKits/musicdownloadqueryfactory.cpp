#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"
//
#include "musicwyqueryrequest.h"
#include "musicxmqueryrequest.h"
#include "musickgqueryrequest.h"
#include "musickwqueryrequest.h"
#include "musicbdqueryrequest.h"
#include "musicqqqueryrequest.h"
//
#include "musicwyquerymovierequest.h"
#include "musicxmquerymovierequest.h"
#include "musickgquerymovierequest.h"
#include "musickwquerymovierequest.h"
#include "musicbdquerymovierequest.h"
#include "musicqqquerymovierequest.h"
//
#include "musicwyqueryalbumrequest.h"
#include "musicxmqueryalbumrequest.h"
#include "musickgqueryalbumrequest.h"
#include "musickwqueryalbumrequest.h"
#include "musicbdqueryalbumrequest.h"
#include "musicqqqueryalbumrequest.h"
//
#include "musicwyqueryartistrequest.h"
#include "musicxmqueryartistrequest.h"
#include "musickgqueryartistrequest.h"
#include "musickwqueryartistrequest.h"
#include "musicbdqueryartistrequest.h"
#include "musicqqqueryartistrequest.h"
//
#include "musicwyqueryartistlistrequest.h"
#include "musicxmqueryartistlistrequest.h"
#include "musickgqueryartistlistrequest.h"
#include "musickwqueryartistlistrequest.h"
#include "musicbdqueryartistlistrequest.h"
#include "musicqqqueryartistlistrequest.h"
//
#include "musicwyquerytoplistrequest.h"
#include "musicqqquerytoplistrequest.h"
#include "musicxmquerytoplistrequest.h"
#include "musicbdquerytoplistrequest.h"
#include "musickgquerytoplistrequest.h"
#include "musickwquerytoplistrequest.h"
//
#include "musicwyqueryplaylistrequest.h"
#include "musicqqqueryplaylistrequest.h"
#include "musicbdqueryplaylistrequest.h"
#include "musicxmqueryplaylistrequest.h"
#include "musickgqueryplaylistrequest.h"
#include "musickwqueryplaylistrequest.h"
//
#include "musicwyqueryrecommendrequest.h"
#include "musicqqqueryrecommendrequest.h"
#include "musicbdqueryrecommendrequest.h"
#include "musicxmqueryrecommendrequest.h"
#include "musickgqueryrecommendrequest.h"
#include "musickwqueryrecommendrequest.h"
//
#include "musicwycommentsrequest.h"
#include "musicxmcommentsrequest.h"
#include "musicbdcommentsrequest.h"
#include "musickgcommentsrequest.h"
#include "musickwcommentsrequest.h"
#include "musicqqcommentsrequest.h"
//
#include "musicwydiscoverlistrequest.h"
#include "musicxmdiscoverlistrequest.h"
#include "musicbddiscoverlistrequest.h"
#include "musicqqdiscoverlistrequest.h"
#include "musickgdiscoverlistrequest.h"
#include "musickwdiscoverlistrequest.h"
//
#include "musicwysongsuggestrequest.h"
#include "musicxmsongsuggestrequest.h"
#include "musicbdsongsuggestrequest.h"
#include "musicqqsongsuggestrequest.h"
#include "musickgsongsuggestrequest.h"
#include "musickwsongsuggestrequest.h"
//
#include "musicwyartistsimilarrequest.h"
#include "musicxmartistsimilarrequest.h"
#include "musicbdartistsimilarrequest.h"
#include "musicqqartistsimilarrequest.h"
#include "musickgartistsimilarrequest.h"
#include "musickwartistsimilarrequest.h"
//
#include "musicbdtranslationrequest.h"
#include "musicwytranslationrequest.h"
//
#include "musicdownloaddatarequest.h"
//
#include "musicdownloadtextrequest.h"
#include "musicwydownloadtextrequest.h"
#include "musicxmdownloadtextrequest.h"
#include "musickwdownloadtextrequest.h"
#include "musicqqdownloadtextrequest.h"
//
#include "musickwdownloadbackgroundrequest.h"
#include "musicqqdownloadbackgroundrequest.h"
//

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getQueryRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryRequest(parent); break;
        case 1:  request = new MusicQQQueryRequest(parent); break;
        case 2:  request = new MusicXMQueryRequest(parent); break;
        case 3:  request = new MusicBDQueryRequest(parent); break;
        case 4:  request = new MusicKWQueryRequest(parent); break;
        case 5:  request = new MusicKGQueryRequest(parent); break;
        default: request = new MusicWYQueryRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryrequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getMovieRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryMovieRequest(parent); break;
        case 1:  request = new MusicQQQueryMovieRequest(parent); break;
        case 2:  request = new MusicXMQueryMovieRequest(parent); break;
        case 3:  request = new MusicBDQueryMovieRequest(parent); break;
        case 4:  request = new MusicKWQueryMovieRequest(parent); break;
        case 5:  request = new MusicKGQueryMovieRequest(parent); break;
        default: request = new MusicWYQueryMovieRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryMovieRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getAlbumRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryAlbumRequest(parent); break;
        case 1:  request = new MusicQQQueryAlbumRequest(parent); break;
        case 2:  request = new MusicXMQueryAlbumRequest(parent); break;
        case 3:  request = new MusicBDQueryAlbumRequest(parent); break;
        case 4:  request = new MusicKWQueryAlbumRequest(parent); break;
        case 5:  request = new MusicKGQueryAlbumRequest(parent); break;
        default: request = new MusicWYQueryAlbumRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryAlbumRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getArtistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryArtistRequest(parent); break;
        case 1:  request = new MusicQQQueryArtistRequest(parent); break;
        case 2:  request = new MusicXMQueryArtistRequest(parent); break;
        case 3:  request = new MusicBDQueryArtistRequest(parent); break;
        case 4:  request = new MusicKWQueryArtistRequest(parent); break;
        case 5:  request = new MusicKGQueryArtistRequest(parent); break;
        default: request = new MusicWYQueryArtistRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryArtistRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getArtistListRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryArtistListRequest(parent); break;
        case 1:  request = new MusicQQQueryArtistListRequest(parent); break;
        case 2:  request = new MusicXMQueryArtistListRequest(parent); break;
        case 3:  request = new MusicBDQueryArtistListRequest(parent); break;
        case 4:  request = new MusicKWQueryArtistListRequest(parent); break;
        case 5:  request = new MusicKGQueryArtistListRequest(parent); break;
        default: request = new MusicWYQueryArtistListRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryArtistListRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getToplistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryToplistRequest(parent); break;
        case 1:  request = new MusicQQQueryToplistRequest(parent); break;
        case 2:  request = new MusicXMQueryToplistRequest(parent); break;
        case 3:  request = new MusicBDQueryToplistRequest(parent); break;
        case 4:  request = new MusicKWQueryToplistRequest(parent); break;
        case 5:  request = new MusicKGQueryToplistRequest(parent); break;
        default: request = new MusicWYQueryToplistRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryToplistRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getPlaylistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryPlaylistRequest(parent); break;
        case 1:  request = new MusicQQQueryPlaylistRequest(parent); break;
        case 2:  request = new MusicXMQueryPlaylistRequest(parent); break;
        case 3:  request = new MusicBDQueryPlaylistRequest(parent); break;
        case 4:  request = new MusicKWQueryPlaylistRequest(parent); break;
        case 5:  request = new MusicKGQueryPlaylistRequest(parent); break;
        default: request = new MusicWYQueryPlaylistRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryPlaylistRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getRecommendRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYQueryRecommendRequest(parent); break;
        case 1:  request = new MusicQQQueryRecommendRequest(parent); break;
        case 2:  request = new MusicXMQueryRecommendRequest(parent); break;
        case 3:  request = new MusicBDQueryRecommendRequest(parent); break;
        case 4:  request = new MusicKWQueryRecommendRequest(parent); break;
        case 5:  request = new MusicKGQueryRecommendRequest(parent); break;
        default: request = new MusicWYQueryRecommendRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryRecommendRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getSimilarSongRequest(QObject *parent)
{
    return getQueryRequest(parent);
}

MusicSimilarRequest *MusicDownLoadQueryFactory::getSimilarArtistRequest(QObject *parent)
{
    MusicSimilarRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYArtistSimilarRequest(parent); break;
        case 1:  request = new MusicQQArtistSimilarRequest(parent); break;
        case 2:  request = new MusicXMArtistSimilarRequest(parent); break;
        case 3:  request = new MusicBDArtistSimilarRequest(parent); break;
        case 4:  request = new MusicKWArtistSimilarRequest(parent); break;
        case 5:  request = new MusicKGArtistSimilarRequest(parent); break;
        default: request = new MusicWYArtistSimilarRequest(parent);
    }
    return request;
}

MusicSongSuggestRequest *MusicDownLoadQueryFactory::getSuggestRequest(QObject *parent)
{
    MusicSongSuggestRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYSongSuggestRequest(parent); break;
        case 1:  request = new MusicQQSongSuggestRequest(parent); break;
        case 2:  request = new MusicXMSongSuggestRequest(parent); break;
        case 3:  request = new MusicBDSongSuggestRequest(parent); break;
        case 4:  request = new MusicKWSongSuggestRequest(parent); break;
        case 5:  request = new MusicKGSongSuggestRequest(parent); break;
        default: request = new MusicWYSongSuggestRequest(parent);
    }
    return request;
}

MusicCommentsRequest *MusicDownLoadQueryFactory::getSongCommentRequest(QObject *parent)
{
    MusicCommentsRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYSongCommentsRequest(parent); break;
        case 1:  request = new MusicQQSongCommentsRequest(parent); break;
        case 2:  request = new MusicXMSongCommentsRequest(parent); break;
        case 3:  request = new MusicBDSongCommentsRequest(parent); break;
        case 4:  request = new MusicKWSongCommentsRequest(parent); break;
        case 5:  request = new MusicKGSongCommentsRequest(parent); break;
        default: request = new MusicWYSongCommentsRequest(parent);
    }
    return request;
}

MusicCommentsRequest *MusicDownLoadQueryFactory::getPlaylistCommentRequest(QObject *parent)
{
    MusicCommentsRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYPlaylistCommentsRequest(parent); break;
        case 1:  request = new MusicQQPlaylistCommentsRequest(parent); break;
        case 2:  request = new MusicXMPlaylistCommentsRequest(parent); break;
        case 3:  request = new MusicBDPlaylistCommentsRequest(parent); break;
        case 4:  request = new MusicKWPlaylistCommentsRequest(parent); break;
        case 5:  request = new MusicKGPlaylistCommentsRequest(parent); break;
        default: request = new MusicWYPlaylistCommentsRequest(parent);
    }
    return request;
}

MusicDiscoverListRequest *MusicDownLoadQueryFactory::getDiscoverListRequest(QObject *parent)
{
    MusicDiscoverListRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYDiscoverListRequest(parent); break;
        case 1:  request = new MusicQQDiscoverListRequest(parent); break;
        case 2:  request = new MusicXMDiscoverListRequest(parent); break;
        case 3:  request = new MusicBDDiscoverListRequest(parent); break;
        case 4:  request = new MusicKWDiscoverListRequest(parent); break;
        case 5:  request = new MusicKGDiscoverListRequest(parent); break;
        default: request = new MusicWYDiscoverListRequest(parent);
    }
    return request;
}

MusicTranslationRequest *MusicDownLoadQueryFactory::getTranslationRequest(QObject *parent)
{
    MusicTranslationRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0:  request = new MusicWYTranslationRequest(parent); break;
        case 1:  request = new MusicBDTranslationRequest(parent); break;
        case 2:  request = new MusicBDTranslationRequest(parent); break;
        case 3:  request = new MusicBDTranslationRequest(parent); break;
        case 4:  request = new MusicBDTranslationRequest(parent); break;
        case 5:  request = new MusicBDTranslationRequest(parent); break;
        default: request = new MusicBDTranslationRequest(parent);
    }
    return request;
}

MusicAbstractDownLoadRequest *MusicDownLoadQueryFactory::getDownloadSmallPicRequest(const QString &url, const QString &save,
                                                                                    MusicObject::DownloadType type,
                                                                                    QObject *parent)
{
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0: return (new MusicDownloadDataRequest(url, save, type, parent));
        case 1: return (new MusicDownloadDataRequest(url, save, type, parent));
        case 2: return (new MusicDownloadDataRequest(url, save, type, parent));
        case 3: return (new MusicDownloadDataRequest(url, save, type, parent));
        case 4: return (new MusicDownloadDataRequest(url, save, type, parent));
        case 5: return (new MusicDownloadDataRequest(url, save, type, parent));
    }
    return (new MusicDownloadDataRequest(url, save, type, parent));
}

MusicAbstractDownLoadRequest *MusicDownLoadQueryFactory::getDownloadLrcRequest(const QString &url, const QString &save,
                                                                               MusicObject::DownloadType type,
                                                                               QObject *parent)
{
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(index)
    {
        case 0: return (new MusicWYDownLoadTextRequest(url, save, type, parent));
        case 1: return (new MusicQQDownLoadTextRequest(url, save, type, parent));
        case 2: return (new MusicXMDownLoadTextRequest(url, save, type, parent));
        case 3: return (new MusicDownLoadTextRequest(url, save, type, parent));
        case 4: return (new MusicKWDownLoadTextRequest(url, save, type, parent));
        case 5: return (new MusicDownLoadTextRequest(url, save, type, parent));
    }
    return (new MusicDownLoadTextRequest(url, save, type, parent));
}

MusicDownloadBackgroundRequest *MusicDownLoadQueryFactory::getDownloadBigPicRequest(const QString &name, const QString &save,
                                                                                    QObject *parent)
{
    const bool index = M_SETTING_PTR->value(MusicSettingManager::OtherBackgroundLossless).toBool();
    if(index)
    {
        return (new MusicQQDownloadBackgroundRequest(name, save, parent));
    }
    else
    {
        return (new MusicKWDownloadBackgroundRequest(name, save, parent));
    }
}
