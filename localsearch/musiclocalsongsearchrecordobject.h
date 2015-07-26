#ifndef MUSICLOCALSONGSEARCHRECORDOBJECT_H
#define MUSICLOCALSONGSEARCHRECORDOBJECT_H

#include "musicxmlabstract.h"

class MUSIC_CORE_EXPORT MusicLocalSongSearchRecordObject : public MusicXmlAbstract
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchRecordObject(QObject *parent = 0);

    inline bool readSearchXMLConfig(){ return readConfig(MUSICSEARCH); }
    void writeSearchConfig(const QStringList &, const QStringList &);
    void readSearchConfig(QStringList&, QStringList &);

};

#endif // MUSICLOCALSONGSEARCHRECORDOBJECT_H
