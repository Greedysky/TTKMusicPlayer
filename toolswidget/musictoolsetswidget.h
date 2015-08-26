#ifndef MUSICTOOLSETSWIDGET_H
#define MUSICTOOLSETSWIDGET_H

#include <QListWidget>
#include "musicuiobject.h"
#include "musiclibexportglobal.h"

class QProcess;
class MusicSpectrumWidget;
class MusicDesktopWallpaperWidget;

class MUSIC_WIDGET_EXPORT MusicToolSetsWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicToolSetsWidget(QWidget *parent = 0);
    ~MusicToolSetsWidget();

signals:
    void setSpectrum(HWND wnd, int w, int h,
                     int x = 0, int y = 0);
    void timerParameterChanged();
    void addSongToPlay(const QStringList &list);
    void getCurrentPlayList(QStringList &list);

public slots:
    void itemHasClicked(QListWidgetItem *item);

protected:
    void setTransparent(int angle);
    void clearAllItems();
    void addListWidgetItem();

    MusicSpectrumWidget *m_musicSpectrumWidget;
    MusicDesktopWallpaperWidget *m_wallpaper;
    QProcess *m_process;

};

#endif // MUSICTOOLSETSWIDGET_H
