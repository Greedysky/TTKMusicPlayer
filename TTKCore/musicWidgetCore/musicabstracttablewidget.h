#ifndef MUSICABSTRACTTABLEWIDGET_H
#define MUSICABSTRACTTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTableWidget>
#include <QHeaderView>

#include "musicglobaldefine.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"

/*! @brief The class of the table widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAbstractTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MusicAbstractTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicAbstractTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void clear();
    /*!
     * Clear current items.
     */

public Q_SLOTS:
    virtual void listCellEntered(int row, int column);
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellClicked(int row, int column) = 0;
    /*!
     * Table widget list cell click.
     * Subclass should implement this function.
     */

protected:
    void setRowColor(int row, const QColor &color) const;
    /*!
     * Set selected item row color.
     */

    QColor m_defaultBkColor;
    int m_previousColorRow;
    int m_previousClickRow;

};

#endif // MUSICABSTRACTTABLEWIDGET_H
