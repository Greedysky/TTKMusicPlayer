#ifndef MUSICLRCMAKERWIDGET_H
#define MUSICLRCMAKERWIDGET_H

#include "musicmovedialogabstract.h"

namespace Ui {
class MusicLrcMakerWidget;
}

class MUSIC_WIDGET_EXPORT MusicLrcMakerWidget : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcMakerWidget(QWidget *parent = 0);

signals:
public slots:

protected:
    Ui::MusicLrcMakerWidget *ui;

};

#endif // MUSICLRCMAKERWIDGET_H
