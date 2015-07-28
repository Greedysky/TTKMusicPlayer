#ifndef MUSICLRCFLOATABSTRACTWIDGET_H
#define MUSICLRCFLOATABSTRACTWIDGET_H

#include <QLabel>
#include <QPropertyAnimation>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicLrcFloatAbstractWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatAbstractWidget(QWidget *parent = 0);
    ~MusicLrcFloatAbstractWidget();

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *){}
    virtual void mouseMoveEvent(QMouseEvent *){}
    virtual void mousePressEvent(QMouseEvent *){}
    virtual void mouseReleaseEvent(QMouseEvent *){}
    void animationIn();
    void animationOut();

    QPropertyAnimation *m_animation;
    QRect m_rectIn, m_rectOut;

};

#endif // MUSICLRCFLOATABSTRACTWIDGET_H
