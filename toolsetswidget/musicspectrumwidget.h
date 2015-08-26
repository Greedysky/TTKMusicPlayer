#ifndef MUSICSPECTRUMWIDGET_H
#define MUSICSPECTRUMWIDGET_H

#define PADDING 10

#include <QTimer>
#include "musicmovewidgetabstract.h"

class MUSIC_WIDGET_EXPORT  MusicSpectrumWidget : public MusicMoveWidgetAbstract
{
    Q_OBJECT
    enum Direction{
        UP,
        DOWN,
        LEFT,
        RIGHT,
        LEFTTOP,
        LEFTBOTTOM,
        RIGHTBOTTOM,
        RIGHTTOP,
        NONE
    };
public:
    explicit MusicSpectrumWidget(QWidget *parent = 0);
    ~MusicSpectrumWidget();

signals:
    void setSpectrum(HWND wnd, int w, int h, int x = 0, int y = 0);

public slots:
    void show();

private slots:
    void timeout();
    void close();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    void region(const QPoint &cursorPoint);
    QPoint m_dragPosition;
    Direction m_direction;
    QTimer m_timer;

};

#endif // MUSICSPECTRUMWIDGET_H
