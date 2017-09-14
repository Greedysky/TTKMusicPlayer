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
    /*!
     * Object contsructor.
     */
    explicit MusicFillItemTableWidget(QWidget *parent = 0);

    virtual ~MusicFillItemTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get selected items.
     */
    MusicObject::MIntList getSelectedItems() const;

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Set select all items.
     */
    void setSelectedAllItems(bool all);

protected:
    MusicQueryTableDelegate *m_checkBoxDelegate;

};

#endif // MUSICFILLITEMTABLEWIDGET_H
