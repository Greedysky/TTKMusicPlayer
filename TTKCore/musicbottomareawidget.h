#ifndef MUSICBOTTOMAREAWIDGET_H
#define MUSICBOTTOMAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QSystemTrayIcon>
#include "musicobject.h"
#include "musicglobaldefine.h"

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
    /*!
     * Set current play state button.
     */
    void setLabelText(const QString &name) const;
    /*!
     * Set current song text.
     */

    void setSystemCloseConfig(const QString &status);
    void setSystemCloseConfig(bool status) { m_systemCloseConfig = status;}
    bool getSystemCloseConfig() { return m_systemCloseConfig;}
    bool systemTrayIsVisible() { return m_systemTray->isVisible();}
    void showMessage(const QString &title, const QString &text);
    void setVolumeValue(int value) const;
    /*!
     * Set current volume value by index.
     */

#if defined MUSIC_DEBUG && defined Q_OS_WIN && defined MUSIC_QT_5
    void setValue(int value) const;
    void setRange(int min, int max) const;
#endif
    void setWindowConcise();

Q_SIGNALS:
    void setShowDesktopLrc(bool show);
    void setWindowLockedChanged();
    /*!
     * Lock current desktop lrc state changed.
     */

public Q_SLOTS:
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
