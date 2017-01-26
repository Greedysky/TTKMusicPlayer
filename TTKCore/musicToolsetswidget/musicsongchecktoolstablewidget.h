#ifndef MUSICSONGCHECKTOOLSTABLEWIDGET_H
#define MUSICSONGCHECKTOOLSTABLEWIDGET_H

#include "musicsong.h"
#include "musicfillitemtablewidget.h"

class MusicSongCheckToolsRenameTableWidget : public MusicFillItemTableWidget
{
public:
    explicit MusicSongCheckToolsRenameTableWidget(QWidget *parent = 0);
    virtual ~MusicSongCheckToolsRenameTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createAllItems(const MusicSongs &songs);
    /*!
     * Create all items.
     */
    void selectedAllItems(bool check);
    /*!
     * Select all items.
     */

};

#endif // MUSICSONGCHECKTOOLSTABLEWIDGET_H
