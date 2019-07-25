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
    M_LOGGER_INFO("MusicApplication Begin");

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

    M_LOGGER_INFO("Load Translation");
    MusicSysConfigManager *xml = new MusicSysConfigManager;
    xml->readConfig();
    xml->readSysConfigData();

    MusicUtils::File::checkCacheSize(
                M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt()*MH_MB2B,
                M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt(),
                M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString());
    M_NETWORK_PTR->setBlockNetWork(
                M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toInt());
    delete xml;

    M_LOGGER_INFO("End load translation");
}

QString MusicRunTimeManager::translator() const
{
    const int index = M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt();
    return MusicUtils::Core::getLanguageName(index);
}
