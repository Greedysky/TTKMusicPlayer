#ifndef MUSICSONGCHECKTOOLSTABLEWIDGET_H
#define MUSICSONGCHECKTOOLSTABLEWIDGET_H

#include "musicsong.h"
#include "musicfillitemtablewidget.h"

/*! @brief The class of the song check tools rename table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsRenameTableWidget : public MusicFillItemTableWidget
{
public:
    explicit MusicSongCheckToolsRenameTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
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



/*! @brief The class of the song check tools duplicate table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsDuplicateTableWidget : public MusicFillItemTableWidget
{
public:
    explicit MusicSongCheckToolsDuplicateTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongCheckToolsDuplicateTableWidget();

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



/*! @brief The class of the song check tools quality table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsQualityTableWidget : public MusicFillItemTableWidget
{
public:
    explicit MusicSongCheckToolsQualityTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongCheckToolsQualityTableWidget();

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
