#ifndef MUSICDESKTOPWALLPAPERWIDGET_H
#define MUSICDESKTOPWALLPAPERWIDGET_H

#include "musicmovewidgetabstract.h"

class MusicDesktopWallpaperThread;

namespace Ui {
class MusicDesktopWallpaperWidget;
}

class MUSIC_WIDGET_EXPORT MusicDesktopWallpaperWidget : public MusicMoveWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicDesktopWallpaperWidget(QWidget *parent = 0);
    ~MusicDesktopWallpaperWidget();

signals:
public slots:
    void netRadioButtonPressed();
    void localRadioButtonPressed();
    void playRadioButtonPressed();
    void viewButtonPressed();
    void confirmButtonPressed();
    void stopButtonPressed();
    void cancelButtonPressed();
    void parameterFinished();

protected:
    void initWidgetStyle() const;
    void initParameters() const;
    void findFiles(const QString &path);
    void setAutoStart(bool autoStart) const;

    Ui::MusicDesktopWallpaperWidget *ui;
    MusicDesktopWallpaperThread *m_wallThread;
    QStringList m_path;
    int m_currentMode;

};

#endif // MUSICDESKTOPWALLPAPERWIDGET_H
