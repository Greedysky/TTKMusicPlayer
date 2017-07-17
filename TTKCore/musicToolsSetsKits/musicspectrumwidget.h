#ifndef MUSICSPECTRUMWIDGET_H
#define MUSICSPECTRUMWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSpectrumWidget;
}

/*! @brief The class of the music spectrum widget all.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicSpectrumWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSpectrumWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void resetFlag(MusicObject::ToolsType flag);
    /*!
     * Reset window open flag.
     */

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Override the widget event.
     */

    Ui::MusicSpectrumWidget *m_ui;

};

#endif // MUSICSPECTRUMWIDGET_H
