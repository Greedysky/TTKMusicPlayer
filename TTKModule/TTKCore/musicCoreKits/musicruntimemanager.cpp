#include "musicruntimemanager.h"
#include "musicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicnetworkthread.h"
#include "musicqmmputils.h"
#include "ttkversion.h"

#include <QFont>
#include <QApplication>

namespace TTK
{
    /*!
     * Generate input directory size.
     */
    static quint64 directorySize(const QString &dirName);
    /*!
     * Check download cachce size.
     */
    static void checkCacheSize();
    /*!
     * Generate core language resource.
     */
    static QString languageCore(int index);
    /*!
     * Generate qmmp language resource.
     */
    static QString languageQmmp(int index);

}

quint64 TTK::directorySize(const QString &dirName)
{
    quint64 size = 0;
    if(QFileInfo(dirName).isDir())
    {
        const QDir dir(dirName);
        const QFileInfoList &fileList = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for(const QFileInfo &fin : qAsConst(fileList))
        {
            if(fin.isDir())
            {
                size += directorySize(fin.absoluteFilePath());
            }
            else
            {
                size += fin.size();
            }
        }
    }
    return size;
}

void TTK::checkCacheSize()
{
    const bool disabled = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheEnable).toInt();
    if(!disabled)
    {
        return;
    }

    const quint64 cacheSize = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt() * TTK_SN_MB2B;
    const QString &path = G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString();
    quint64 size = directorySize(path);
    if(size > cacheSize)
    {
        const QFileInfoList &fileList = QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(const QFileInfo &fin : qAsConst(fileList))
        {
            size -= fin.size();
            QFile::remove(fin.absoluteFilePath());
            if(size <= cacheSize)
            {
                break;
            }
        }
    }
}

QString TTK::languageCore(int index)
{
    QString lan(LANGUAGE_DIR_FULL);
    switch(index)
    {
        case 0: return lan.append("cn.ln");
        case 1: return lan.append("tc.ln");
        case 2: return lan.append("en.ln");
        default: return {};
    }
}

QString TTK::languageQmmp(int index)
{
    QString lan(LANGUAGE_DIR_FULL);
    switch(index)
    {
        case 0: return lan.append("cnq.ln");
        case 1: return lan.append("tcq.ln");
        case 2: return lan.append("enq.ln");
        default: return {};
    }
}


void MusicRunTimeManager::execute() const
{
    TTK_INFO_STREAM("MusicApplication Run");
    // read config
    MusicConfigManager manager;
    if(manager.fromFile(COFIG_PATH_FULL))
    {
        manager.readBuffer();
    }

    TTK::TTKQmmp::updateBaseConfig();

#ifdef Q_OS_WIN
    bool status = true;
    if(!QFile::exists(PLUGINS_DIR_FULL + "libeay32.dll") || !QFile::exists(PLUGINS_DIR_FULL + "ssleay32.dll"))
    {
        const QString &path = TTK::applicationPath();
        status &= QFile::copy(path + "libeay32.dll", PLUGINS_DIR_FULL + "libeay32.dll");
        status &= QFile::copy(path + "ssleay32.dll", PLUGINS_DIR_FULL + "ssleay32.dll");
    }
    TTK_INFO_STREAM("Install openssl library to plugin dir is" << (status ? "success" : "failed"));
#elif defined Q_OS_UNIX
    QFont font;
    font.setPixelSize(13);
    qApp->setFont(font);
#endif

    //detect the current network state
    G_NETWORK_PTR->start();

    TTK::checkCacheSize();
    G_NETWORK_PTR->setBlockNetwork(G_SETTING_PTR->value(MusicSettingManager::CloseNetWorkMode).toBool());
}

QStringList MusicRunTimeManager::translator() const
{
    const int index = G_SETTING_PTR->value(MusicSettingManager::LanguageIndex).toInt();
    return {TTK::languageCore(index), TTK::languageQmmp(index)};
}

bool MusicRunTimeManager::configVersionCheck() const noexcept
{
    return G_SETTING_PTR->value(MusicSettingManager::ConfigVersion).toString() == TTK_CONFIG_VERSION_STR;
}
