#include "musicruntimemanager.h"
#include "musicsysconfigmanager.h"
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
    M_NETWORK_PTR->start();

    MusicSysConfigManager *xml = new MusicSysConfigManager;
    xml->readConfig();
    xml->readSysConfigData();

    MusicUtils::File::checkCacheSize(
                M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt() * MH_MB2B,
                M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimit).toInt(),
                M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDir).toString());
    M_NETWORK_PTR->setBlockNetWork(
                M_SETTING_PTR->value(MusicSettingManager::CloseNetWork).toInt());
    delete xml;
}

QString MusicRunTimeManager::translator() const
{
    const int index = M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt();
    return MusicUtils::Core::getLanguageName(index);
}
