#ifndef MUSICREMOTEWIDGETFORCIRCLE_H
#define MUSICREMOTEWIDGETFORCIRCLE_H

#include "musicremotewidget.h"

class MUSIC_WIDGET_EXPORT MusicRemoteWidgetForCircle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForCircle(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent* event);

};

#endif // MUSICREMOTEWIDGETFORCIRCLE_H
