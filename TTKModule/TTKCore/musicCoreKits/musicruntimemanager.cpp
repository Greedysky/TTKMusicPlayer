#include "musicruntimemanager.h"
#include "musicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicnetworkthread.h"
#include "musicqmmputils.h"
#include "musicfileutils.h"
#include "musiccoreutils.h"
#include "musiccodecutils.h"

#include <QFont>
#include <QApplication>

void MusicRunTimeManager::run() const
{
    TTK_LOGGER_INFO("MusicApplication Run");
#ifndef TTK_GREATER_NEW
    MusicUtils::Codec::setLocalCodec();
#endif
    MusicUtils::QMMP::updateMidConfigFile();

#ifdef Q_OS_UNIX
    QFont font;
    font.setPixelSize(13);
    qApp->setFont(font);
#endif

    //detect the current network state
    G_NETWORK_PTR->start();

    MusicConfigManager xml;
    xml.readConfig();
    xml.readSysConfigData();

    MusicUtils::File::checkCacheSize(
                G_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt() * MH_MB2B,
                G_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimit).toInt(),
                G_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDir).toString());
    G_NETWORK_PTR->setBlockNetWork(
                G_SETTING_PTR->value(MusicSettingManager::CloseNetWork).toInt());
}

QString MusicRunTimeManager::translator() const
{
    const int index = G_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt();
    return MusicUtils::Core::getLanguageName(index);
}
