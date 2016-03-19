#ifndef MUSICLOCALSONGSEARCH_H
#define MUSICLOCALSONGSEARCH_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include "musicglobaldefine.h"

class QLabel;
class QToolButton;
class MusicLocalSongSearchEdit;

/*! @brief The class of the local song search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearch : public QDialog
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearch(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLocalSongSearch();

    QString getSearchedText() const;
    /*!
     * Get the search text that the user searched.
     */
    void clearSearchedText() const;
    /*!
     * Clear the search text.
     */

public Q_SLOTS:
    bool close();
    /*!
     * Override the close function.
     */

private:
    QLabel *m_showIcon;
    QToolButton *m_closeButton;
    MusicLocalSongSearchEdit *m_searchLine;

};

#endif // MUSICLOCALSONGSEARCH_H
