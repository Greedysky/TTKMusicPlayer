#ifndef MUSICMOVEWIDGETABSTRACT_H
#define MUSICMOVEWIDGETABSTRACT_H

#include <QWidget>
#include <QMouseEvent>
#include <QBitmap>
#include <QPainter>
#include "musiclibexportglobal.h"

class MUSIC_EXPORT MusicMoveWidgetAbstract : public QWidget
{
    Q_OBJECT
public:
    explicit MusicMoveWidgetAbstract(QWidget *parent = 0);
    ~MusicMoveWidgetAbstract();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_pressAt;
    bool m_leftButtonPress;

};

#endif // MUSICMOVEWIDGETABSTRACT_H
