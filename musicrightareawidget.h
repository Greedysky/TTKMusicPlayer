#ifndef MUSICRIGHTAREAWIDGET_H
#define MUSICRIGHTAREAWIDGET_H

#include <QWidget>
#include "musiclibexportglobal.h"

class MusicVideoPlayer;
class MusicLrcContainerForDesktop;

namespace Ui {
    class MusicApplication;
}

class MUSIC_WIDGET_EXPORT MusicRightAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicRightAreaWidget(QWidget *parent = 0);
    ~MusicRightAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    void stopLrcMask();
    void startTimerClock();
    void showPlayStatus(bool status);
    void setDestopLrcVisible(const QString& status);
    bool getDestopLrcVisible();
    void setInlineLrcVisible(const QString& status);
    void setSettingParameter();
    void getParameterSetting();
    bool checkSettingParameterValue();
    void updateCurrentLrc(qint64 current, qint64 total,
                          bool playStatus);
    void loadCurrentSongLrc(const QString& name,
                            const QString& path);
    void setSongSpeedAndSlow(qint64 time);

signals:
    void updateBgThemeDownload();
    void updateBackgroundTheme();

public slots:
    void setDestopLrcVisible(bool);
    void setWindowLockedChanged();
    void musicSearchButtonSearched();
    void musicIndexWidgetButtonSearched();
    void musicSearchWidgetButtonSearched();
    void musicLrcWidgetButtonSearched();
    void musicSearchRefreshButtonRefreshed();
    void musicVedioWidgetButtonSearched();
    void musicVedioWidgetButtonDoubleClicked();

protected:
    void createVedioWidget(bool);

    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    MusicLrcContainerForDesktop* m_musiclrcfordesktop;
    MusicVideoPlayer* m_videoPlayer;

};

#endif // MUSICRIGHTAREAWIDGET_H
