#ifndef MUSICUSERMANAGERDIALOG_H
#define MUSICUSERMANAGERDIALOG_H

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

#include <QTime>
#include "musicwidgetheaders.h"
#include "musicabstractmovedialog.h"
#include "musicdatabaseobject.h"

class MusicUserModel;

namespace Ui {
class MusicUserManagerDialog;
}

/*! @brief The class of the user manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_USER_EXPORT MusicUserManagerDialog : public QDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicUserManagerDialog)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicUserManagerDialog(QWidget *parent = nullptr);

    ~MusicUserManagerDialog();

    /*!
     * Set current user's uid.
     */
    void setUserUID(const MusicUserUIDItem &uid);
    /*!
     * Set user model.
     */
    void setUserModel(MusicUserModel *model);

Q_SIGNALS:
    /*!
     * User login state changed.
     */
    void userStateChanged(const MusicUserUIDItem &uid, const QString &icon);

public Q_SLOTS:
    /*!
     * User logout clicked.
     */
    void musicUserLogoff();
    /*!
     * Popup user record widget clicked.
     */
    void popupUserRecordWidget();
    /*!
     * Reset current user name.
     */
    void resetUserName(const QString &name);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Create current user all login time to show.
     */
    void createUserTime() const;
    /*!
     * Create functions menu popup initialized.
     */
    void createButtonPopMenu();

    Ui::MusicUserManagerDialog *m_ui;
    MusicUserModel *m_userModel;
    MusicUserUIDItem m_userUID;
    QTime m_time;
    QMenu m_popMenu;

};

#endif // MUSICUSERMANAGERDIALOG_H
