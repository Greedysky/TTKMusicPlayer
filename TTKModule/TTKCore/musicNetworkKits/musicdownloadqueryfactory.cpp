#include "musicdownloadqueryfactory.h"
//
#include "musicwyquerysongrequest.h"
#include "musickgquerysongrequest.h"
#include "musickwquerysongrequest.h"
//
#include "musicwyquerymovierequest.h"
#include "musickgquerymovierequest.h"
#include "musickwquerymovierequest.h"
//
#include "musicwyqueryalbumrequest.h"
#include "musickgqueryalbumrequest.h"
#include "musickwqueryalbumrequest.h"
//
#include "musicwyqueryartistrequest.h"
#include "musickgqueryartistrequest.h"
#include "musickwqueryartistrequest.h"
//
#include "musicwyqueryartistlistrequest.h"
#include "musickgqueryartistlistrequest.h"
#include "musickwqueryartistlistrequest.h"
//
#include "musicwyquerytoplistrequest.h"
#include "musickgquerytoplistrequest.h"
#include "musickwquerytoplistrequest.h"
//
#include "musicwyqueryplaylistrequest.h"
#include "musickgqueryplaylistrequest.h"
#include "musickwqueryplaylistrequest.h"
//
#include "musicwycommentsrequest.h"
#include "musickgcommentsrequest.h"
#include "musickwcommentsrequest.h"
//
#include "musicwydiscoverlistrequest.h"
#include "musickgdiscoverlistrequest.h"
#include "musickwdiscoverlistrequest.h"
//
#include "musickwcoverrequest.h"
#include "musiccoversourcerequest.h"
//
#include "musicdownloaddatarequest.h"
//
#include "musicdownloadtextrequest.h"
#include "musicwydownloadtextrequest.h"
#include "musickwdownloadtextrequest.h"
//
#include "musickwdownloadimagerequest.h"
#include "musicdownloadbackgroundrequest.h"

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeSongRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQuerySongRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQuerySongRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQuerySongRequest(parent); break;
        default: req = new MusicWYQuerySongRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryrequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeNewSongRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryNewSongRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryNewSongRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryNewSongRequest(parent); break;
        default: req = new MusicWYQueryNewSongRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryrequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeMovieRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryMovieRequest(parent); break;
        default: req = new MusicWYQueryMovieRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryMovieRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeAlbumRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryAlbumRequest(parent); break;
        default: req = new MusicWYQueryAlbumRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryAlbumRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeNewAlbumRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryNewAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryNewAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryNewAlbumRequest(parent); break;
        default: req = new MusicWYQueryNewAlbumRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryAlbumRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeArtistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryArtistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryArtistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryArtistRequest(parent); break;
        default: req = new MusicWYQueryArtistRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeArtistListRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryArtistListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryArtistListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryArtistListRequest(parent); break;
        default: req = new MusicWYQueryArtistListRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistListRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeHotArtistListRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryHotArtistListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryHotArtistListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryHotArtistListRequest(parent); break;
        default: req = new MusicWYQueryHotArtistListRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistListRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeArtistAlbumRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryArtistAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryArtistAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryArtistAlbumRequest(parent); break;
        default: req = new MusicWYQueryArtistAlbumRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistAlbumRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeArtistMovieRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryArtistMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryArtistMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryArtistMovieRequest(parent); break;
        default: req = new MusicWYQueryArtistMovieRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistMovieRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makeToplistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryToplistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryToplistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryToplistRequest(parent); break;
        default: req = new MusicWYQueryToplistRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryToplistRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makePlaylistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryPlaylistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryPlaylistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryPlaylistRequest(parent); break;
        default: req = new MusicWYQueryPlaylistRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryPlaylistRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makePlaylistRecommendRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryPlaylistRecommendRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryPlaylistRecommendRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryPlaylistRecommendRequest(parent); break;
        default: req = new MusicWYQueryPlaylistRecommendRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryPlaylistRequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownloadQueryFactory::makePlaylistHighRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryPlaylistHighRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryPlaylistHighRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryPlaylistHighRequest(parent); break;
        default: req = new MusicWYQueryPlaylistHighRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryPlaylistRequest server:" << req->queryServer());
    return req;
}

MusicCommentsRequest *MusicDownloadQueryFactory::makeSongCommentRequest(QObject *parent)
{
    MusicCommentsRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYSongCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWSongCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGSongCommentsRequest(parent); break;
        default: req = new MusicWYSongCommentsRequest(parent); break;
    }
    return req;
}

MusicCommentsRequest *MusicDownloadQueryFactory::makePlaylistCommentRequest(QObject *parent)
{
    MusicCommentsRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYPlaylistCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWPlaylistCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGPlaylistCommentsRequest(parent); break;
        default: req = new MusicWYPlaylistCommentsRequest(parent); break;
    }
    return req;
}

MusicDiscoverListRequest *MusicDownloadQueryFactory::makeDiscoverListRequest(QObject *parent)
{
    MusicDiscoverListRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYDiscoverListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWDiscoverListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGDiscoverListRequest(parent); break;
        default: req = new MusicWYDiscoverListRequest(parent); break;
    }
    return req;
}

MusicCoverRequest *MusicDownloadQueryFactory::makeCoverRequest(QObject *parent)
{
    MusicCoverRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWCoverSourceRequest(parent); break;
        default: req = new MusicCoverSourceRequest(parent); break;
    }
    return req;
}

MusicAbstractDownloadRequest *MusicDownloadQueryFactory::makeLrcRequest(const QString &url, const QString &path, QObject *parent)
{
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: return (new MusicWYDownloadTextRequest(url, path, parent));
        case MusicAbstractQueryRequest::QueryServer::KW: return (new MusicKWDownloadTextRequest(url, path, parent));
        case MusicAbstractQueryRequest::QueryServer::KG: return (new MusicDownloadTextRequest(url, path, parent));
        default: break;
    }
    return (new MusicDownloadTextRequest(url, path, parent));
}

MusicAbstractDownloadRequest *MusicDownloadQueryFactory::makeCoverRequest(const QString &url, const QString &path, QObject *parent)
{
    MusicAbstractDownloadRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWDownloadCoverRequest(url, path, parent); break;
        default: req = new MusicDownloadDataRequest(url, path, TTK::Download::Cover, parent); break;
    }
    return req;
}

MusicDownloadBackgroundRequest *MusicDownloadQueryFactory::makeBackgroundRequest(const QString &name, const QString &path, QObject *parent)
{
    return (new MusicDownloadBackgroundRequest(name, path, parent));
}
