#ifndef MUSICREMOTEWIDGETFORSTRIP_H
#define MUSICREMOTEWIDGETFORSTRIP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

class MusicMarqueeWidget;

/*! @brief The class of the desktop strip remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForStrip : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForStrip(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRemoteWidgetForStrip();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void setLabelText(const QString &value) override;
    /*!
     * Set current song text.
     */

private Q_SLOTS:
    void windowStateChanged();
    /*!
     * Window statec hanged.
     */

protected:
    QPushButton *m_lrcButton, *m_wallPaperButton, *m_listButton;
    QPushButton *m_windowStateButton;
    QToolButton *m_closeButton;
    MusicMarqueeWidget *m_songNameLabel;
    QWidget *m_bottomWidget;

};

#endif // MUSICREMOTEWIDGETFORSTRIP_H
