#ifndef MUSICFILEINFORMATION_H
#define MUSICFILEINFORMATION_H

#include <QObject>
#include "musicplayer.h"

class MUSIC_EXPORT MusicFileInformation
{
public:
    MusicFileInformation();
    ~MusicFileInformation();

    bool readFile(const QString &file);
    /////////////////////////////////////////////
    QString getArtist() const;
    QString getTitle() const;
    QString getAlbum() const;
    QString getComment() const;
    QString getYear() const;
    QString getTrackNum() const;
    QString getGenre() const;
    QString getAlbumArtist() const;
    QString getComposer() const;
    QString getOriginalArtist() const;
    QString getCopyright() const;
    QString getURL() const;
    QString getEncoder() const;
    QString getPublisher() const;
    uint getBPM() const;
    TID3PictureW getPicture() const;
    /////////////////////////////////////////////
    int getSamplingRate() const;
    int getChannelNumber() const;
    int getVBR() const;
    QString getVBRString() const;
    int getBitrate() const;
    TStreamTime getLength() const;
    QString getLengthString() const;
    QString getDescription() const;
    /////////////////////////////////////////////
protected:
    ZPlay *m_player;
    TID3InfoExW m_tagInfo;
    TStreamInfoW m_pInfo;

};

#endif // MUSICFILEINFORMATION_H
