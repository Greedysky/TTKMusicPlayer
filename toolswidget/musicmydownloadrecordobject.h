#ifndef MUSICMYDOWNLOADRECORDOBJECT_H
#define MUSICMYDOWNLOADRECORDOBJECT_H

#include <QObject>
#include "../core/musiclibexportglobal.h"
#include "../core/musicobject.h"

class QFile;
class QDomDocument;

class MUSIC_EXPORT MusicMyDownloadRecordObject : public QObject
{
public:
    explicit MusicMyDownloadRecordObject(QObject *parent = 0);
    ~MusicMyDownloadRecordObject();

    inline bool readDownloadXMLConfig(){ return readConfig(DOWNLOADINFO); }
    void writeDownloadConfig(const QStringList &, const QStringList &);
    void readDownloadConfig(QStringList&, QStringList &);

protected:
    bool readConfig(const QString& type);
    QFile* m_file;
    QDomDocument* m_ddom;

};

#endif // MUSICMYDOWNLOADRECORDOBJECT_H
