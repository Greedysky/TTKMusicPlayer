#ifndef MUSICBGTHEMESLIDE_H
#define MUSICBGTHEMESLIDE_H

#include <QObject>
#include "musicobject.h"
#include "musiclibexportglobal.h"

#define TMP_DOWNLOAD "tmp"

class MUSIC_CORE_EXPORT MusicBgThemeDownload : public QObject
{
    Q_OBJECT
public:
    explicit MusicBgThemeDownload(const QString &name, const QString &save,
                                  QObject *parent = 0);
signals:
    void musicBgDownloadFinished();

public slots:
    void downLoadFinished(const QString &name);
    void bgDownLoadFinished(const QString &name);

protected:
    QString m_artName;
    QString m_savePath;
    int m_index;
    int m_counter;

};

#endif // MUSICBGTHEMESLIDE_H
