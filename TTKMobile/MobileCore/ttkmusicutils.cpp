#include "ttkmusicutils.h"
#include "musicobject.h"
#include "musictime.h"
#include "musicversion.h"
#include "musicsongtag.h"
#include "musicnumberutils.h"
#include "musicsettingmanager.h"
#include "musicsourceupdatethread.h"
#include "musicsemaphoreloop.h"
#include "musicnetworkthread.h"

#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QFontMetrics>

TTKMusicUtils::TTKMusicUtils(QObject *parent)
    : QObject(parent)
{
    m_songTag = nullptr;
}

TTKMusicUtils::~TTKMusicUtils()
{
    delete m_songTag;
}

QVariant TTKMusicUtils::getValue(const QString &key) const
{
    return M_SETTING_PTR->value(key);
}

void TTKMusicUtils::setValue(const QString &key, const QVariant &value) const
{
    M_SETTING_PTR->setValue(key, value);
}

QString TTKMusicUtils::getRoot() const
{
    return MusicObject::getAppDir();
}

QString TTKMusicUtils::getRootPath() const
{
    QString path = getRoot();
#if defined (Q_OS_ANDROID)
    path.remove("TTKMobile/");
#endif
    return "file://" + path;
}

QString TTKMusicUtils::getCachedPath() const
{
    return CACHE_DIR_FULL;
}

void TTKMusicUtils::showWindowNotify(int value)
{
#if defined (Q_OS_ANDROID)
    QAndroidJniObject::callStaticMethod<void>(APP_PKG_NAME,
              "notify", "(Ljava/lang/String;Ljava/lang/String;I)V",
              QAndroidJniObject::fromString( QString() ).object<jstring>(),
              QAndroidJniObject::fromString( QString() ).object<jstring>(),
              static_cast<jint>(value));
#else
    Q_UNUSED(value);
#endif
}

void TTKMusicUtils::showWindowNotify(const QString &title, const QString &text, int value)
{
#if defined (Q_OS_ANDROID)
    QAndroidJniObject::callStaticMethod<void>(APP_PKG_NAME,
              "notify", "(Ljava/lang/String;Ljava/lang/String;I)V",
              QAndroidJniObject::fromString( title ).object<jstring>(),
              QAndroidJniObject::fromString( text ).object<jstring>(),
              static_cast<jint>(value));
#else
    Q_UNUSED(title);
    Q_UNUSED(text);
    Q_UNUSED(value);
#endif
}

void TTKMusicUtils::showMessageBox(const QString &text, const QString &title, QWidget *parent)
{
#if defined Q_OS_WIN
    QMessageBox::information(parent, title, text);
#elif defined (Q_OS_ANDROID)
    Q_UNUSED(title);
    Q_UNUSED(parent);
    QAndroidJniObject t = QAndroidJniObject::fromString(text);
    QAndroidJniObject::callStaticMethod<void>(APP_PKG_NAME, "showMessageBox", "(Ljava/lang/String;)V",
                                              t.object<jstring>());
#endif
}

bool TTKMusicUtils::currentNetIsWifi()
{
#if defined (Q_OS_ANDROID)
    int wifi = QAndroidJniObject::callStaticMethod<jint>(APP_PKG_NAME, "currentNetIsWifi", "()I");
    return (wifi == 0);
#endif
    return false;
}

void TTKMusicUtils::setNetworkBlockNotWifi()
{
#if defined (Q_OS_ANDROID)
    bool block = M_SETTING_PTR->value(MusicSettingManager::MobileWifiConnectChoiced).toBool() && currentNetIsWifi();
    M_NETWORK_PTR->setBlockNetWork(block);
#endif
}

void TTKMusicUtils::updateApplicationDialog()
{
#if defined (Q_OS_ANDROID)
    MusicSemaphoreLoop loop;
    MusicSourceUpdateThread *download = new MusicSourceUpdateThread(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), &loop, SLOT(quit()));
    download->startToDownload();
    loop.exec();

    if(!download->isLastedVersion())
    {
        QAndroidJniObject::callStaticMethod<void>(APP_PKG_NAME, "updateApplicationDialog", "(Ljava/lang/String;)V",
        QAndroidJniObject::fromString( download->getLastedVersion() ).object<jstring>());
    }
    else
    {
        showMessageBox(tr("Current version is updated!"));
    }
#endif
}

QString TTKMusicUtils::normalizeTime(qint64 time, const QString &format)
{
    return MusicTime(time, MusicTime::All_Msec).toString(format);
}

QString TTKMusicUtils::fromMSecsSinceEpoch(qint64 msecs, const QString &format)
{
    return QDateTime::fromMSecsSinceEpoch(msecs).toString(format);
}

QString TTKMusicUtils::size2Label(qint64 size)
{
    return MusicUtils::Number::size2Label(size);
}

int TTKMusicUtils::stringFontWidth(const QFont &font, const QString &str)
{
    return QFontMetrics(font).width(str);
}

bool TTKMusicUtils::readTagFromFile(const QString &path)
{
    delete m_songTag;
    m_songTag = new MusicSongTag;
    return m_songTag->readFile(path);
}

QString TTKMusicUtils::getTag(int index)
{
    QString value;
    switch(index)
    {
        case 0: value = m_songTag->getAlbum(); break;
        case 1: value = QFileInfo(m_songTag->getFilePath()).suffix(); break;
        case 2: value = m_songTag->getBitrate(); break;
        case 3: value = m_songTag->getSamplingRate(); break;
        case 4: value = m_songTag->getYear(); break;
        case 5: value = size2Label(QFileInfo(m_songTag->getFilePath()).size()); break;
        case 6: value = m_songTag->getLengthString(); break;
        case 7: value = m_songTag->getFilePath(); break;
    }
    return value.isEmpty() ? "--" : value;
}

void TTKMusicUtils::closeTagFromFile()
{
    delete m_songTag;
    m_songTag = nullptr;
}

bool TTKMusicUtils::removeCacheDir(const QString &dir)
{
    QDir d(dir);
    if(d.exists() && d.removeRecursively())
    {
        d.mkpath(CACHE_DIR_FULL);
        return true;
    }
    return false;
}

bool TTKMusicUtils::removeDir(const QString &dir)
{
    QDir d(dir);
    return (d.exists() && d.removeRecursively());
}

void TTKMusicUtils::checkTheFileNeededExist()
{
    checkTheDirectoryExist();
    if(!QFile::exists(COFIGPATH_FULL))
    {
        QFile(":/extra/musicconfig").copy(COFIGPATH_FULL);
        QFile::setPermissions(COFIGPATH_FULL, QFile::WriteOwner | QFile::ReadOwner);
    }

    if(!QFile::exists(MUSICPATH_FULL))
    {
        QFile(":/extra/music").copy(MUSICPATH_FULL);
        QFile::setPermissions(MUSICPATH_FULL, QFile::WriteOwner | QFile::ReadOwner);
    }

    QFile(":/extra/TTKMobile").copy(LANGUAGE_DIR_FULL + "TTKMobile.ln");
    QFile::setPermissions(LANGUAGE_DIR_FULL + "TTKMobile.ln", QFile::WriteOwner | QFile::ReadOwner);
}

void TTKMusicUtils::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkdir(name);
    }
}

void TTKMusicUtils::checkTheDirectoryExist()
{
    dirIsExist(APPDATA_DIR_FULL);
    dirIsExist(DOWNLOADS_DIR_FULL);

    dirIsExist(LRC_DIR_FULL);
    dirIsExist(MUSIC_DIR_FULL);
    dirIsExist(MOVIE_DIR_FULL);
    dirIsExist(CACHE_DIR_FULL);
    dirIsExist(ART_DIR_FULL);
    dirIsExist(BACKGROUND_DIR_FULL);

    dirIsExist(LANGUAGE_DIR_FULL);
}
