#ifndef MUSICBOTTOMAREAWIDGET_H
#define MUSICBOTTOMAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include <QWidget>
#include <QSystemTrayIcon>
#include "musicglobaldefine.h"

class MusicSystemTrayMenu;
class MusicWindowExtras;
class MusicRippleSpecturmObject;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app bottom area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicBottomAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBottomAreaWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBottomAreaWidget(QWidget *parent = nullptr);

    ~MusicBottomAreaWidget();

    /*!
     * Get class object instance.
     */
    static MusicBottomAreaWidget *instance();
    /*!
     * Set up app ui.
     */
    void setupUi(Ui::MusicApplication* ui);
    /*!
     * Set destop lrc visible by string.
     */
    void setDestopLrcVisible(bool status) const;
    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status) const;
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
    void setSystemCloseConfig(bool status) { m_systemCloseConfig = status;}
    /*!
     * Get system close config state.
     */
    bool getSystemCloseConfig() { return m_systemCloseConfig;}
    /*!
     * Get system tray visible state.
     */
    bool systemTrayIsVisible() { return m_systemTray->isVisible();}
    /*!
     * Set system tray message show title and context.
     */
    void showMessage(const QString &title, const QString &text);

#if defined TTK_DEBUG && defined Q_OS_WIN && defined TTK_WINEXTRAS
    /*!
     * Set current value.
     */
    void setValue(int value) const;
    /*!
     * Set current range from start to end.
     */
    void setRange(int min, int max) const;
#endif
    /*!
     * Set window concise.
     */
    void setWindowConcise();
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();
    /*!
     * Get setting parameter.
     */
    void getParameterSetting();
    /*!
     * Current is show full container.
     */
    bool isLrcWidgetShowFullScreen() const;

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

protected:
    /*!
     * Create system tray icon.
     */
    void createSystemTrayIcon();

    Ui::MusicApplication *m_ui;
    bool m_systemCloseConfig;
    bool m_lrcWidgetShowFullScreen;
    QSystemTrayIcon *m_systemTray;
    MusicSystemTrayMenu *m_systemTrayMenu;
    MusicWindowExtras *m_musicWindowExtras;
    MusicRippleSpecturmObject *m_musicRipplesObject;

    static MusicBottomAreaWidget *m_instance;
};

#endif // MUSICBOTTOMAREAWIDGET_H
