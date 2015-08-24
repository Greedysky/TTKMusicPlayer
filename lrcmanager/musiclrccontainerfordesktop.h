#ifndef MUSICLRCCONTAINERFORDESKTOP_H
#define MUSICLRCCONTAINERFORDESKTOP_H

#include "musiclrccontainer.h"

class QPushButton;
class QToolButton;
class MusicLRCManagerForDesktop;

class MUSIC_WIDGET_EXPORT MusicLrcContainerForDesktop : public MusicLrcContainer
{
    Q_OBJECT
public:
    explicit MusicLrcContainerForDesktop(QWidget *parent = 0);
    virtual ~MusicLrcContainerForDesktop();

    virtual void startTimerClock();
    virtual void stopLrcMask();
    virtual void setMaskLinearGradientColor(QColor = CL_Mask);
    virtual void setSettingParameter();
    void showPlayStatus(bool);
    void initCurrentLrc();
    void updateCurrentLrc(const QString&, const QString&,
                          qint64 time);
signals:
    void desktopLrcClosed();
    void setWindowLockedChanged(bool);

public slots:
    void setWindowLockedChanged();
    void setLrcBigerChanged();
    void setLrcSmallerChanged();

protected:
    void setSelfGeometry();
    void creatToolBarWidget();
    void setButtonIcon();
    void setButtonCursor();
    void setButtonTips();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);

    QWidget *m_supperClass;
    QWidget* m_toolBarWidget;
    QWidget* m_desktopWidget;
    QPushButton* m_showMainWindow;
    QToolButton* m_toolPreSongButton;
    QToolButton* m_toolNextSongButton;
    QToolButton* m_toolPlayButton;
    QToolButton* m_toolSettingButton;
    QToolButton* m_toolCloseButton;
    QToolButton* m_toolWindowLockedButton;
    QToolButton* m_toolLrcBigerButton;
    QToolButton* m_toolLrcSmallerButton;
    QToolButton* m_toolUpdateLrcButton;

    QPoint m_offset;
    bool m_windowLocked;
    bool m_reverse;
    int m_currentLrcFontSize;
    QPoint m_geometry;

};

#endif // MUSICLRCCONTAINERFORDESKTOP_H
