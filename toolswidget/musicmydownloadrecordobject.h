#ifndef MUSICMYDOWNLOADRECORDOBJECT_H
#define MUSICMYDOWNLOADRECORDOBJECT_H

#include "musicxmlabstract.h"

class MUSIC_CORE_EXPORT MusicMyDownloadRecordObject : public MusicXmlAbstract
{
public:
    explicit MusicMyDownloadRecordObject(QObject *parent = 0);

    inline bool readDownloadXMLConfig(){ return readConfig(DOWNLOADINFO); }
    void writeDownloadConfig(const QStringList &, const QStringList &);
    void readDownloadConfig(QStringList&, QStringList &);

};

#endif // MUSICMYDOWNLOADRECORDOBJECT_H
