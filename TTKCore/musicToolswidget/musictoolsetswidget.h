#ifndef MUSICTOOLSETSWIDGET_H
#define MUSICTOOLSETSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QListWidget>
#include "musicuiobject.h"
#include "musicglobaldefine.h"

class QProcess;

/*! @brief The class of the tool sets widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicToolSetsWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicToolSetsWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicToolSetsWidget();

Q_SIGNALS:
    void getCurrentPlayList(QStringList &list);
    /*!
     * Get current play lists.
     */
    void showSpectrumWidget();
    /*!
     * Show dpectrum widget dialog.
     */

public Q_SLOTS:
    void itemHasClicked(QListWidgetItem *item);
    /*!
     * Tool sets list item has clicked.
     */
    void addListWidgetItem();
    /*!
     * Add tool sets list into list widget.
     */

protected:
    void clearAllItems();
    /*!
     * Clear All Items.
     */

    QProcess *m_process;

};

#endif // MUSICTOOLSETSWIDGET_H
