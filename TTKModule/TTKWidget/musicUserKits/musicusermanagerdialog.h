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
    /*!
     * Object contsructor.
     */
    explicit MusicUserManagerDialog(QWidget *parent = 0);

    ~MusicUserManagerDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current user's uid.
     */
    void setUserUID(const QString &uid);
    /*!
     * Set user model.
     */
    void setUserModel(MusicUserModel *model);

Q_SIGNALS:
    /*!
     * User login state changed.
     */
    void userStateChanged(const QString &uid, const QString &icon);

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
    MusicUserModel* m_userModel;
    QString m_currentUserUID;
    QTime m_time;
    QMenu m_popMenu;


};

#endif // MUSICUSERMANAGERDIALOG_H
