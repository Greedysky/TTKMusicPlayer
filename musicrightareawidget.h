#ifndef MUSICRIGHTAREAWIDGET_H
#define MUSICRIGHTAREAWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musiclibexportglobal.h"

class MusicVideoPlayer;
class MusicSettingWidget;
class MusicDownloadStatusLabel;
class MusicLrcContainerForDesktop;

namespace Ui {
    class MusicApplication;
}

class MUSIC_GUI_EXPORT MusicRightAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicRightAreaWidget(QWidget *parent = 0);
    ~MusicRightAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    void stopLrcMask() const;
    void startTimerClock() const;
    void showPlayStatus(bool status) const;
    void setDestopLrcVisible(const QString &status) const;
    bool getDestopLrcVisible() const;
    void setInlineLrcVisible(const QString &status) const;
    void setSettingParameter() const;
    void getParameterSetting() const;
    bool checkSettingParameterValue() const;
    void updateCurrentLrc(qint64 current, qint64 total,
                          bool playStatus) const;
    void loadCurrentSongLrc(const QString &name,
                            const QString &path) const;
    void setSongSpeedAndSlow(qint64 time) const;
    void musicCheckHasLrcAlready() const;
    void showSettingWidget() const;

signals:
    void updateBgThemeDownload();
    void updateBackgroundTheme();
    void desktopLrcClosed();
    void lockDesktopLrc(bool lock);

public slots:
    void setDestopLrcVisible(bool v) const;
    void setWindowLockedChanged();
    void musicSearchButtonSearched();
    void musicResearchButtonSearched(const QString &name);
    void musicIndexWidgetButtonSearched();
    void musicSearchWidgetButtonSearched();
    void musicLrcWidgetButtonSearched();
    void musicSearchRefreshButtonRefreshed();
    void musicVideoWidgetButtonSearched();
    void musicVideoButtonSearched(const QString &name);
    void musicVideoSetPopup(bool popup);
    void musicVideoFullscreen(bool full);

protected:
    void createVideoWidget(bool create);
    void musicButtonStyleClear();

    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    MusicSettingWidget *m_setting;
    MusicLrcContainerForDesktop *m_musiclrcfordesktop;
    MusicDownloadStatusLabel *m_downloadStatusLabel;
    MusicVideoPlayer *m_videoPlayer;

};

#endif // MUSICRIGHTAREAWIDGET_H
