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
    /*!
     * Object contsructor.
     */
    explicit MusicUserWindow(QWidget *parent = 0);

    ~MusicUserWindow();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Check if the user is login now.
     */
    bool isUserLogin() const;

public Q_SLOTS:
    /*!
     * User widget button clicked.
     */
    void musicUserLogin();
    /*!
     * User login state changed.
     */
    void userStateChanged(const QString &uid, const QString &icon);
    /*!
     * Send user to login.
     */
    void musicUserContextLogin();
    /*!
     * Check current user to login auto automatic.
     */
    void checkToAutoLogin();

protected:
    /*!
     * Connect to database.
     */
    bool connectDatabase();
    /*!
     * Disconnect to database.
     */
    bool disConnectDatabase();

    Ui::MusicUserWindow *m_ui;
    MusicUserModel* m_userModel;
    MusicUserManagerDialog *m_userManager;

};

#endif // MUSICUSERWINDOW_H
