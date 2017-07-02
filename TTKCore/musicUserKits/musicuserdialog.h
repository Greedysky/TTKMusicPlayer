#ifndef MUSICUSERDIALOG_H
#define MUSICUSERDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
    explicit MusicUserDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicUserDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void checkToAutoLogin(QString &name, QString &icon);
    /*!
     * Check current user to login auto automatic.
     */
    void setUserModel(MusicUserModel *model);
    /*!
     * Set user model.
     */

Q_SIGNALS:
    void userLoginSuccess(const QString &uid, const QString &icon);
    /*!
     * User login success emit.
     */

public Q_SLOTS:
    void userLogin();
    /*!
     * Change to user login widget.
     */
    void checkUserLogin();
    /*!
     * Check the current user login state.
     */
    void registerUser();
    /*!
     * Change to user register widget.
     */
    void checkRegisterUser();
    /*!
     * Check user register mod.
     */
    void userForgotPasswd();
    /*!
     * Change to user forgot pwd widget.
     */
    void checkUserForgotPasswd();
    /*!
     * Check user forgot password mod.
     */
    void changeVerificationCode();
    /*!
     * Set current verification code.
     */
    void userComboBoxChanged(const QString &name);
    /*!
     * Selected new user to login.
     */
    void userEditTextChanged(const QString &name);
    /*!
     * Input a new user to login.
     */
    void buttonClicked(int index);
    /*!
     * Other login way, but now there is no supported.
     */

protected:
    void firstStatckWidget();
    /*!
     * Init user login widget.
     */
    void secondStatckWidget();
    /*!
     * Init user register widget.
     */
    void thirdStatckWidget();
    /*!
     * Init user forgot pwd widget.
     */
    void clearOriginData();
    /*!
     * Clear origin data in need.
     */
    void readFromUserConfig();
    /*!
     * Read user setting to config file.
     */
    int findUserNameIndex(const QString &name);
    /*!
     * To find current username in the record list.
     */
    void readFromUserSettings();
    /*!
     * Read user info from setting.
     */
    void writeToUserConfig();
    /*!
     * Write user setting to config file.
     */
    void writeToUserSettings();
    /*!
     * Write user info to setting.
     */
    void windowRectChanged(int index, int height);
    /*!
     * Reset window rect by diff index and geometry.
     */

    Ui::MusicUserDialog *m_ui;
    MusicUserModel *m_userModel;
    MusicUserRecords m_records;
    QString m_userName;

};

#endif // MUSICUSERDIALOG_H
