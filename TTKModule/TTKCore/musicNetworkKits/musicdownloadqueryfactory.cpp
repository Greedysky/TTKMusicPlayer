#include "musicdownloadqueryfactory.h"
//
#include "musicwyqueryrequest.h"
#include "musickgqueryrequest.h"
#include "musickwqueryrequest.h"
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeQueryRequest(QObject *parent)
{
    MusicAbstractQueryRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: req = new MusicWYQueryRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWQueryRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: req = new MusicKGQueryRequest(parent); break;
        default: req = new MusicWYQueryRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryrequest server:" << req->queryServer());
    return req;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeMovieRequest(QObject *parent)
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeAlbumRequest(QObject *parent)
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistRequest(QObject *parent)
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistListRequest(QObject *parent)
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistAlbumRequest(QObject *parent)
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistMovieRequest(QObject *parent)
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeToplistRequest(QObject *parent)
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

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makePlaylistRequest(QObject *parent)
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

MusicCommentsRequest *MusicDownLoadQueryFactory::makeSongCommentRequest(QObject *parent)
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

MusicCommentsRequest *MusicDownLoadQueryFactory::makePlaylistCommentRequest(QObject *parent)
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

MusicDiscoverListRequest *MusicDownLoadQueryFactory::makeDiscoverListRequest(QObject *parent)
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

MusicCoverRequest *MusicDownLoadQueryFactory::makeCoverRequest(QObject *parent)
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

MusicAbstractDownLoadRequest *MusicDownLoadQueryFactory::makeLrcRequest(const QString &url, const QString &path, QObject *parent)
{
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: return (new MusicWYDownLoadTextRequest(url, path, parent));
        case MusicAbstractQueryRequest::QueryServer::KW: return (new MusicKWDownLoadTextRequest(url, path, parent));
        case MusicAbstractQueryRequest::QueryServer::KG: return (new MusicDownLoadTextRequest(url, path, parent));
        default: break;
    }
    return (new MusicDownLoadTextRequest(url, path, parent));
}

MusicAbstractDownLoadRequest *MusicDownLoadQueryFactory::makeCoverRequest(const QString &url, const QString &path, QObject *parent)
{
    MusicAbstractDownLoadRequest *req = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::KW: req = new MusicKWDownLoadCoverRequest(url, path, parent); break;
        default: req = new MusicDownloadDataRequest(url, path, TTK::Download::Cover, parent); break;
    }
    return req;
}

MusicDownloadBackgroundRequest *MusicDownLoadQueryFactory::makeBackgroundRequest(const QString &name, const QString &path, QObject *parent)
{
    return (new MusicDownloadBackgroundRequest(name, path, parent));
}
