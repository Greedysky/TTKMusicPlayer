#include "musicbackupmodule.h"
#include "musicapplication.h"
#include "musicsettingmanager.h"
#include "musictkplconfigmanager.h"
#include "musicsongscontainerwidget.h"
#include "musicfileutils.h"

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
    const QString &root = APPBACKUP_DIR_FULL + "playlist";
    const QString &child = QDate::currentDate().toString(TTK_YEAR_FORMAT);

    QDir dir(root);
    dir.mkpath(child);

    const QFileInfoList &dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time | QDir::Reversed);
    if(dirList.count() > 7)
    {
        TTK::File::removeRecursively(dirList.front().absoluteFilePath());
    }

    dir.cd(child);

    MusicTKPLConfigManager manager;
    if(!manager.load(QString("%1/%2%3").arg(dir.absolutePath()).arg(TTKDateTime::currentTimestamp()).arg(TKF_FILE)))
    {
        return;
    }

    manager.writeBuffer(MusicApplication::instance()->m_songTreeWidget->items());

    const QFileInfoList &fileList = dir.entryInfoList(QDir::Files, QDir::Time | QDir::Reversed);
    if(fileList.count() > 7)
    {
        QFile::remove(fileList.front().absoluteFilePath());
    }
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
