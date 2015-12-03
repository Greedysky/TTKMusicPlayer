#ifndef MUSICLRCCCONTAINERFORINLINELR_H
#define MUSICLRCCCONTAINERFORINLINELR_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrccontainer.h"

#define MIN_LRCCONTAIN_COUNT 9
#define CURRENT_LRC_PAINT 4

class QVBoxLayout;
class MusicLRCManagerForInline;
class MusicLrcFloatWidget;

class MUSIC_LRC_EXPORT MusicLrcContainerForInline : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForInline(QWidget *parent = 0);
    virtual ~MusicLrcContainerForInline();

    virtual void startTimerClock() override;
    virtual void stopLrcMask() override;
    virtual void setMaskLinearGradientColor(QColor = CL_Mask) const override;
    void updateCurrentLrc(qint64 time);

    bool transLrcFileToTime(const QString &lrcFileName);
    QString text() const;
    void setSongSpeedAndSlow(qint64 time);
    inline MIntStringMap& getLrcContainer() {return m_lrcContainer;}

    inline bool artBackgroundIsShow() const {return m_showArtBackground;}
    void setLrcSize(MusicLRCManager::LrcSizeTable = MusicLRCManager::Middle) const;
    int getLrcSize() const;

    void resizeWidth(int width);

signals:
    void updateCurrentTime(qint64 time);
    void theArtBgHasChanged();

public slots:
    void lrcSizeChanged(QAction *action) const;
    void theArtBgChanged();
    void theArtBgUploaded();
    void theShowLrcChanged();
    void lrcOpenFileDir() const;
    void lrcCopyClipboard() const;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    void changeLrcPostion(const QString &type);
    void initLrc();

    QPoint m_mousePressedAt;
    QPoint m_mouseMovedAt;
    bool m_mouseLeftPressed;
    bool m_showArtBackground;
    bool m_showInlineLrc;
    int m_currentLrcIndex;
    QVBoxLayout *m_vBoxLayout;
    QStringList m_currentShowLrcContainer;
    QString m_currentLrcFileName;
    MIntStringMap m_lrcContainer;
    MusicLrcFloatWidget *m_lrcFloatWidget;

};

#endif // MUSICLRCCCONTAINERFORINLINELR_H
