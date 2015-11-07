#ifndef MUSICCLICKEDLABEL_H
#define MUSICCLICKEDLABEL_H

#include <QLabel>
#include "musiclibexportglobal.h"

class MUSIC_LRC_EXPORT MusicClickedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MusicClickedLabel(QWidget *parent = 0);

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event);

};

#endif // MUSICCLICKEDLABEL_H
