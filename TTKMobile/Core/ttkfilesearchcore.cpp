#include "ttkfilesearchcore.h"
#include <QApplication>
#include <QDir>
#if defined (Q_OS_ANDROID)
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#endif

TTKFileSearchCore::TTKFileSearchCore(QObject *parent)
    : QObject(parent)
{

}

QString TTKFileSearchCore::getRoot() const
{
#if defined (Q_OS_ANDROID)
    QAndroidJniObject mediaDir = QAndroidJniObject::callStaticObjectMethod("android/os/Environment",
                                                                           "getExternalStorageDirectory",
                                                                           "()Ljava/io/File;");
    QAndroidJniObject mediaPath = mediaDir.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );
    return mediaPath.toString();
#else
    return QApplication::applicationDirPath() + "/";
#endif
}

void TTKFileSearchCore::search(const QStringList &path)
{

    QFileInfoList l;
    foreach(QString var, path)
    {
        l << findFile(var.remove("file:///"), QStringList());
    }

    QStringList pathes;
    foreach(const QFileInfo &var, l)
    {
        pathes << ("file:///" + var.absoluteFilePath());
    }
    emit finished(pathes);
}

void TTKFileSearchCore::search(const QString &path)
{
    search(QStringList(path));
}

QFileInfoList TTKFileSearchCore::findFile(const QString &path, const QStringList &filter)
{
    QDir dir(path);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(const QFileInfo &folder, folderList)
    {
        fileList.append( findFile(folder.absoluteFilePath(), filter) );
    }
    return fileList;
}
