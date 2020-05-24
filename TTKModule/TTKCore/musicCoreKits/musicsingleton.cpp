#include "musicconnectionpool.h"
#include "musicbackgroundmanager.h"
#include "musicdispatchmanager.h"
#include "musichotkeymanager.h"
#include "musicsettingmanager.h"
#include "musicsinglemanager.h"
#include "musicdownloadmanager.h"
#include "musicdownloadqueryfactory.h"

MusicConnectionPool* GetMusicConnectionPool()
{
    return MusicSingleton<MusicConnectionPool>::createInstance();
}

MusicBackgroundManager* GetMusicBackgroundManager()
{
    return MusicSingleton<MusicBackgroundManager>::createInstance();
}

MusicDispatchManager* GetMusicDispatchManager()
{
    return MusicSingleton<MusicDispatchManager>::createInstance();
}

MusicHotKeyManager* GetMusicHotKeyManager()
{
    return MusicSingleton<MusicHotKeyManager>::createInstance();
}

MusicSettingManager* GetMusicSettingManager()
{
    return MusicSingleton<MusicSettingManager>::createInstance();
}

MusicSingleManager* GetMusicSingleManager()
{
    return MusicSingleton<MusicSingleManager>::createInstance();
}

MusicDownLoadManager* GetMusicDownLoadManager()
{
    return MusicSingleton<MusicDownLoadManager>::createInstance();
}

MusicDownLoadQueryFactory* GetMusicDownLoadQueryFactory()
{
    return MusicSingleton<MusicDownLoadQueryFactory>::createInstance();
}

MusicNetworkThread* GetMusicNetworkThread()
{
    return MusicSingleton<MusicNetworkThread>::createInstance();
}
