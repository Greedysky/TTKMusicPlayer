#ifndef MUSICSONGSLISTPLAYWIDGET_H
#define MUSICSONGSLISTPLAYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include "musicglobaldefine.h"

class MusicSplitItemClickedLabel;
class MusicSongsToolItemRenamedWidget;

/*! @brief The class of the song list play widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListPlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListPlayWidget(int index, QWidget *parent = 0);
    /*!
     * Object contsructor by index.
     */
    virtual ~MusicSongsListPlayWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void insertTimerLabel(const QString &t) const;
    /*!
     * Update item time label time.
     */
    void updateCurrentArtist();
    /*!
     * Update current artist when it download finished.
     */
    void setParameter(const QString &name, const QString &path);
    /*!
     * Update item artist label by name.
     */
    void setItemRename();
    /*!
     * Rename item artist label name.
     */
    bool getItemRenameState() { return m_renameLine != nullptr;}
    /*!
     * Get rename item artist label state.
     */

    inline void setCurrentPlayIndex(int index) { m_currentPlayIndex = index;}
    /*!
     * Set current play index.
     */
    inline int getCurrentPlayIndex() const { return m_currentPlayIndex;}
    /*!
     * Get current play index.
     */

Q_SIGNALS:
    void renameFinished(const QString &text);
    /*!
     * Rename item artist label is finised it emit.
     */
    void enterChanged(int row, int column);
    /*!
     * Mouse is the item geometry, emit row and column.
     */

public Q_SLOTS:
    void deleteRenameItem();
    /*!
     * Delete rename item.
     */
    void setChangItemName(const QString &name);
    /*!
     * Rename item artist label is finised.
     */
    void currentLoveStateClicked();
    /*!
     * Reset current music love icon state.
     */
    void currentDownloadStateClicked();
    /*!
     * Reset current music download icon state.
     */

protected:
    virtual void enterEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createMoreMenu(QMenu *menu);
    /*!
     * Create more menu information.
     */
    bool showArtPicture(const QString &name) const;
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */

    QString m_totalTime;
    int m_currentPlayIndex;
    QLabel *m_artPictureLabel, *m_timeLabel;
    MusicSplitItemClickedLabel *m_songNameLabel;
    QPushButton *m_loveButton, *m_deleteButton,* m_showMVButton;
    QPushButton *m_downloadButton, *m_moreButton;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};

#endif // MUSICSONGSLISTPLAYWIDGET_H
