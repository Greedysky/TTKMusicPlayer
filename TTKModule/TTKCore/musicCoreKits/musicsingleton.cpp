#include "musicconnectionpool.h"
#include "musicbackgroundmanager.h"
#include "musicdispatchmanager.h"
#include "musichotkeymanager.h"
#include "musicsinglemanager.h"
#include "musicdownloadmanager.h"
#include "musicdownloadqueryfactory.h"

TTKDispatchManager* makeMusicDispatchManager()
{
    return TTKSingleton<TTKDispatchManager>::createInstance();
}

MusicConnectionPool* makeMusicConnectionPool()
{
    return TTKSingleton<MusicConnectionPool>::createInstance();
}

MusicBackgroundManager* makeMusicBackgroundManager()
{
    return TTKSingleton<MusicBackgroundManager>::createInstance();
}

MusicHotKeyManager* makeMusicHotKeyManager()
{
    return TTKSingleton<MusicHotKeyManager>::createInstance();
}

MusicSettingManager* makeMusicSettingManager()
{
    return TTKSingleton<MusicSettingManager>::createInstance();
}

MusicSingleManager* makeMusicSingleManager()
{
    return TTKSingleton<MusicSingleManager>::createInstance();
}

MusicDownLoadManager* makeMusicDownLoadManager()
{
    return TTKSingleton<MusicDownLoadManager>::createInstance();
}

MusicDownLoadQueryFactory* makeMusicDownLoadQueryFactory()
{
    return TTKSingleton<MusicDownLoadQueryFactory>::createInstance();
}

MusicNetworkThread* makeMusicNetworkThread()
{
    return TTKSingleton<MusicNetworkThread>::createInstance();
}
