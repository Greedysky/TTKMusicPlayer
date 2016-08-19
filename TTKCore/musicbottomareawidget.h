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
class MusicLocalSongSearchDialog;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app bottom area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_GUI_EXPORT MusicBottomAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBottomAreaWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBottomAreaWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    static MusicBottomAreaWidget *instance();
    /*!
     * Get class object instance.
     */
    void setupUi(Ui::MusicApplication* ui);
    /*!
     * Set up app ui.
     */
    void setDestopLrcVisible(bool status) const;
    /*!
     * Set destop lrc visible by string.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play state button.
     */
    void setVolumeValue(int value) const;
    /*!
     * Set current play volume.
     */
    void setLabelText(const QString &name) const;
    /*!
     * Set current song text.
     */

    void setSystemCloseConfig(bool status) { m_systemCloseConfig = status;}
    /*!
     * Set system close config or not.
     */
    bool getSystemCloseConfig() { return m_systemCloseConfig;}
    /*!
     * Get system close config state.
     */
    bool systemTrayIsVisible() { return m_systemTray->isVisible();}
    /*!
     * Get system tray visible state.
     */
    void showMessage(const QString &title, const QString &text);
    /*!
     * Set system tray message show title and context.
     */

#if defined MUSIC_DEBUG && defined Q_OS_WIN && defined MUSIC_GREATER_NEW
    void setValue(int value) const;
    /*!
     * Set current value.
     */
    void setRange(int min, int max) const;
    /*!
     * Set current range from start to end.
     */
#endif
    void setWindowConcise();
    /*!
     * Set window concise.
     */

    QString getSearchedText() const;
    /*!
     * Get the search text that the user searched.
     */

    void resizeWindow();
    /*!
     * Resize window bound by widgte resize called.
     */

Q_SIGNALS:
    void setShowDesktopLrc(bool show);
    /*!
     * Set show desktop lrc state changed emit.
     */
    void setWindowLockedChanged();
    /*!
     * Lock current desktop lrc state changed.
     */

public Q_SLOTS:
    void musicSearch();
    /*!
     * Show searched text widget.
     */
    void clearSearchedText();
    /*!
     * Clear current search lineedit text.
     */
    void lockDesktopLrc(bool lock);
    /*!
     * Lock or not current desktop lrc.
     */
    void desktopLrcClosed();
    /*!
     * Set desktop lrc close state.
     */
    void iconActivated(QSystemTrayIcon::ActivationReason);
    /*!
     * System tray icon activate.
     */

protected:
    void createSystemTrayIcon();
    /*!
     * Create system tray icon.
     */

    Ui::MusicApplication *m_ui;
    bool m_systemCloseConfig;
    QMenu m_toolPopupMenu;
    QSystemTrayIcon *m_systemTray;
    MusicSystemTrayMenu *m_systemTrayMenu;
    MusicWindowExtras *m_musicWindowExtras;
    MusicLocalSongSearchDialog *m_musicLocalSongSearch;

    static MusicBottomAreaWidget *m_instance;
};

#endif // MUSICBOTTOMAREAWIDGET_H
