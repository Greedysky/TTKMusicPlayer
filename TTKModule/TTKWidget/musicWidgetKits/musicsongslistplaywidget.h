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
#include "musicglobaldefine.h"

class QMenu;
class QPushButton;
class MusicSplitItemClickedLabel;
class MusicSongsToolItemRenamedWidget;

/*! @brief The class of the song list play widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListPlayWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor by index.
     */
    explicit MusicSongsListPlayWidget(int index, QWidget *parent = 0);

    virtual ~MusicSongsListPlayWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Update item time label time.
     */
    void insertTimerLabel(const QString &time, const QString &total);
    /*!
     * Update current artist when it download finished.
     */
    void updateCurrentArtist();
    /*!
     * Update item artist label by name.
     */
    void setParameter(const QString &name, const QString &path);
    /*!
     * Rename item artist label name.
     */
    void setItemRename();
    /*!
     * Get rename item artist label state.
     */
    bool getItemRenameState() { return m_renameLine != nullptr;}

    /*!
     * Set current play index.
     */
    inline void setCurrentPlayIndex(int index) { m_currentPlayIndex = index;}
    /*!
     * Get current play index.
     */
    inline int getCurrentPlayIndex() const { return m_currentPlayIndex;}

Q_SIGNALS:
    /*!
     * Rename item artist label is finised it emit.
     */
    void renameFinished(const QString &text);
    /*!
     * Mouse is the item geometry, emit row and column.
     */
    void enterChanged(int row, int column);

public Q_SLOTS:
    /*!
     * Delete rename item.
     */
    void deleteRenameItem();
    /*!
     * Rename item artist label is finised.
     */
    void setChangItemName(const QString &name);
    /*!
     * Reset current music love icon state.
     */
    void currentLoveStateClicked();
    /*!
     * Reset current music download icon state.
     */
    void currentDownloadStateClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    /*!
     * Create more menu information.
     */
    void createMoreMenu(QMenu *menu);
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */
    bool showArtPicture(const QString &name) const;

    bool m_noCover;
    int m_currentPlayIndex;
    QString m_totalTimeLabel;
    QLabel *m_artPictureLabel, *m_timeLabel;
    MusicSplitItemClickedLabel *m_songNameLabel;
    QPushButton *m_loveButton, *m_deleteButton,* m_showMVButton;
    QPushButton *m_downloadButton, *m_moreButton;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};

#endif // MUSICSONGSLISTPLAYWIDGET_H
