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
    return TTKSingleton<MusicConnectionPool>::createInstance();
}

MusicBackgroundManager* GetMusicBackgroundManager()
{
    return TTKSingleton<MusicBackgroundManager>::createInstance();
}

MusicDispatchManager* GetMusicDispatchManager()
{
    return TTKSingleton<MusicDispatchManager>::createInstance();
}

MusicHotKeyManager* GetMusicHotKeyManager()
{
    return TTKSingleton<MusicHotKeyManager>::createInstance();
}

MusicSettingManager* GetMusicSettingManager()
{
    return TTKSingleton<MusicSettingManager>::createInstance();
}

MusicSingleManager* GetMusicSingleManager()
{
    return TTKSingleton<MusicSingleManager>::createInstance();
}

MusicDownLoadManager* GetMusicDownLoadManager()
{
    return TTKSingleton<MusicDownLoadManager>::createInstance();
}

MusicDownLoadQueryFactory* GetMusicDownLoadQueryFactory()
{
    return TTKSingleton<MusicDownLoadQueryFactory>::createInstance();
}

MusicNetworkThread* GetMusicNetworkThread()
{
    return TTKSingleton<MusicNetworkThread>::createInstance();
}
