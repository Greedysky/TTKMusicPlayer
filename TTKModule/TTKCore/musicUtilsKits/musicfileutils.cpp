#include "musicfileutils.h"
#include "musicwidgetheaders.h"
#include "musicsettingmanager.h"

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
            ok = MusicUtils::File::removeRecursively(filePath, self); // recursive
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
    QString path = G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString();
    path = QFileDialog::getExistingDirectory(obj, QObject::tr("Choose a dir to open under"), path);
    if(!path.isEmpty())
    {
        G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, path);
    }
    return path;
}

QString MusicUtils::File::openFileDialog(QWidget *obj, const QString &filter)
{
    QString path = G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString();
    path = QFileDialog::getOpenFileName(obj, QObject::tr("Choose a filename to open under"), path, filter);
    if(!path.isEmpty())
    {
        G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, path);
    }
    return path;
}

QStringList MusicUtils::File::openFilesDialog(QWidget *obj, const QString &filter)
{
    QString path = G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString();
    const QStringList &files = QFileDialog::getOpenFileNames(obj, QObject::tr("Choose a filename to open under"), path, filter);
    if(!files.isEmpty())
    {
        const QString &v = files.front();
        if(!v.isEmpty())
        {
            G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, v);
        }
    }
    return files;
}

QString MusicUtils::File::saveFileDialog(QWidget *obj, const QString &filter)
{
    const QString &title = QObject::tr("Choose a filename to save under");
    QString path = G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString();
#if defined Q_OS_WIN || defined Q_OS_MAC
    path = QFileDialog::getSaveFileName(obj, title, path, filter);
    if(!path.isEmpty())
    {
        G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, path);
    }
    return path;
#else
    QFileDialog dialog(obj, title, path, filter);
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

    const QString &v = dialog.selectedFiles().front();
    if(!v.isEmpty())
    {
        G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, v);
    }
    return v;
#endif
}
