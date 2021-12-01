#include "musicfileutils.h"
#include "musicwidgetheaders.h"

#include <QDirIterator>

QFileInfoList MusicUtils::File::getFileListByDir(const QString &dpath, bool recursively)
{
    return getFileListByDir(dpath, QStringList(), recursively);
}

QFileInfoList MusicUtils::File::getFileListByDir(const QString &dpath, const QStringList &filter, bool recursively)
{
    QDir dir(dpath);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden);
    if(recursively)
    {
        const QFileInfoList& folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(const QFileInfo &fileInfo : qAsConst(folderList))
        {
            fileList.append(getFileListByDir(fileInfo.absoluteFilePath(), filter, recursively));
        }
    }

    return fileList;
}

bool MusicUtils::File::removeRecursively(const QString &dir, bool self)
{
    QDir dr(dir);
    if(!dr.exists())
    {
        return true;
    }

    bool success = true;
    const QString &dirPath = dr.path();
    // not empty -- we must empty it first
    QDirIterator di(dirPath, QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    while(di.hasNext())
    {
        di.next();
        const QFileInfo &fi = di.fileInfo();
        const QString &filePath = di.filePath();

        bool ok;
        if(fi.isDir() && !fi.isSymLink())
        {
            ok = MusicUtils::File::removeRecursively(filePath); // recursive
        }
        else
        {
            ok = QFile::remove(filePath);
            if(!ok)
            {
                // Read-only files prevent directory deletion on Windows, retry with Write permission.
                const QFile::Permissions permissions = QFile::permissions(filePath);
                if(!(permissions & QFile::WriteUser))
                {
                    ok = QFile::setPermissions(filePath, permissions | QFile::WriteUser) && QFile::remove(filePath);
                }
            }
        }

        if(!ok)
        {
            success = false;
        }
    }

    if(success && self)
    {
        success = dr.rmdir(dr.absolutePath());
    }

    return success;
}

QString MusicUtils::File::getOpenDirectoryDialog(QWidget *obj)
{
    return QFileDialog::getExistingDirectory(obj, QString(), "./");
}

QString MusicUtils::File::getOpenFileDialog(QWidget *obj, const QString &filter)
{
    return QFileDialog::getOpenFileName(obj, QObject::tr("Choose a filename to open under"), QDir::currentPath(), filter);
}

QStringList MusicUtils::File::getOpenFilesDialog(QWidget *obj, const QString &filter)
{
    return QFileDialog::getOpenFileNames(obj, QObject::tr("Choose a filename to open under"), QDir::currentPath(), filter);
}

QString MusicUtils::File::getSaveFileDialog(QWidget *obj, const QString &filter)
{
    return QFileDialog::getSaveFileName(obj,  QObject::tr("Choose a filename to save under"), QDir::currentPath(), filter);
}
