#ifndef MUSICLRCFLOATWIDGET_H
#define MUSICLRCFLOATWIDGET_H

#include <QLabel>
#include "musiclibexportglobal.h"

class QPropertyAnimation;
class QPushButton;

class MUSIC_EXPORT MusicLrcFloatWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatWidget(QWidget *parent = 0);
    ~MusicLrcFloatWidget();

signals:
public slots:

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *){}
    virtual void mouseMoveEvent(QMouseEvent *){}
    virtual void mousePressEvent(QMouseEvent *){}
    virtual void mouseReleaseEvent(QMouseEvent *){}

    int m_posX, m_posY, m_valueX;
    QPropertyAnimation *m_animation;
    QPushButton *m_more,*m_update,*m_search;
    QPushButton *m_wallp,*m_photo;

};

#endif // MUSICLRCFLOATWIDGET_H
