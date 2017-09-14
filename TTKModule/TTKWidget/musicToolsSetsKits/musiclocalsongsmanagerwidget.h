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
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsManagerWidget(QWidget *parent = 0);

    virtual ~MusicLocalSongsManagerWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Find extra device path.
     */
    void findExtraDevicePath(const QString &dir);

Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);
    /*!
     * Add current selected song to play lists.
     */
    void addSongToPlay(const QStringList &names);

public Q_SLOTS:
    /*!
     * Select all items.
     */
    void selectedAllItems(bool check);
    /*!
     * Watch dir enable or not.
     */
    void watchDirEnable(bool enable);
    /*!
     * Watch dir selected.
     */
    void watchDirSelected();
    /*!
     * Watch dir path changed.
     */
    void watchDirChanged(const QString &path);
    /*!
     * Music song audition play.
     */
    void auditionButtonClick();
    /*!
     * Add music song to play list.
     */
    void addButtonClick();
    /*!
     * Item cell on click by row and col.
     */
    void itemCellOnClick(int row, int col);
    /*!
     * Item cell on double click by row and col.
     */
    void itemDoubleClicked(int row, int col);
    /*!
     * Send the searched file or path.
     */
    void setSongNamePath(const QFileInfoList &name);
    /*!
     * Start to fetch file or files.
     */
    void filterScanChanged(int index);
    /*!
     * Search file from list.
     */
    void musicSearchIndexChanged(int row, int col);
    /*!
     * Update file lists.
     */
    void updateFileLists(const QFileInfoList &list);

    /*!
     * Select to show list mode.
     */
    void setShowlistButton();
    /*!
     * Select to show art mode.
     */
    void setShowArtButton();
    /*!
     * Select to show album mode.
     */
    void setShowAlbumButton();
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Clear All Items.
     */
    void clearAllItems();
    /*!
     * Add all file items into list.
     */
    void addAllItems(const QFileInfoList &name);
    /*!
     * Add current system all supported drives list.
     */
    void addDrivesList();
    /*!
     * Set current item or items selected.
     */
    void itemsSelected();
    /*!
     * Current drive index changed.
     */
    bool filterIndexChanged();
    /*!
     * Current custom dir path changed.
     */
    bool filterIndexCustChanged();
    /*!
     * Control enable or disable.
     */
    void controlEnable(bool state);
    /*!
     * Loading label disable.
     */
    void loadingLabelState(bool state);

    Ui::MusicLocalSongsManagerWidget *m_ui;
    QFileInfoList m_fileNames;
    MusicLocalSongsManagerThread *m_thread;
    MusicObject::MIntsListMap m_searchfileListCache;
    QFileSystemWatcher *m_fileSystemWatcher;

};

#endif // MUSICLOCALSONGSMANAGERWIDGET_H
