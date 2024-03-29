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
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryRequest(parent); break;
        default: request = new MusicWYQueryRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryrequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeMovieRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryMovieRequest(parent); break;
        default: request = new MusicWYQueryMovieRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryMovieRequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeAlbumRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryAlbumRequest(parent); break;
        default: request = new MusicWYQueryAlbumRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryAlbumRequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryArtistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryArtistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryArtistRequest(parent); break;
        default: request = new MusicWYQueryArtistRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistRequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistListRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryArtistListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryArtistListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryArtistListRequest(parent); break;
        default: request = new MusicWYQueryArtistListRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistListRequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistAlbumRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryArtistAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryArtistAlbumRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryArtistAlbumRequest(parent); break;
        default: request = new MusicWYQueryArtistAlbumRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistAlbumRequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeArtistMovieRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryArtistMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryArtistMovieRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryArtistMovieRequest(parent); break;
        default: request = new MusicWYQueryArtistMovieRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryArtistMovieRequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makeToplistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryToplistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryToplistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryToplistRequest(parent); break;
        default: request = new MusicWYQueryToplistRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryToplistRequest server:" << request->queryServer());
    return request;
}

MusicAbstractQueryRequest *MusicDownLoadQueryFactory::makePlaylistRequest(QObject *parent)
{
    MusicAbstractQueryRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYQueryPlaylistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWQueryPlaylistRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGQueryPlaylistRequest(parent); break;
        default: request = new MusicWYQueryPlaylistRequest(parent); break;
    }
    TTK_INFO_STREAM("MusicQueryPlaylistRequest server:" << request->queryServer());
    return request;
}

MusicCommentsRequest *MusicDownLoadQueryFactory::makeSongCommentRequest(QObject *parent)
{
    MusicCommentsRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYSongCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWSongCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGSongCommentsRequest(parent); break;
        default: request = new MusicWYSongCommentsRequest(parent); break;
    }
    return request;
}

MusicCommentsRequest *MusicDownLoadQueryFactory::makePlaylistCommentRequest(QObject *parent)
{
    MusicCommentsRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYPlaylistCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWPlaylistCommentsRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGPlaylistCommentsRequest(parent); break;
        default: request = new MusicWYPlaylistCommentsRequest(parent); break;
    }
    return request;
}

MusicDiscoverListRequest *MusicDownLoadQueryFactory::makeDiscoverListRequest(QObject *parent)
{
    MusicDiscoverListRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::WY: request = new MusicWYDiscoverListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWDiscoverListRequest(parent); break;
        case MusicAbstractQueryRequest::QueryServer::KG: request = new MusicKGDiscoverListRequest(parent); break;
        default: request = new MusicWYDiscoverListRequest(parent); break;
    }
    return request;
}

MusicCoverRequest *MusicDownLoadQueryFactory::makeCoverRequest(QObject *parent)
{
    MusicCoverRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWCoverSourceRequest(parent); break;
        default: request = new MusicCoverSourceRequest(parent); break;
    }
    return request;
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
    MusicAbstractDownLoadRequest *request = nullptr;
    const int index = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    switch(TTKStaticCast(MusicAbstractQueryRequest::QueryServer, index))
    {
        case MusicAbstractQueryRequest::QueryServer::KW: request = new MusicKWDownLoadCoverRequest(url, path, parent); break;
        default: request = new MusicDownloadDataRequest(url, path, TTK::Download::Cover, parent); break;
    }
    return request;
}

MusicDownloadBackgroundRequest *MusicDownLoadQueryFactory::makeBackgroundRequest(const QString &name, const QString &path, QObject *parent)
{
    return (new MusicDownloadBackgroundRequest(name, path, parent));
}
