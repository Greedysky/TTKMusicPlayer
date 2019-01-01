#ifndef MUSICUSERWINDOW_H
#define MUSICUSERWINDOW_H

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

#include "musicwidgetheaders.h"
#include "musicdatabaseobject.h"

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
    TTK_DECLARE_MODULE(MusicUserWindow)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicUserWindow(QWidget *parent = nullptr);

    ~MusicUserWindow();

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
    void userStateChanged(const MusicUserUIDItem &uid, const QString &icon);
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
    MusicUserModel *m_userModel;
    MusicUserManagerDialog *m_userManager;

};

#endif // MUSICUSERWINDOW_H
