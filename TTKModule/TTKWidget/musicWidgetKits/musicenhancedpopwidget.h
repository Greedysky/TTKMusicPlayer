#ifndef MUSICENHANCEDPOPWIDGET_H
#define MUSICENHANCEDPOPWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#include "musictoolmenuwidget.h"

class QLabel;
class QPropertyAnimation;

/*! @brief The class of the enhanced tool button.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicEnhancedToolButton : public QToolButton
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicEnhancedToolButton)
public:
    /*!
     * Object constructor.
     */
    explicit MusicEnhancedToolButton(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicEnhancedToolButton();

    /*!
     * Set object style sheet.
     */
    void setStyleSheet(const QString &styleSheet, bool state = false);
    /*!
     * Start label animation .
     */
    void start();
    /*!
     * Stop label animation.
     */
    void stop();

private Q_SLOTS:
    /*!
     * Label animation just finished.
     */
    void finished();

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;

    bool m_state;
    QLabel *m_foreLabel, *m_animationLabel;
    QPropertyAnimation *m_animation;

};


/*! @brief The class of the enhanced pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicEnhancedPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicEnhancedPopWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicEnhancedPopWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicEnhancedPopWidget();

Q_SIGNALS:
    /*!
     * Set enhanced song config changed return selected index.
     */
    void enhancedSongChanged(int type);

public Q_SLOTS:
    /*!
     * Set enhanced song config.
     */
    void setEnhancedSongConfig(int type);
    /*!
     * switch button on or off.
     */
    void switchButtonState();
    /*!
     * Button animation state changed.
     */
    void buttonAnimationChanged(bool state);
    /*!
     * Help button clicked.
     */
    void helpButtonClicked();

private:
    /*!
     * Create all widget in layout.
     */
    void initialize();

    int m_lastSelectedIndex;
    QToolButton *m_caseButton;
    QList<MusicEnhancedToolButton*> m_buttons;

};

#endif // MUSICENHANCEDPOPWIDGET_H
