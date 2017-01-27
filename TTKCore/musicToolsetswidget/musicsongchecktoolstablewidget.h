#ifndef MUSICSONGCHECKTOOLSTABLEWIDGET_H
#define MUSICSONGCHECKTOOLSTABLEWIDGET_H

#include "musicsongchecktoolsunit.h"
#include "musicfillitemtablewidget.h"

/*! @brief The class of the song check tools rename table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsRenameTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongCheckToolsRenameTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createAllItems(const SongCheckToolsRenames &items);
    /*!
     * Create all items.
     */

public Q_SLOTS:
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
    Q_OBJECT
public:
    explicit MusicSongCheckToolsDuplicateTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createAllItems(const SongCheckToolsDuplicates &songs);
    /*!
     * Create all items.
     */

public Q_SLOTS:
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
    Q_OBJECT
public:
    explicit MusicSongCheckToolsQualityTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createAllItems(const SongCheckToolsQualitys &songs);
    /*!
     * Create all items.
     */
    void transfromBitrateToQuality(int bitrate, QString &bitString, QColor &color) const;
    /*!
     * Transfrom bitrate to quality.
     */

public Q_SLOTS:
    void selectedAllItems(bool check);
    /*!
     * Select all items.
     */

};

#endif // MUSICSONGCHECKTOOLSTABLEWIDGET_H
