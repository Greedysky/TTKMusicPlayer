#ifndef MUSICBOTTOMAREAWIDGET_H
#define MUSICBOTTOMAREAWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include "musiclibexportglobal.h"

class MusicSystemTrayMenu;
class MusicWindowExtras;

namespace Ui {
    class MusicApplication;
}

class MUSIC_GUI_EXPORT MusicBottomAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBottomAreaWidget(QWidget *parent = 0);
    ~MusicBottomAreaWidget();

    void setupUi(Ui::MusicApplication* ui);
    void setDestopLrcVisible(const QString &status) const;
    void showPlayStatus(bool status) const;
    void setLabelText(const QString &name) const;

    void setSystemCloseConfig(const QString &status);
    void setSystemCloseConfig(bool status)
            { m_systemCloseConfig = status;}
    bool getSystemCloseConfig()
            { return m_systemCloseConfig;}
    bool systemTrayIsVisible()
            { return m_systemTray->isVisible();}
    void showMessage(const QString &title, const QString &text);
    void setVolumeValue(int value) const;

#if defined MUSIC_DEBUG && defined Q_OS_WIN
    void setValue(int value) const;
    void setRange(int min, int max) const;
#endif
    void setWindowConcise();

signals:
    void setShowDesktopLrc(bool show);
    void setWindowLockedChanged();

public slots:
    void lockDesktopLrc(bool lock);
    void desktopLrcClosed();
    void iconActivated(QSystemTrayIcon::ActivationReason);

protected:
    void createToolPopupMenu();
    void createSystemTrayIcon();
    void createMenuActions() const;

    QWidget *m_supperClass;
    Ui::MusicApplication *m_ui;
    bool m_systemCloseConfig;
    QMenu m_toolPopupMenu;
    QSystemTrayIcon *m_systemTray;
    MusicSystemTrayMenu *m_systemTrayMenu;
    MusicWindowExtras *m_musicWindowExtras;

};

#endif // MUSICBOTTOMAREAWIDGET_H
