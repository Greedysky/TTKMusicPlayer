#ifndef MUSICLRCFLOATWIDGET_H
#define MUSICLRCFLOATWIDGET_H

#include <QLabel>
#include "musicobject.h"
#include "musiclibexportglobal.h"

#define PHOTO_WIDTH     110
#define PHOTO_HEIGHT    65
#define PHOTO_PERLINE   3

class QPropertyAnimation;
class QPushButton;
class QCheckBox;

class MUSIC_EXPORT MusicLrcFloatPhotoPlaneWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatPhotoPlaneWidget(QWidget *parent = 0);

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *);

};



class MUSIC_EXPORT MusicLrcFloatPhotoWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatPhotoWidget(QWidget *parent = 0);
    ~MusicLrcFloatPhotoWidget();

public slots:
    void show();
    void confirmButtonClicked();
    void photoPrevious();
    void photoNext();
    void artHasChanged();
    void sendUserSelectArtBg1();
    void sendUserSelectArtBg2();
    void sendUserSelectArtBg3();
    void userSelectCheckBoxChecked1(bool b);
    void userSelectCheckBoxChecked2(bool b);
    void userSelectCheckBoxChecked3(bool b);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *){}
    virtual void mouseMoveEvent(QMouseEvent *){}
    virtual void mousePressEvent(QMouseEvent *){}
    virtual void mouseReleaseEvent(QMouseEvent *){}
    void showPhoto();

    QWidget* m_filmBGWidget;
    QPropertyAnimation *m_animation;
    QCheckBox* m_checkBox;
    QPushButton* m_previous,*m_next;
    QPushButton* m_confirmButton,*m_cancelButton;
    MusicLrcFloatPhotoPlaneWidget* m_plane1,*m_plane2,*m_plane3;
    QCheckBox* m_radio1,* m_radio2,* m_radio3;
    QStringList m_artPath;
    MIntSet m_selectNum;
    int m_currentIndex;

};



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
    MusicLrcFloatPhotoWidget* m_floatPhotoWidget;

};

#endif // MUSICLRCFLOATWIDGET_H
