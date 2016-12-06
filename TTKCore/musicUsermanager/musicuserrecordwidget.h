#ifndef MUSICUSERRECORDWIDGET_H
#define MUSICUSERRECORDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicUserRecordWidget;
}

class QComboBox;
class MusicUserModel;

/*! @brief The class of the user record widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserRecordWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicUserRecordWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicUserRecordWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setUserModel(MusicUserModel *model, const QString &uid);
    /*!
     * Set user model.
     */

Q_SIGNALS:
    void resetUserName(const QString &name);
    /*!
     * Reset current user name.
     */
    void userIconChanged(const QString &uid, const QString &icon);
    /*!
     * Reset current user icon.
     */

public Q_SLOTS:
    void cityComboBoxIndexChanged(const QString &city);
    /*!
     * City index has changed.
     */
    void confirmButtonClickedF();
    /*!
     * Save first information button clicked.
     */
    void openFileButtonClickedS();
    /*!
     * Open selected new pic button clicked.
     */
    void changeVerificationCodeT();
    /*!
     * Change verfication button clicked.
     */
    void confirmButtonClickedT();
    /*!
     * Modify third password button clicked.
     */
    void checkPwdStrength(int code);
    /*!
     * Check the new password strength.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void initTabF();
    /*!
     * init first infomation widget.
     */
    void initTabS();
    /*!
     * init second change icon widget.
     */
    void initTabT();
    /*!
     * init third security widget.
     */

    Ui::MusicUserRecordWidget *m_ui;
    MusicUserModel *m_userModel;

};

#endif // MUSICUSERRECORDWIDGET_H
