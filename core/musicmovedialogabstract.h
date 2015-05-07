#ifndef MUSICMOVEDIALOGABSTRACT_H
#define MUSICMOVEDIALOGABSTRACT_H

#include <QDialog>
#include <QMouseEvent>
#include <QBitmap>
#include <QPainter>
#include "musiclibexportglobal.h"

class MUSIC_EXPORT MusicMoveDialogAbstract : public QDialog
{
    Q_OBJECT
public:
    explicit MusicMoveDialogAbstract(QWidget *parent = 0);
    ~MusicMoveDialogAbstract();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_pressAt;
    bool m_leftButtonPress;

};

#endif // MUSICMOVEDIALOGABSTRACT_H
