#ifndef MUSICSYSTEMTRAYMENU_H
#define MUSICSYSTEMTRAYMENU_H

#include <QMenu>
#include "musiclibexportglobal.h"

class QLabel;
class QWidgetAction;
class QToolButton;

class MUSIC_WIDGET_EXPORT MusicSystemTrayMenu : public QMenu
{
    Q_OBJECT
public:
    MusicSystemTrayMenu(QWidget *parent = 0);
    ~MusicSystemTrayMenu();

    void setLabelText(const QString&);
    void showDesktopLrc(const QString&);
    void showPlayStatus(bool);

signals:
    void setShowDesktopLrc(bool);
    void setWindowLockedChanged();

public slots:
    void showDesktopLrc();
    void lockDesktopLrc(bool);

protected:
    void createWidgetActions();
    QWidgetAction* m_widgetAction;
    QLabel* m_showText;
    QToolButton* m_previousPlay;
    QToolButton* m_nextPlay;
    QToolButton* m_PlayOrStop;
    QAction* m_showLrcAction;
    QAction* m_lockLrcAction;

};

#endif // MUSICSYSTEMTRAYMENU_H
