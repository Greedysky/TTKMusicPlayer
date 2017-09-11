#ifndef MUSICTOOLSETSWIDGET_H
#define MUSICTOOLSETSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicabstractmovewidget.h"

class QListWidgetItem;
namespace Ui {
class MusicToolSetsWidget;
}

/*! @brief The class of the tool sets widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicToolSetsWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicToolSetsWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicToolSetsWidget();

    static QString getClassName();
    /*!
     * Get class object name.
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
    void resetFlag(MusicObject::ToolsType flag);
    /*!
     * Reset window open flag.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    void clearAllItems();
    /*!
     * Clear All Items.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    Ui::MusicToolSetsWidget *m_ui;
    MusicObject::ToolsTypes m_toolsFlags;

};

#endif // MUSICTOOLSETSWIDGET_H
