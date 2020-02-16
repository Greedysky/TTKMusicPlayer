#ifndef MUSICSCREENSAVERWIDGET_H
#define MUSICSCREENSAVERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "musicglobaldefine.h"

/*! @brief The class of the screen saver widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicScreenSaverWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicScreenSaverWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicScreenSaverWidget(QWidget *parent = nullptr);

    virtual ~MusicScreenSaverWidget();

    /*!
     * Apply settings parameters.
     */
    void applySettingParameter();

private Q_SLOTS:
    /*!
     * Input data changed
     */
    void inputDataChanged();
    /*!
     * Case button on and off.
     */
    void caseButtonOnAndOff();

private:
    QLineEdit *m_inputEdit;
    QPushButton *m_caseButton;
};



/*! @brief The class of the screen saver background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicScreenSaverBackgroundWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicScreenSaverBackgroundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicScreenSaverBackgroundWidget(QWidget *parent = nullptr);

    virtual ~MusicScreenSaverBackgroundWidget();

    /*!
     * Apply settings parameters.
     */
    void applySettingParameter();

private Q_SLOTS:
    /*!
     * Screen saver time out.
     */
    void timeout();

private:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool m_state;
    bool m_isRunning;
    QTimer *m_timer;

};

#endif // MUSICSCREENSAVERWIDGET_H
