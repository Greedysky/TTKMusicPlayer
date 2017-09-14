#ifndef MUSICABSTRACTTABLEWIDGET_H
#define MUSICABSTRACTTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicAbstractTableWidget(QWidget *parent = 0);

    virtual ~MusicAbstractTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Clear current items.
     */
    void clear();

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column);
    /*!
     * Table widget list cell click.
     * Subclass should implement this function.
     */
    virtual void listCellClicked(int row, int column) = 0;

protected:
    /*!
     * Set selected item row color.
     */
    void setRowColor(int row, const QColor &color) const;

    QColor m_defaultBkColor;
    int m_previousColorRow;
    int m_previousClickRow;

};

#endif // MUSICABSTRACTTABLEWIDGET_H
