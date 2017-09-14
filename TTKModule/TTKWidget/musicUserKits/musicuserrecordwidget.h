#ifndef MUSICUSERRECORDWIDGET_H
#define MUSICUSERRECORDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicUserRecordWidget(QWidget *parent = 0);

    ~MusicUserRecordWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set user model.
     */
    void setUserModel(MusicUserModel *model, const QString &uid);

Q_SIGNALS:
    /*!
     * Reset current user name.
     */
    void resetUserName(const QString &name);
    /*!
     * Reset current user icon.
     */
    void userIconChanged(const QString &uid, const QString &icon);

public Q_SLOTS:
    /*!
     * City index has changed.
     */
    void cityComboBoxIndexChanged(const QString &city);
    /*!
     * Save first information button clicked.
     */
    void confirmButtonClickedF();
    /*!
     * Open selected new pic button clicked.
     */
    void openFileButtonClickedS();
    /*!
     * Save selected new pic button clicked.
     */
    void saveFileButtonClickedS();
    /*!
     * Send intersected pixmap if intersected flag set.
     */
    void intersectedPixmap(const QPixmap &pix);
    /*!
     * Change verfication button clicked.
     */
    void changeVerificationCodeT();
    /*!
     * Modify third password button clicked.
     */
    void confirmButtonClickedT();
    /*!
     * Check the new password strength.
     */
    void checkPwdStrength(int code);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * init first infomation widget.
     */
    void initTabF();
    /*!
     * init second change icon widget.
     */
    void initTabS();
    /*!
     * init third security widget.
     */
    void initTabT();

    Ui::MusicUserRecordWidget *m_ui;
    MusicUserModel *m_userModel;
    QString m_iconLocalPath;

};

#endif // MUSICUSERRECORDWIDGET_H
