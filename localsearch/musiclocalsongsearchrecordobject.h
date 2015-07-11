#ifndef MUSICLOCALSONGSEARCHRECORDOBJECT_H
#define MUSICLOCALSONGSEARCHRECORDOBJECT_H

#include "../core/musicxmlabstract.h"

class MUSIC_EXPORT MusicLocalSongSearchRecordObject : public MusicXmlAbstract
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchRecordObject(QObject *parent = 0);

    inline bool readSearchXMLConfig(){ return readConfig(MUSICSEARCH); }
    void writeSearchConfig(const QStringList &, const QStringList &);
    void readSearchConfig(QStringList&, QStringList &);

};

#endif // MUSICLOCALSONGSEARCHRECORDOBJECT_H
