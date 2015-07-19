#ifndef MUSICLRCARTPHOTOLABEL_H
#define MUSICLRCARTPHOTOLABEL_H

#include <QLabel>
#define PIX_HEIGHT 16
#define PIX_WIDTH  16

class MusicLrcArtPhotoLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLrcArtPhotoLabel(QWidget *parent = 0);

signals:
public slots:
    virtual void paintEvent(QPaintEvent *event);

};

#endif // MUSICLRCARTPHOTOLABEL_H
