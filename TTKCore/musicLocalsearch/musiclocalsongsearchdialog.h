#ifndef MUSICLOCALSONGSEARCHDIALOG_H
#define MUSICLOCALSONGSEARCHDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include "musicglobaldefine.h"

class MusicLocalSongSearchEdit;

/*! @brief The class of the local song search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLocalSongSearchDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */
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
    MusicLocalSongSearchEdit *m_searchLine;

};

#endif // MUSICLOCALSONGSEARCHDIALOG_H
