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
#include "musicabstractmovewidget.h"
#include <QFileInfo>

class QFileSystemWatcher;
class MusicLocalSongsManagerThread;

namespace Ui {
class MusicLocalSongsManagerWidget;
}

/*! @brief The class of the lcal songs manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsManagerWidget : public MusicAbstractMoveWidget
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

    void findExtraDevicePath(const QString &dir);
    /*!
     * Find extra device path.
     */

Q_SIGNALS:
    void resetFlag(MusicObject::ToolsType flag);
    /*!
     * Reset window open flag.
     */
    void addSongToPlay(const QStringList &names);
    /*!
     * Add current selected song to play lists.
     */

public Q_SLOTS:
    void selectedAllItems(bool check);
    /*!
     * Select all items.
     */
    void watchDirEnable(bool enable);
    /*!
     * Watch dir enable or not.
     */
    void watchDirSelected();
    /*!
     * Watch dir selected.
     */
    void watchDirChanged(const QString &path);
    /*!
     * Watch dir path changed.
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
    void updateFileLists(const QFileInfoList &list);
    /*!
     * Update file lists.
     */

    void setShowlistButton();
    /*!
     * Select to show list mode.
     */
    void setShowArtButton();
    /*!
     * Select to show art mode.
     */
    void setShowAlbumButton();
    /*!
     * Select to show album mode.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Override the widget event.
     */
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
    void controlEnable(bool state);
    /*!
     * Control enable or disable.
     */
    void loadingLabelState(bool state);
    /*!
     * Loading label disable.
     */

    Ui::MusicLocalSongsManagerWidget *m_ui;
    QFileInfoList m_fileNames;
    MusicLocalSongsManagerThread *m_thread;
    MusicObject::MIntsListMap m_searchfileListCache;
    QFileSystemWatcher *m_fileSystemWatcher;

};

#endif // MUSICLOCALSONGSMANAGERWIDGET_H
