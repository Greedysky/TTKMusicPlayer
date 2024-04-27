#include "musicconnectionpool.h"
#include "musicbackgroundmanager.h"
#include "musicdispatchmanager.h"
#include "musichotkeymanager.h"
#include "musicsinglemanager.h"
#include "musicdownloadmanager.h"
#include "musicdownloadqueryfactory.h"

TTKDispatchManager* makeMusicDispatchManager()
{
    return TTKSingleton<TTKDispatchManager>::instance();
}

MusicConnectionPool* makeMusicConnectionPool()
{
    return TTKSingleton<MusicConnectionPool>::instance();
}

MusicBackgroundManager* makeMusicBackgroundManager()
{
    return TTKSingleton<MusicBackgroundManager>::instance();
}

MusicHotKeyManager* makeMusicHotKeyManager()
{
    return TTKSingleton<MusicHotKeyManager>::instance();
}

MusicSettingManager* makeMusicSettingManager()
{
    return TTKSingleton<MusicSettingManager>::instance();
}

MusicSingleManager* makeMusicSingleManager()
{
    return TTKSingleton<MusicSingleManager>::instance();
}

MusicDownLoadManager* makeMusicDownLoadManager()
{
    return TTKSingleton<MusicDownLoadManager>::instance();
}

MusicDownLoadQueryFactory* makeMusicDownLoadQueryFactory()
{
    return TTKSingleton<MusicDownLoadQueryFactory>::instance();
}

MusicNetworkThread* makeMusicNetworkThread()
{
    return TTKSingleton<MusicNetworkThread>::instance();
}
