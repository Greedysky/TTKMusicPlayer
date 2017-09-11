#ifndef MUSICUSERWINDOW_H
#define MUSICUSERWINDOW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QStackedWidget>
#include "musicglobaldefine.h"

class MusicUserModel;
class MusicUserDialog;
class MusicUserManagerDialog;

namespace Ui {
class MusicUserWindow;
}

/*! @brief The class of the user window.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserWindow : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MusicUserWindow(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicUserWindow();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    bool isUserLogin() const;
    /*!
     * Check if the user is login now.
     */

public Q_SLOTS:
    void musicUserLogin();
    /*!
     * User widget button clicked.
     */
    void userStateChanged(const QString &uid, const QString &icon);
    /*!
     * User login state changed.
     */
    void musicUserContextLogin();
    /*!
     * Send user to login.
     */
    void checkToAutoLogin();
    /*!
     * Check current user to login auto automatic.
     */

protected:
    bool connectDatabase();
    /*!
     * Connect to database.
     */
    bool disConnectDatabase();
    /*!
     * Disconnect to database.
     */

    Ui::MusicUserWindow *m_ui;
    MusicUserModel* m_userModel;
    MusicUserManagerDialog *m_userManager;

};

#endif // MUSICUSERWINDOW_H
