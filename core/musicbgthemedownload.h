#ifndef MUSICBGTHEMESLIDE_H
#define MUSICBGTHEMESLIDE_H

#include <QObject>
#include "musicobject.h"
#include "musiclibexportglobal.h"

#define TMP_DOWNLOAD "tmp"

class MUSIC_EXPORT MusicBgThemeDownload : public QObject
{
    Q_OBJECT
public:
    explicit MusicBgThemeDownload(const QString &name,
                                  QObject *parent = 0);

public slots:
    void downLoadFinished(const QString &);

protected:
    QString m_artName;

};

#endif // MUSICBGTHEMESLIDE_H
