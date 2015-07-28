#ifndef MUSICLRCFLOATSETTINGWIDGET_H
#define MUSICLRCFLOATSETTINGWIDGET_H

#include "musiclrcfloatabstractwidget.h"

class MUSIC_WIDGET_EXPORT MusicLrcFloatSettingWidget : public MusicLrcFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatSettingWidget(QWidget *parent = 0);

signals:
public slots:
    void show();

protected:
    virtual void enterEvent(QEvent *){}
    virtual void leaveEvent(QEvent *);

};

#endif // MUSICLRCFLOATSETTINGWIDGET_H
