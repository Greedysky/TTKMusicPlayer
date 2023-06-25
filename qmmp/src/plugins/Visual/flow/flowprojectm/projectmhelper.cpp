#include "projectmhelper.h"

QFileInfoList fileListByPath(const QString &dpath, const QStringList &filter)
{
    QDir dir(dpath);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden);
    const QFileInfoList& folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(const QFileInfo &fin : folderList)
    {
        fileList.append(fileListByPath(fin.absoluteFilePath(), filter));
    }

    return fileList;
}
