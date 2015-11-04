#ifndef MUSICLRCCONTAINERFORDESKTOP_H
#define MUSICLRCCONTAINERFORDESKTOP_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrccontainer.h"

class QPushButton;
class QToolButton;
class MusicLRCManagerForDesktop;

class MUSIC_LRC_EXPORT MusicLrcContainerForDesktop : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForDesktop(QWidget *parent = 0);
    virtual ~MusicLrcContainerForDesktop();

    virtual void startTimerClock();
    virtual void stopLrcMask();
    virtual void setMaskLinearGradientColor(QColor = CL_Mask) const;
    virtual void setSettingParameter();
    void showPlayStatus(bool status) const;
    void initCurrentLrc() const;
    void updateCurrentLrc(const QString &first,
                          const QString &second, qint64 time);
signals:
    void desktopLrcClosed();
    void setWindowLockedChanged(bool lock);

public slots:
    void setWindowLockedChanged();
    void setLrcBigerChanged();
    void setLrcSmallerChanged();

protected:
    void setSelfGeometry() const;
    void creatToolBarWidget();
    void setButtonIcon() const;
    void setButtonCursor() const;
    void setButtonTips() const;
    void resizeLrcSizeArea();
    void resizeLrcSizeArea(bool bigger);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);

    QWidget *m_supperClass, *m_toolBarWidget, *m_desktopWidget;
    QPushButton *m_showMainWindow;
    QToolButton *m_toolPreSongButton, *m_toolNextSongButton;
    QToolButton *m_toolPlayButton, *m_toolSettingButton;
    QToolButton *m_toolCloseButton, *m_toolWindowLockedButton;
    QToolButton *m_toolLrcBigerButton, *m_toolLrcSmallerButton;
    QToolButton *m_toolUpdateLrcButton;

    QPoint m_offset;
    bool m_windowLocked, m_reverse;
    int m_currentLrcFontSize;
    qint64 m_currentTime;
    QPoint m_geometry;

};

#endif // MUSICLRCCONTAINERFORDESKTOP_H
