#ifndef MUSICLOCALSONGSEARCHDIALOG_H
#define MUSICLOCALSONGSEARCHDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchDialog(QWidget *parent = 0);

    ~MusicLocalSongSearchDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Get the search text that the user searched.
     */
    QString getSearchedText() const;
    /*!
     * Clear the search text.
     */
    void clearSearchedText() const;

public Q_SLOTS:
    /*!
     * Override the close function.
     */
    bool close();

private:
    MusicLocalSongSearchEdit *m_searchLine;

};

#endif // MUSICLOCALSONGSEARCHDIALOG_H
