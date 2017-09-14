#ifndef MUSICBOTTOMAREAWIDGET_H
#define MUSICBOTTOMAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicBottomAreaWidget(QWidget *parent = 0);

    ~MusicBottomAreaWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
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

#if defined MUSIC_DEBUG && defined Q_OS_WIN && defined MUSIC_GREATER_NEW
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
     * Get the search text that the user searched.
     */
    QString getSearchedText() const;

    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * Show searched text widget.
     */
    void musicSearchWidget();
    /*!
     * Clear current search lineedit text.
     */
    void clearSearchedText();
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
    QMenu m_toolPopupMenu;
    QSystemTrayIcon *m_systemTray;
    MusicSystemTrayMenu *m_systemTrayMenu;
    MusicWindowExtras *m_musicWindowExtras;
    MusicLocalSongSearchDialog *m_musicSongSearchWidget;

    static MusicBottomAreaWidget *m_instance;
};

#endif // MUSICBOTTOMAREAWIDGET_H
