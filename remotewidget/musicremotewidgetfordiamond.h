#ifndef MUSICREMOTEWIDGETFORDIAMOND_H
#define MUSICREMOTEWIDGETFORDIAMOND_H

#include "musicremotewidget.h"

class MUSIC_WIDGET_EXPORT MusicRemoteWidgetForDiamond : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForDiamond(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent* event);

};

#endif // MUSICREMOTEWIDGETFORDIAMOND_H
