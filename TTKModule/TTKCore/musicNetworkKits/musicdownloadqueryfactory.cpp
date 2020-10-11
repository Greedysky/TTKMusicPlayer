#include "musicdownloadqueryfactory.h"
#include "musicsettingmanager.h"
//
#include "musicwyqueryrequest.h"
#include "musicxmqueryrequest.h"
#include "musickgqueryrequest.h"
#include "musickwqueryrequest.h"
#include "musicqqqueryrequest.h"
//
#include "musicwyquerymovierequest.h"
#include "musicxmquerymovierequest.h"
#include "musickgquerymovierequest.h"
#include "musickwquerymovierequest.h"
#include "musicqqquerymovierequest.h"
//
#include "musicwyqueryalbumrequest.h"
#include "musicxmqueryalbumrequest.h"
#include "musickgqueryalbumrequest.h"
#include "musickwqueryalbumrequest.h"
#include "musicqqqueryalbumrequest.h"
//
#include "musicwyqueryartistrequest.h"
#include "musicxmqueryartistrequest.h"
#include "musickgqueryartistrequest.h"
#include "musickwqueryartistrequest.h"
#include "musicqqqueryartistrequest.h"
//
#include "musicwyqueryartistlistrequest.h"
#include "musicxmqueryartistlistrequest.h"
#include "musickgqueryartistlistrequest.h"
#include "musickwqueryartistlistrequest.h"
#include "musicqqqueryartistlistrequest.h"
//
#include "musicwyquerytoplistrequest.h"
#include "musicqqquerytoplistrequest.h"
#include "musicxmquerytoplistrequest.h"
#include "musickgquerytoplistrequest.h"
#include "musickwquerytoplistrequest.h"
//
#include "musicwyqueryplaylistrequest.h"
#include "musicqqqueryplaylistrequest.h"
#include "musicxmqueryplaylistrequest.h"
#include "musickgqueryplaylistrequest.h"
#include "musickwqueryplaylistrequest.h"
//
#include "musicwyqueryrecommendrequest.h"
#include "musicqqqueryrecommendrequest.h"
#include "musicxmqueryrecommendrequest.h"
#include "musickgqueryrecommendrequest.h"
#include "musickwqueryrecommendrequest.h"
//
#include "musicwycommentsrequest.h"
#include "musicxmcommentsrequest.h"
#include "musickgcommentsrequest.h"
#include "musickwcommentsrequest.h"
#include "musicqqcommentsrequest.h"
//
#include "musicwydiscoverlistrequest.h"
#include "musicxmdiscoverlistrequest.h"
#include "musicqqdiscoverlistrequest.h"
#include "musickgdiscoverlistrequest.h"
#include "musickwdiscoverlistrequest.h"
//
#include "musicwysongsuggestrequest.h"
#include "musicxmsongsuggestrequest.h"
#include "musicqqsongsuggestrequest.h"
#include "musickgsongsuggestrequest.h"
#include "musickwsongsuggestrequest.h"
//
#include "musicwyartistsimilarrequest.h"
#include "musicxmartistsimilarrequest.h"
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
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryRequest(parent); break;
        default: request = new MusicWYQueryRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryrequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getMovieRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryMovieRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryMovieRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryMovieRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryMovieRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryMovieRequest(parent); break;
        default: request = new MusicWYQueryMovieRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryMovieRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getAlbumRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryAlbumRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryAlbumRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryAlbumRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryAlbumRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryAlbumRequest(parent); break;
        default: request = new MusicWYQueryAlbumRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryAlbumRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getArtistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryArtistRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryArtistRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryArtistRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryArtistRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryArtistRequest(parent); break;
        default: request = new MusicWYQueryArtistRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryArtistRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getArtistListRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryArtistListRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryArtistListRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryArtistListRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryArtistListRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryArtistListRequest(parent); break;
        default: request = new MusicWYQueryArtistListRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryArtistListRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getToplistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryToplistRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryToplistRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryToplistRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryToplistRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryToplistRequest(parent); break;
        default: request = new MusicWYQueryToplistRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryToplistRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getPlaylistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryPlaylistRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryPlaylistRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryPlaylistRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryPlaylistRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryPlaylistRequest(parent); break;
        default: request = new MusicWYQueryPlaylistRequest(parent);
    }
    TTK_LOGGER_INFO(QString("MusicQueryPlaylistRequest server: %1").arg(request->getQueryServer()));
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::getRecommendRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYQueryRecommendRequest(parent); break;
        case QQQueryServer:  request = new MusicQQQueryRecommendRequest(parent); break;
        case XMQueryServer:  request = new MusicXMQueryRecommendRequest(parent); break;
        case KWQueryServer:  request = new MusicKWQueryRecommendRequest(parent); break;
        case KGQueryServer:  request = new MusicKGQueryRecommendRequest(parent); break;
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
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYArtistSimilarRequest(parent); break;
        case QQQueryServer:  request = new MusicQQArtistSimilarRequest(parent); break;
        case XMQueryServer:  request = new MusicXMArtistSimilarRequest(parent); break;
        case KWQueryServer:  request = new MusicKWArtistSimilarRequest(parent); break;
        case KGQueryServer:  request = new MusicKGArtistSimilarRequest(parent); break;
        default: request = new MusicWYArtistSimilarRequest(parent);
    }
    return request;
}

MusicSongSuggestRequest *MusicDownLoadQueryFactory::getSuggestRequest(QObject *parent)
{
    MusicSongSuggestRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYSongSuggestRequest(parent); break;
        case QQQueryServer:  request = new MusicQQSongSuggestRequest(parent); break;
        case XMQueryServer:  request = new MusicXMSongSuggestRequest(parent); break;
        case KWQueryServer:  request = new MusicKWSongSuggestRequest(parent); break;
        case KGQueryServer:  request = new MusicKGSongSuggestRequest(parent); break;
        default: request = new MusicWYSongSuggestRequest(parent);
    }
    return request;
}

MusicCommentsRequest *MusicDownLoadQueryFactory::getSongCommentRequest(QObject *parent)
{
    MusicCommentsRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYSongCommentsRequest(parent); break;
        case QQQueryServer:  request = new MusicQQSongCommentsRequest(parent); break;
        case XMQueryServer:  request = new MusicXMSongCommentsRequest(parent); break;
        case KWQueryServer:  request = new MusicKWSongCommentsRequest(parent); break;
        case KGQueryServer:  request = new MusicKGSongCommentsRequest(parent); break;
        default: request = new MusicWYSongCommentsRequest(parent);
    }
    return request;
}

MusicCommentsRequest *MusicDownLoadQueryFactory::getPlaylistCommentRequest(QObject *parent)
{
    MusicCommentsRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYPlaylistCommentsRequest(parent); break;
        case QQQueryServer:  request = new MusicQQPlaylistCommentsRequest(parent); break;
        case XMQueryServer:  request = new MusicXMPlaylistCommentsRequest(parent); break;
        case KWQueryServer:  request = new MusicKWPlaylistCommentsRequest(parent); break;
        case KGQueryServer:  request = new MusicKGPlaylistCommentsRequest(parent); break;
        default: request = new MusicWYPlaylistCommentsRequest(parent);
    }
    return request;
}

MusicDiscoverListRequest *MusicDownLoadQueryFactory::getDiscoverListRequest(QObject *parent)
{
    MusicDiscoverListRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYDiscoverListRequest(parent); break;
        case QQQueryServer:  request = new MusicQQDiscoverListRequest(parent); break;
        case XMQueryServer:  request = new MusicXMDiscoverListRequest(parent); break;
        case KWQueryServer:  request = new MusicKWDiscoverListRequest(parent); break;
        case KGQueryServer:  request = new MusicKGDiscoverListRequest(parent); break;
        default: request = new MusicWYDiscoverListRequest(parent);
    }
    return request;
}

MusicTranslationRequest *MusicDownLoadQueryFactory::getTranslationRequest(QObject *parent)
{
    MusicTranslationRequest *request = nullptr;
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer:  request = new MusicWYTranslationRequest(parent); break;
        case QQQueryServer:  request = new MusicBDTranslationRequest(parent); break;
        case XMQueryServer:  request = new MusicBDTranslationRequest(parent); break;
        case KWQueryServer:  request = new MusicBDTranslationRequest(parent); break;
        case KGQueryServer:  request = new MusicBDTranslationRequest(parent); break;
        default: request = new MusicBDTranslationRequest(parent);
    }
    return request;
}

MusicAbstractDownLoadRequest *MusicDownLoadQueryFactory::getDownloadSmallPicRequest(const QString &url, const QString &save,
                                                                                    MusicObject::DownloadType type,
                                                                                    QObject *parent)
{
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer: return (new MusicDownloadDataRequest(url, save, type, parent));
        case QQQueryServer: return (new MusicDownloadDataRequest(url, save, type, parent));
        case XMQueryServer: return (new MusicDownloadDataRequest(url, save, type, parent));
        case KWQueryServer: return (new MusicDownloadDataRequest(url, save, type, parent));
        case KGQueryServer: return (new MusicDownloadDataRequest(url, save, type, parent));
    }
    return (new MusicDownloadDataRequest(url, save, type, parent));
}

MusicAbstractDownLoadRequest *MusicDownLoadQueryFactory::getDownloadLrcRequest(const QString &url, const QString &save,
                                                                               MusicObject::DownloadType type,
                                                                               QObject *parent)
{
    const int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    switch(TTKStatic_cast(DownloadQueryServer, index))
    {
        case WYQueryServer: return (new MusicWYDownLoadTextRequest(url, save, type, parent));
        case QQQueryServer: return (new MusicQQDownLoadTextRequest(url, save, type, parent));
        case XMQueryServer: return (new MusicXMDownLoadTextRequest(url, save, type, parent));
        case KWQueryServer: return (new MusicKWDownLoadTextRequest(url, save, type, parent));
        case KGQueryServer: return (new MusicDownLoadTextRequest(url, save, type, parent));
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
