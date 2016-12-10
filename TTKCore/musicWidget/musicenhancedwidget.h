#ifndef MUSICENHANCEDWIDGET_H
#define MUSICENHANCEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"
#include "musictoolmenuwidget.h"

class QLabel;
class QPropertyAnimation;

/*! @brief The class of the enhanced tool button.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicEnhancedToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit MusicEnhancedToolButton(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicEnhancedToolButton();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setStyleSheet(const QString &styleSheet, bool state = false);
    /*!
     * Set object style sheet.
     */
    void start();
    /*!
     * Start label animation .
     */
    void stop();
    /*!
     * Stop label animation.
     */

private Q_SLOTS:
    void finished();
    /*!
     * Label animation just finished.
     */

protected:
    virtual void enterEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_state;
    QLabel *m_foreLabel, *m_animationLabel;
    QPropertyAnimation *m_animation;
};


/*! @brief The class of the enhanced widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicEnhancedWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicEnhancedWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicEnhancedWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void enhancedMusicChanged(int type);
    /*!
     * Set enhanced music config changed return selected index.
     */

public Q_SLOTS:
    void setEnhancedMusicConfig(int type);
    /*!
     * Set enhanced music config.
     */
    void caseButtonOnAndOff();
    /*!
     * Case button on and off.
     */
    void buttonAnimationChanged(bool state);
    /*!
     * Button animation state changed.
     */

    void helpButtonClicked();
    /*!
     * Help button clicked.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    int m_lastSelectedIndex;
    QToolButton *m_caseButton;
    QList<MusicEnhancedToolButton*> m_buttons;

};

#endif // MUSICENHANCEDWIDGET_H
