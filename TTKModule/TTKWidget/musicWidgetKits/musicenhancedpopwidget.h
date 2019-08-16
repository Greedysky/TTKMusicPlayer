#ifndef MUSICENHANCEDPOPWIDGET_H
#define MUSICENHANCEDPOPWIDGET_H

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

#include "musictoolmenuwidget.h"

class QLabel;
class QPropertyAnimation;

/*! @brief The class of the enhanced tool button.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicEnhancedToolButton : public QToolButton
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicEnhancedToolButton)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicEnhancedToolButton(QWidget *parent = nullptr);

    virtual ~MusicEnhancedToolButton();

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

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;

    bool m_state;
    QLabel *m_foreLabel, *m_animationLabel;
    QPropertyAnimation *m_animation;
};


/*! @brief The class of the enhanced pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicEnhancedPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicEnhancedPopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicEnhancedPopWidget(QWidget *parent = nullptr);

    virtual ~MusicEnhancedPopWidget();

Q_SIGNALS:
    /*!
     * Set enhanced music config changed return selected index.
     */
    void enhancedMusicChanged(int type);

public Q_SLOTS:
    /*!
     * Set enhanced music config.
     */
    void setEnhancedMusicConfig(int type);
    /*!
     * Case button on and off.
     */
    void caseButtonOnAndOff();
    /*!
     * Button animation state changed.
     */
    void buttonAnimationChanged(bool state);
    /*!
     * Help button clicked.
     */
    void helpButtonClicked();

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    int m_lastSelectedIndex;
    QToolButton *m_caseButton;
    QList<MusicEnhancedToolButton*> m_buttons;

};

#endif // MUSICENHANCEDPOPWIDGET_H
