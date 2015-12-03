#ifndef MUSICLRCFLOATPHOTOWIDGET_H
#define MUSICLRCFLOATPHOTOWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musiclrcfloatabstractwidget.h"

#define PHOTO_WIDTH     110
#define PHOTO_HEIGHT    65
#define PHOTO_PERLINE   3

class QPushButton;
class QCheckBox;
class MusicClickedLabel;

class MUSIC_LRC_EXPORT MusicLrcFloatPhotoWidget : public MusicLrcFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatPhotoWidget(QWidget *parent = 0);
    virtual ~MusicLrcFloatPhotoWidget();

    void resizeWidth(int width);

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
    void userSelectCheckBoxChecked(int index);

protected:
    virtual void enterEvent(QEvent *) override {}
    virtual void leaveEvent(QEvent *) override {}
    void showPhoto() const;

    QWidget* m_filmBGWidget;
    QCheckBox* m_checkBox;
    QPushButton* m_previous,*m_next;
    QPushButton* m_confirmButton,*m_cancelButton;
    MusicClickedLabel* m_plane1,*m_plane2,*m_plane3;
    QCheckBox* m_radio1,* m_radio2,* m_radio3;
    QStringList m_artPath;
    MIntSet m_selectNum;
    int m_currentIndex;

};


#endif // MUSICLRCFLOATPHOTOWIDGET_H
