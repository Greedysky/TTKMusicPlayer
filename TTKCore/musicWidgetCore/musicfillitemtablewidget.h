#ifndef MUSICFILLITEMTABLEWIDGET_H
#define MUSICFILLITEMTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"

class MusicQueryTableDelegate;

/*! @brief The class of the fill item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFillItemTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicFillItemTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicFillItemTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    MusicObject::MIntList getSelectedItems() const;
    /*!
     * Get selected items.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    void setSelectedAllItems(bool all);
    /*!
     * Set select all items.
     */

protected:
    MusicQueryTableDelegate *m_checkBoxDelegate;


};

#endif // MUSICFILLITEMTABLEWIDGET_H
