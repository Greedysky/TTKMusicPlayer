#include "musicbackupmodule.h"
#include "musicapplication.h"
#include "musicsettingmanager.h"
#include "musictkplconfigmanager.h"
#include "musicsongssummariziedwidget.h"

MusicAbstractBackup::MusicAbstractBackup(int interval, QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(interval);
    connect(&m_timer, SIGNAL(timeout()), SLOT(runBackup()));
}

void MusicAbstractBackup::start()
{
    m_timer.start();
}

void MusicAbstractBackup::stop()
{
    m_timer.stop();
}


MusicPlaylistBackupModule::MusicPlaylistBackupModule(QObject *parent)
    : MusicAbstractBackup(1000 * 60 * 10 /*10 minutes*/, parent)
{

}

void MusicPlaylistBackupModule::runBackup()
{
    const QString &path = TTK_STR_CAT(APPBACKUP_DIR_FULL, "playlist");
    QDir().mkpath(path);

    MusicTKPLConfigManager manager;
    if(!manager.load(QString("%1/%2%3").arg(path).arg(TTKDateTime::currentTimestamp()).arg(TKF_FILE)))
    {
        return;
    }

    manager.writeBuffer(MusicApplication::instance()->m_songTreeWidget->musicItemList());
}


MusicBackupModule::MusicBackupModule()
{
    m_modules.push_back(new MusicPlaylistBackupModule);
}

MusicBackupModule::~MusicBackupModule()
{
    qDeleteAll(m_modules);
}

void MusicBackupModule::run()
{
    if(G_SETTING_PTR->value(MusicSettingManager::OtherPlaylistAutoSaveEnable).toBool())
    {
        m_modules[0]->start();
    }
    else
    {
        m_modules[0]->stop();
    }
}
