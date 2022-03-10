#include "musicfileutils.h"
#include "musicwidgetheaders.h"

#include <QDirIterator>

QFileInfoList MusicUtils::File::fileListByPath(const QString &dpath, const QStringList &filter, bool recursively)
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
        for(const QFileInfo &fin : qAsConst(folderList))
        {
            fileList.append(fileListByPath(fin.absoluteFilePath(), filter, recursively));
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
        const QFileInfo &fin = di.fileInfo();
        const QString &filePath = di.filePath();

        bool ok;
        if(fin.isDir() && !fin.isSymLink())
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

QString MusicUtils::File::openDirectoryDialog(QWidget *obj)
{
    return QFileDialog::getExistingDirectory(obj, QString(), "./");
}

QString MusicUtils::File::openFileDialog(QWidget *obj, const QString &filter)
{
    return QFileDialog::getOpenFileName(obj, QObject::tr("Choose a filename to open under"), QDir::currentPath(), filter);
}

QStringList MusicUtils::File::openFilesDialog(QWidget *obj, const QString &filter)
{
    return QFileDialog::getOpenFileNames(obj, QObject::tr("Choose a filename to open under"), QDir::currentPath(), filter);
}

QString MusicUtils::File::saveFileDialog(QWidget *obj, const QString &filter)
{
    const QString &title = QObject::tr("Choose a filename to save under");
#if defined Q_OS_WIN || defined Q_OS_MAC
    return QFileDialog::getSaveFileName(obj, title, QDir::currentPath(), filter);
#else
    QFileDialog dialog(obj, title, QDir::currentPath(), filter);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if(obj)
    {
        dialog.setWindowModality(Qt::WindowModal);
    }

    if(dialog.exec() != QDialog::Accepted)
    {
        return QString();
    }

    const QString &selectFilter = dialog.selectedNameFilter();
    const QStringList &filters = filter.split(";;");
    if(!filters.isEmpty())
    {
        QRegExp regex("(?:^\\*\\.(?!.*\\()|\\(\\*\\.)(\\w+)");
        if(regex.indexIn(selectFilter) != -1)
        {
            dialog.setDefaultSuffix(regex.cap(1));
        }
    }
    return dialog.selectedFiles().front();
#endif
}
