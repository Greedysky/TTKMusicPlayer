#ifndef MUSICLRCMAKERWIDGET_H
#define MUSICLRCMAKERWIDGET_H

#include "musicmovewidgetabstract.h"

namespace Ui {
class MusicLrcMakerWidget;
}

class MUSIC_WIDGET_EXPORT MusicLrcMakerWidget : public MusicMoveWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcMakerWidget(QWidget *parent = 0);
    void setCurrentSongName(const QString& name);

signals:
public slots:
    void makeButtonClicked();
    void saveButtonClicked();
    void reviewButtonClicked();

protected:
    virtual void keyPressEvent(QKeyEvent*);
    virtual void keyReleaseEvent(QKeyEvent*);
    void setControlEnable(bool);
    Ui::MusicLrcMakerWidget *ui;

};

#endif // MUSICLRCMAKERWIDGET_H
