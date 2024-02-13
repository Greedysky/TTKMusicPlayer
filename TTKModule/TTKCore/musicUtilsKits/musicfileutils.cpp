#include "musicfileutils.h"
#include "musicwidgetheaders.h"
#include "musicsettingmanager.h"

#include <QDirIterator>

QStringList TTK::File::fileListByPath(const QString &dpath, const QStringList &filter, bool recursively)
{
    QDir dir(dpath);
    if(TTK::Core::isBreakPointEnabled() || !dir.exists())
    {
        return {};
    }

    const QString &spr = dpath.endsWith(TTK_SEPARATOR) ? QString() : TTK_SEPARATOR;

    QStringList fileList;
    for(const QString &path : dir.entryList(filter, QDir::Files | QDir::Hidden))
    {
        fileList.append(dpath + spr + path);
    }

    if(recursively)
    {
        const QFileInfoList &folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(const QFileInfo &fin : qAsConst(folderList))
        {
            fileList.append(TTK::File::fileListByPath(fin.absoluteFilePath(), filter, recursively));
        }
    }
    return fileList;
}

QFileInfoList TTK::File::fileInfoListByPath(const QString &dpath, const QStringList &filter, bool recursively)
{
    QDir dir(dpath);
    if(TTK::Core::isBreakPointEnabled() || !dir.exists())
    {
        return {};
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden);
    if(recursively)
    {
        const QFileInfoList &folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for(const QFileInfo &fin : qAsConst(folderList))
        {
            fileList.append(TTK::File::fileInfoListByPath(fin.absoluteFilePath(), filter, recursively));
        }
    }
    return fileList;
}

bool TTK::File::copyPath(const QString &srcPath, const QString &dstPath, bool overwrite)
{
    if(TTK::Core::isBreakPointEnabled())
    {
        return false;
    }

    if(QFileInfo(srcPath).isFile())
    {
        if(QFileInfo(dstPath).isFile())
        {
            if(overwrite && QFile::exists(dstPath))
            {
                QFile::remove(dstPath);
            }
            return QFile::copy(srcPath, dstPath);
        }
        return false;
    }

    QDir dstDir(dstPath);
    if(!dstDir.exists() && !dstDir.mkpath(dstDir.absolutePath()))
    {
        return false;
    }

    for(const QFileInfo &fileInfo : QDir(srcPath).entryInfoList())
    {
        const QString &fileName = fileInfo.fileName();
        if(fileName == "." || fileName == "..")
        {
            continue;
        }

        if(fileInfo.isDir())
        {
            if(!TTK::File::copyPath(fileInfo.filePath(), dstDir.filePath(fileName), overwrite))
            {
                return false;
            }
        }
        else
        {
            if(overwrite && dstDir.exists(fileName))
            {
                dstDir.remove(fileName);
            }

            if(!QFile::copy(fileInfo.filePath(), dstDir.filePath(fileName)))
            {
                return false;
            }
        }
    }
    return true;
}

bool TTK::File::removeRecursively(const QString &dir, bool self)
{
    QDir dr(dir);
    if(TTK::Core::isBreakPointEnabled() || !dr.exists())
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

        bool ok = false;
        if(fin.isDir() && !fin.isSymLink())
        {
            ok = TTK::File::removeRecursively(filePath, self); // recursive
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

QString TTK::File::getExistingDirectory(QWidget *parent)
{
    QString path = G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString();
    path = QFileDialog::getExistingDirectory(parent, QObject::tr("Choose a dir to open under"), path);
    if(!path.isEmpty())
    {
        path += TTK_SEPARATOR;
        G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, path);
    }
    return path;
}

QString TTK::File::getOpenFileName(QWidget *parent, const QString &filter)
{
    QString path = QFileInfo(G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString()).absolutePath();
    path = QFileDialog::getOpenFileName(parent, QObject::tr("Choose a filename to open under"), path, filter);
    if(!path.isEmpty())
    {
        G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, path);
    }
    return path;
}

QStringList TTK::File::getOpenFileNames(QWidget *parent, const QString &filter)
{
    const QString &path = QFileInfo(G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString()).absolutePath();
    const QStringList &files = QFileDialog::getOpenFileNames(parent, QObject::tr("Choose a filename to open under"), path, filter);
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

QString TTK::File::getSaveFileName(QWidget *parent, const QString &filter)
{
    const QString &title = QObject::tr("Choose a filename to save under");
    QString path = G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString();
#if defined Q_OS_WIN || defined Q_OS_MAC
    path = QFileDialog::getSaveFileName(parent, title, path, filter);
#else
    QFileDialog dialog(parent, title, QFileInfo(path).absolutePath(), filter);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if(parent)
    {
        dialog.setWindowModality(Qt::WindowModal);
    }

    if(dialog.exec() != QDialog::Accepted)
    {
        return {};
    }

    const QString &selectFilter = dialog.selectedNameFilter();
    const QStringList &filters = filter.split(";;");

    if(!filters.isEmpty())
    {
        const QRegExp regx("(?:^\\*\\.(?!.*\\()|\\(\\*\\.)(\\w+)");
        if(regx.indexIn(selectFilter) != -1)
        {
            dialog.setDefaultSuffix(regx.cap(1));
        }
    }

    const QStringList &files = dialog.selectedFiles();
    if(files.isEmpty())
    {
        return {};
    }

    path = files.front();
    const QString &suffix = dialog.defaultSuffix();
    if(!suffix.isEmpty() && suffix != QFileInfo(path).suffix())
    {
        path += "." + suffix;
    }
#endif
    if(!path.isEmpty())
    {
        G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, path);
    }
    return path;
}
