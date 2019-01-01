#ifndef MUSICUSERRECORDWIDGET_H
#define MUSICUSERRECORDWIDGET_H

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

#include "musicdatabaseobject.h"
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
    TTK_DECLARE_MODULE(MusicUserRecordWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicUserRecordWidget(QWidget *parent = nullptr);

    ~MusicUserRecordWidget();

    /*!
     * Avatar pixmap render.
     */
    static QString avatarPixmapRender(const MusicUserUIDItem &item, const QPixmap &pix);

    /*!
     * Set user model.
     */
    void setUserModel(MusicUserModel *model, const MusicUserUIDItem &uid);

Q_SIGNALS:
    /*!
     * Reset current user name.
     */
    void resetUserName(const QString &name);
    /*!
     * Reset current user icon.
     */
    void userIconChanged(const MusicUserUIDItem &uid, const QString &icon);

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
    MusicUserUIDItem m_userUID;
    QString m_iconLocalPath;

};

#endif // MUSICUSERRECORDWIDGET_H
