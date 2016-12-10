#ifndef MUSICUSERMANAGERDIALOG_H
#define MUSICUSERMANAGERDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTime>
#include <QMenu>
#include "musicabstractmovedialog.h"

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
public:
    explicit MusicUserManagerDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicUserManagerDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setUserUID(const QString &uid);
    /*!
     * Set current user's uid.
     */

Q_SIGNALS:
    void userStateChanged(const QString &uid, const QString &icon);
    /*!
     * User login state changed.
     */

public Q_SLOTS:
    void musicUserLogoff();
    /*!
     * User logout clicked.
     */
    void popupUserRecordWidget();
    /*!
     * Popup user record widget clicked.
     */
    void resetUserName(const QString &name);
    /*!
     * Reset current user name.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createUserTime() const;
    /*!
     * Create current user all login time to show.
     */
    void createButtonPopMenu();
    /*!
     * Create functions menu popup initialized.
     */

    Ui::MusicUserManagerDialog *m_ui;
    MusicUserModel* m_userModel;
    QString m_currentUserUID;
    QTime m_time;
    QMenu m_popMenu;


};

#endif // MUSICUSERMANAGERDIALOG_H
