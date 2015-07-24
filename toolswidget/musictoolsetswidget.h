#ifndef MUSICTOOLSETSWIDGET_H
#define MUSICTOOLSETSWIDGET_H

#include <QListWidget>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class QProcess;
class MusicApplication;
class MusicSpectrumWidget;
class MusicDesktopWallpaperWidget;

class MUSIC_EXPORT MusicToolSetsWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicToolSetsWidget(MusicApplication *parent = 0);
    ~MusicToolSetsWidget();

    void setSongStringList(const QStringList&);

signals:
    void setSpectrum(HWND,int w,int h,int x = 0,int y = 0);

public slots:
    void itemHasClicked(QListWidgetItem*);
    void setCurrentFileName(QString&);

protected:
    void setTransparent(int angle);
    void clearAllItems();
    void addListWidgetItem();

    QStringList m_songlist;
    MusicApplication* m_parentWidget;
    MusicSpectrumWidget* m_musicSpectrumWidget;
    MusicDesktopWallpaperWidget* m_wallpaper;
    QProcess* m_process;

};

#endif // MUSICTOOLSETSWIDGET_H
