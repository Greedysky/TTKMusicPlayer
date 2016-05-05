#ifndef MUSICLRCLOCALLINKWIDGET_H
#define MUSICLRCLOCALLINKWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the local lrc link table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcLocalLinkTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicLrcLocalLinkTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

public Q_SLOTS:
    virtual void listCellClicked(int, int) override {}
    /*!
     * Table widget list cell click.
     */

};


typedef struct LocalDataItem{
    QString m_name;
    QString m_path;
}LocalDataItem;
typedef QList<LocalDataItem> LocalDataItems;

namespace Ui {
class MusicLrcLocalLinkWidget;
}

/*! @brief The class of the local lrc link widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcLocalLinkWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcLocalLinkWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcLocalLinkWidget();

    void setCurrentSongName(const QString &name);
    /*!
     * Set current song name.
     */
    void createAllItems(const LocalDataItems &items);
    /*!
     * Create all items.
     */

Q_SIGNALS:
    void currentLrcChanged(const QString &data);
    /*!
     * Current lrc changed by user, and emit Lrc flag.
     */

public Q_SLOTS:
    void fuzzyStateChanged();
    /*!
     * Search fuzzy state changed.
     */
    void findInLocalFile();
    /*!
     * Find in local file button clicked.
     */
    void deleteFoundLrc();
    /*!
     * Delete found lrc button clicked.
     */
    void confirmButtonClicked();
    /*!
     * Confirm button clicked.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void searchInLocalMLrc();
    /*!
     * Search local lrc in MLrc dir.
     */

    Ui::MusicLrcLocalLinkWidget *ui;

};

#endif // MUSICLRCLOCALLINKWIDGET_H
