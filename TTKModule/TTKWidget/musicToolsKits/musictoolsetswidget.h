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
    /*!
     * Object contsructor.
     */
    explicit MusicToolSetsWidget(QWidget *parent = 0);

    virtual ~MusicToolSetsWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();


public Q_SLOTS:
    /*!
     * Tool sets list item has clicked.
     */
    void itemHasClicked(QListWidgetItem *item);
    /*!
     * Add tool sets list into list widget.
     */
    void addListWidgetItem();
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Clear All Items.
     */
    void clearAllItems();
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    Ui::MusicToolSetsWidget *m_ui;
    MusicObject::ToolsTypes m_toolsFlags;

};

#endif // MUSICTOOLSETSWIDGET_H
