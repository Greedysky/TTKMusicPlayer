#ifndef MUSICCONNECTTRANSFERTABLEWIDGET_H
#define MUSICCONNECTTRANSFERTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicitemdelegate.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the transfer file list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectTransferTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicConnectTransferTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicConnectTransferTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void cancelAllSelectedItems();
    /*!
     * Cancel all selected items.
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

protected:
    MusicQueryTableDelegate *m_checkBoxDelegate;

};

#endif // MUSICCONNECTTRANSFERTABLEWIDGET_H
