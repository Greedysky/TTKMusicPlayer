#ifndef MUSICENHANCEDWIDGET_H
#define MUSICENHANCEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QToolButton>
#include "musicglobaldefine.h"

/*! @brief The class of the enhanced widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicEnhancedWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit MusicEnhancedWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicEnhancedWidget();

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

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    QMenu *m_menu;
    QToolButton *m_caseButton, *m_Button1, *m_Button2;
    QToolButton *m_Button3, *m_Button4;

};

#endif // MUSICENHANCEDWIDGET_H
