#ifndef MUSICLRCFLOATPHOTOWIDGET_H
#define MUSICLRCFLOATPHOTOWIDGET_H

#include <QWidget>
#include "musiclrcfloatabstractwidget.h"

#define PHOTO_WIDTH     110
#define PHOTO_HEIGHT    65
#define PHOTO_PERLINE   3

class QPushButton;
class QCheckBox;

class MUSIC_WIDGET_EXPORT MusicLrcFloatPhotoPlaneWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatPhotoPlaneWidget(QWidget *parent = 0);

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *);

};



class MUSIC_WIDGET_EXPORT MusicLrcFloatPhotoWidget : public MusicLrcFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatPhotoWidget(QWidget *parent = 0);
    ~MusicLrcFloatPhotoWidget();

public slots:
    void show();
    void close();
    void parentClose() { QLabel::close(); }
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
    virtual void enterEvent(QEvent *){}
    virtual void leaveEvent(QEvent *){}
    void showPhoto();

    QWidget* m_filmBGWidget;
    QCheckBox* m_checkBox;
    QPushButton* m_previous,*m_next;
    QPushButton* m_confirmButton,*m_cancelButton;
    MusicLrcFloatPhotoPlaneWidget* m_plane1,*m_plane2,*m_plane3;
    QCheckBox* m_radio1,* m_radio2,* m_radio3;
    QStringList m_artPath;
    MIntSet m_selectNum;
    int m_currentIndex;

};


#endif // MUSICLRCFLOATPHOTOWIDGET_H
