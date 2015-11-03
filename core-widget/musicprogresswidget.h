#ifndef MUSICPROGRESSWIDGET_H
#define MUSICPROGRESSWIDGET_H

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicProgressWidget;
}

class MUSIC_WIDGET_EXPORT MusicProgressWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicProgressWidget(QWidget *parent = 0);
    ~MusicProgressWidget();

    void setTitle(const QString &name);
    void setValue(int value);
    void setRange(int min, int max);

public slots:
    virtual int exec();

protected:
    Ui::MusicProgressWidget *ui;

};

#endif // MUSICPROGRESSWIDGET_H
