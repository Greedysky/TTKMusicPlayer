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
    void setSpectrum(HWND,int w,int h,int x = 0,int y = 0);
    void timerParameterChanged();
    void addSongToPlay(const QStringList&);
    void getCurrentPlayList(QStringList&);

public slots:
    void itemHasClicked(QListWidgetItem*);

protected:
    void setTransparent(int angle);
    void clearAllItems();
    void addListWidgetItem();

    MusicSpectrumWidget* m_musicSpectrumWidget;
    MusicDesktopWallpaperWidget* m_wallpaper;
    QProcess* m_process;

};

#endif // MUSICTOOLSETSWIDGET_H
