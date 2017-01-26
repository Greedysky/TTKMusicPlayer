#ifndef MUSICSONGCHECKTOOLSTABLEWIDGET_H
#define MUSICSONGCHECKTOOLSTABLEWIDGET_H

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

};

#endif // MUSICSONGCHECKTOOLSTABLEWIDGET_H
