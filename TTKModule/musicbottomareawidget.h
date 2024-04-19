#ifndef MUSICBOTTOMAREAWIDGET_H
#define MUSICBOTTOMAREAWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QWidget>
#include <QSystemTrayIcon>
#include "musicglobaldefine.h"

class MusicSystemTrayMenu;
class MusicPlatformExtras;
class MusicBlurSpecturmModule;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app bottom area widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBottomAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBottomAreaWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicBottomAreaWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBottomAreaWidget();

    /*!
     * Get class object instance.
     */
    static MusicBottomAreaWidget *instance();

    /*!
     * Set up app ui.
     */
    void setupUi(Ui::MusicApplication *ui);
    /*!
     * Set destop lrc visible by string.
     */
    void setDestopLrcVisible(bool status) const;
    /*!
     * Set current play state.
     */
    void setCurrentPlayState(bool state) const;
    /*!
     * Set current play volume.
     */
    void setVolumeValue(int value) const;
    /*!
     * Set current song text.
     */
    void setLabelText(const QString &name) const;

    /*!
     * Set system close config or not.
     */
    void setSystemCloseConfig(bool status) { m_systemCloseConfig = status; }
    /*!
     * Get system close config state.
     */
    bool systemCloseConfig() { return m_systemCloseConfig; }
    /*!
     * Get system tray visible state.
     */
    bool systemTrayIsVisible() { return m_systemTray->isVisible(); }
    /*!
     * Set system tray message show title and context.
     */
    void showMessage(const QString &title, const QString &text);

    /*!
     * Set window concise.
     */
    void setWindowConcise();
    /*!
     * Current is show full container.
     */
    bool isLrcWidgetShowFullScreen() const;

    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();
    /*!
     * Apply settings parameters.
     */
    void applyParameter();

public Q_SLOTS:
    /*!
     * Show full container or not.
     */
    void lrcWidgetShowFullScreen();
    /*!
     * Lock or not current desktop lrc.
     */
    void lockDesktopLrc(bool lock);
    /*!
     * Set desktop lrc close state.
     */
    void desktopLrcClosed();
    /*!
     * System tray icon activate.
     */
    void iconActivated(QSystemTrayIcon::ActivationReason);

private:
    /*!
     * Create system tray icon.
     */
    void createSystemTrayIcon();

    Ui::MusicApplication *m_ui;
    bool m_systemCloseConfig;
    bool m_lrcWidgetShowFullScreen;
    QSystemTrayIcon *m_systemTray;
    MusicSystemTrayMenu *m_systemTrayMenu;
    MusicPlatformExtras *m_platformExtras;
    MusicBlurSpecturmModule *m_blurModule;

    static MusicBottomAreaWidget *m_instance;

};

#endif // MUSICBOTTOMAREAWIDGET_H
