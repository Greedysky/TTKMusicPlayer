#ifndef MUSICUSERDIALOG_H
#define MUSICUSERDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include <QMouseEvent>
#include "musicabstractmovedialog.h"
#include "musicuserconfigmanager.h"

class MusicUserModel;

namespace Ui {
class MusicUserDialog;
}

/*! @brief The class of the user dialog.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicUserDialog(QWidget *parent = 0);

    ~MusicUserDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Check current user to login auto automatic.
     */
    void checkToAutoLogin(QString &name, QString &icon);
    /*!
     * Set user model.
     */
    void setUserModel(MusicUserModel *model);

Q_SIGNALS:
    /*!
     * User login success emit.
     */
    void userLoginSuccess(const QString &uid, const QString &icon);

public Q_SLOTS:
    /*!
     * Change to user login widget.
     */
    void userLogin();
    /*!
     * Check the current user login state.
     */
    void checkUserLogin();
    /*!
     * Change to user register widget.
     */
    void registerUser();
    /*!
     * Check user register mod.
     */
    void checkRegisterUser();
    /*!
     * Change to user forgot pwd widget.
     */
    void userForgotPasswd();
    /*!
     * Check user forgot password mod.
     */
    void checkUserForgotPasswd();
    /*!
     * Set current verification code.
     */
    void changeVerificationCode();
    /*!
     * Selected new user to login.
     */
    void userComboBoxChanged(const QString &name);
    /*!
     * Input a new user to login.
     */
    void userEditTextChanged(const QString &name);
    /*!
     * Other login way, but now there is no supported.
     */
    void buttonClicked(int index);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Init user login widget.
     */
    void firstStatckWidget();
    /*!
     * Init user register widget.
     */
    void secondStatckWidget();
    /*!
     * Init user forgot pwd widget.
     */
    void thirdStatckWidget();
    /*!
     * Clear origin data in need.
     */
    void clearOriginData();
    /*!
     * Read user setting to config file.
     */
    void readFromUserConfig();
    /*!
     * To find current username in the record list.
     */
    int findUserNameIndex(const QString &name);
    /*!
     * Read user info from setting.
     */
    void readFromUserSettings();
    /*!
     * Write user setting to config file.
     */
    void writeToUserConfig();
    /*!
     * Write user info to setting.
     */
    void writeToUserSettings();
    /*!
     * Reset window rect by diff index and geometry.
     */
    void windowRectChanged(int index, int height);

    Ui::MusicUserDialog *m_ui;
    MusicUserModel *m_userModel;
    MusicUserRecords m_records;
    QString m_userName;

};

#endif // MUSICUSERDIALOG_H
