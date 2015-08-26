#ifndef MUSICWINDOWEXTRAS_H
#define MUSICWINDOWEXTRAS_H

#include <QObject>
#include "musiclibexportglobal.h"

class MusicApplication;
class QWinTaskbarButton;
class QWinTaskbarProgress;
class QWinThumbnailToolBar;
class QWinThumbnailToolButton;

class MUSIC_WIDGET_EXPORT MusicWindowExtras : public QObject
{
    Q_OBJECT
public:
    explicit MusicWindowExtras(MusicApplication *parent = 0);
    ~MusicWindowExtras();

    void showPlayStatus(bool status);
    void setValue(int value);
    void setRange(int start, int end);
    void disableBlurBehindWindow(bool enable);
    inline bool isDisableBlurBehindWindow() const
                { return m_disableBlurBehindWindow; }

signals:

protected:
    void createJumpList();
    void createTaskbar();
    void createThumbnailToolBar();

    bool m_disableBlurBehindWindow;
    MusicApplication *m_superClass;
    QWinTaskbarButton *m_taskbarButton;
    QWinTaskbarProgress *m_taskbarProgress;
    QWinThumbnailToolBar *m_thumbnailToolBar;
    QWinThumbnailToolButton *m_playToolButton;
    QWinThumbnailToolButton *m_forwardToolButton;
    QWinThumbnailToolButton *m_backwardToolButton;

};

#endif // MUSICWINDOWEXTRAS_H
