#include "ttkmusicutils.h"
#include "musicobject.h"
#include "musicsourceupdatethread.h"
#include "musicsemaphoreloop.h"
#include "musicnetworkthread.h"

#include <QMessageBox>
#include <QFontMetrics>

TTKMusicUtils::TTKMusicUtils(QObject *parent)
    : QObject(parent)
{

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

void TTKMusicUtils::updateApplicationDialog()
{
#if defined (Q_OS_ANDROID)

    if(!M_NETWORK_PTR->isOnline())
    {
        return;
    }

    MusicSemaphoreLoop loop;
    MusicSourceUpdateThread *download = new MusicSourceUpdateThread(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), &loop, SLOT(quit()));
    download->startToDownload();
    loop.exec();

    if(!download->isLastedVersion())
    {
        if(!M_NETWORK_PTR->isOnline())
        {
            return;
        }

        QAndroidJniObject::callStaticMethod<void>(APP_PKG_NAME, "updateApplicationDialog", "(Ljava/lang/String;)V",
        QAndroidJniObject::fromString( download->getLastedVersion() ).object<jstring>());
    }
    else
    {
        showMessageBox(tr("Current version is updated!"));
    }
#endif
}

int TTKMusicUtils::stringFontWidth(const QFont &font, const QString &str)
{
    return QFontMetrics(font).width(str);
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

    dirIsExist(MUSIC_DIR_FULL);

    dirIsExist(LANGUAGE_DIR_FULL);
}
