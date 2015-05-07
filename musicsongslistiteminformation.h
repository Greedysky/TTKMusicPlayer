#ifndef MUSICSONGSLISTITEM_H
#define MUSICSONGSLISTITEM_H

#include <QWidget>
#include "core/musicsonginformation.h"
#include "core/musiclibexportglobal.h"

namespace Ui {
class MusicSongsListItemInformation;
}

class MUSIC_EXPORT MusicSongsListItemInformation : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListItemInformation(QWidget *parent = 0);
    ~MusicSongsListItemInformation();

    void setMusicSongInformation(QMusicSong& );

protected:
    Ui::MusicSongsListItemInformation *ui;
    bool showArtPicture(const QString& name);

};

#endif // MUSICSONGSLISTITEM_H
