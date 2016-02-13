#ifndef MUSICLRCCCONTAINERFORINLINELR_H
#define MUSICLRCCCONTAINERFORINLINELR_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QBoxLayout>
#include "musiclrccontainer.h"

class MusicLRCManagerForInline;
class MusicLrcFloatWidget;
class MusicClickedLabel;
class MusicLrcAnalysis;

class MUSIC_LRC_EXPORT MusicLrcContainerForInline : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForInline(QWidget *parent = 0);
    virtual ~MusicLrcContainerForInline();

    virtual void startTimerClock() override;
    virtual void stopLrcMask() override;
    virtual void setMaskLinearGradientColor(QColor = CL_Mask) const override;
    virtual void setSettingParameter();
    void updateCurrentLrc(qint64 time);

    bool transLrcFileToTime(const QString &lrcFileName);
    QString text() const;
    qint64 setSongSpeedAndSlow(qint64 time);
    bool findText(qint64 total, QString &pre, QString &last, qint64 &interval) const;

    inline bool artBackgroundIsShow() const {return m_showArtBackground;}
    void setLrcSize(MusicLRCManager::LrcSizeTable = MusicLRCManager::Middle) const;
    int getLrcSize() const;

    void resizeWidth(int width);

Q_SIGNALS:
    void updateCurrentTime(qint64 time);
    void theArtBgHasChanged();

public Q_SLOTS:
    void lrcSizeChanged(QAction *action) const;
    void lrcTimeSpeedChanged(QAction *action);
    void revertLrcTimeSpeed();
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
    void revertLrcTimeSpeed(qint64 pos);
    void createNoLrcCurrentInfo();
    void showNoLrcCurrentInfo();
    void initLrc(const QString &str);
    void setItemStyleSheet();
    void setItemStyleSheet(int index, int size, int transparent);

    QPoint m_mousePressedAt, m_mouseMovedAt;
    bool m_mouseLeftPressed;
    bool m_showArtBackground;
    bool m_showInlineLrc;
    qint64 m_changeSpeedValue;
    QVBoxLayout *m_vBoxLayout;
    MusicLrcFloatWidget *m_lrcFloatWidget;
    MusicClickedLabel *m_noLrcCurrentInfo;
    MusicLrcAnalysis *m_lrcAnalysis;

};

#endif // MUSICLRCCCONTAINERFORINLINELR_H
