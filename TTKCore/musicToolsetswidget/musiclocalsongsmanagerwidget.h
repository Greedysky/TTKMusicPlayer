#ifndef MUSICLOCALSONGSMANAGERWIDGET_H
#define MUSICLOCALSONGSMANAGERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovedialog.h"
#include <QFileInfo>

class MusicLocalSongsManagerThread;

namespace Ui {
class MusicLocalSongsManagerWidget;
}

/*! @brief The class of the lcal songs manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsManagerWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLocalSongsManagerWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLocalSongsManagerWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void addSongToPlay(const QStringList &names);
    /*!
     * Add current selected song to play lists.
     */

public Q_SLOTS:
    void selectedAllItems(bool check);
    /*!
     * Select all items.
     */
    void auditionButtonClick();
    /*!
     * Music song audition play.
     */
    void addButtonClick();
    /*!
     * Add music song to play list.
     */
    void itemCellOnClick(int row, int col);
    /*!
     * Item cell on click by row and col.
     */
    void itemDoubleClicked(int row, int col);
    /*!
     * Item cell on double click by row and col.
     */
    void setSongNamePath(const QFileInfoList &name);
    /*!
     * Send the searched file or path.
     */
    void filterScanChanged(int index);
    /*!
     * Start to fetch file or files.
     */
    void musicSearchIndexChanged(int row, int col);
    /*!
     * Search file from list.
     */

    void setShowlistButton();
    /*!
     * Select to show list mode.
     */
    void setShowPathButton();
    /*!
     * Select to show path mode.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void clearAllItems();
    /*!
     * Clear All Items.
     */
    void addAllItems(const QFileInfoList &name);
    /*!
     * Add all file items into list.
     */
    void addDrivesList();
    /*!
     * Add current system all supported drives list.
     */
    void itemsSelected();
    /*!
     * Set current item or items selected.
     */
    bool filterIndexChanged();
    /*!
     * Current drive index changed.
     */
    bool filterIndexCustChanged();
    /*!
     * Current custom dir path changed.
     */

    Ui::MusicLocalSongsManagerWidget *m_ui;
    int m_currentIndex;
    QFileInfoList m_filenames;
    MusicLocalSongsManagerThread *m_thread;
    MusicObject::MIntsListMap m_searchfileListCache;

};

#endif // MUSICLOCALSONGSMANAGERWIDGET_H
