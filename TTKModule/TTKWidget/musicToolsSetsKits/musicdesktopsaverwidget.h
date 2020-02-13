#ifndef MUSICDESKTOPSAVERWIDGET_H
#define MUSICDESKTOPSAVERWIDGET_H

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

/*! @brief The class of the desktop saver widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicDesktopSaverWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDesktopSaverWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDesktopSaverWidget(QWidget *parent = nullptr);

    virtual ~MusicDesktopSaverWidget();

    /*!
     * Apply settings parameters.
     */
    void applySettingParameter();

private Q_SLOTS:
    /*!
     * Case button on and off.
     */
    void caseButtonOnAndOff();

private:
    QLineEdit *m_inputEdit;
    QPushButton *m_caseButton;
};



/*! @brief The class of the desktop saver background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicDesktopSaverBackgroundWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDesktopSaverBackgroundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDesktopSaverBackgroundWidget(QWidget *parent = nullptr);

    virtual ~MusicDesktopSaverBackgroundWidget();

    /*!
     * Apply settings parameters.
     */
    void applySettingParameter();

private Q_SLOTS:
    /*!
     * Desktop saver time out.
     */
    void timeout();

private:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool m_isRunning;
    QTimer *m_timer;

};

#endif // MUSICDESKTOPSAVERWIDGET_H
