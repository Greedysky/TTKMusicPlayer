#ifndef MUSICLRCARTPHOTOUPLOAD_H
#define MUSICLRCARTPHOTOUPLOAD_H

#include "musicmovedialogabstract.h"

namespace Ui {
class MusicLrcArtPhotoUpload;
}

class MUSIC_EXPORT MusicLrcArtPhotoUpload : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcArtPhotoUpload(QWidget *parent = 0);

signals:
public slots:
    void selectButtonClicked();
    void uploadButtonClicked();

protected:
    Ui::MusicLrcArtPhotoUpload *ui;

};

#endif // MUSICLRCARTPHOTOUPLOAD_H
