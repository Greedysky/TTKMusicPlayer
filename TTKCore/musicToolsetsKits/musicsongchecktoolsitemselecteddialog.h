#ifndef MUSICSONGCHECKTOOLSITEMSELECTEDDIALOG_H
#define MUSICSONGCHECKTOOLSITEMSELECTEDDIALOG_H

#include "musicsong.h"
#include "musicabstractmovedialog.h"
#include "musicfillitemtablewidget.h"

/*! @brief The class of the song check tools item selected table widget
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsItemSelectedTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongCheckToolsItemSelectedTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createAllItems(MusicSongItems *items);
    /*!
     * Create all items.
     */
    MusicObject::MIntList getSelectedItems() const;
    /*!
     * Get selected items.
     */

public Q_SLOTS:
    void selectedAllItems(bool check);
    /*!
     * Select all items.
     */

};


namespace Ui {
class MusicSongCheckToolsItemSelectedDialog;
}

/*! @brief The class of the song check tools item selected dialog widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongCheckToolsItemSelectedDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSongCheckToolsItemSelectedDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongCheckToolsItemSelectedDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void createAllItems(MusicSongItems *items);
    /*!
     * Create all items.
     */

Q_SIGNALS:
    void itemListsChanged(const MusicObject::MIntList &items);
    /*!
     * Item lists changed.
     */

public Q_SLOTS:
    void confirmButtonClicked();
    /*!
     * Confirm button clicked.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicSongCheckToolsItemSelectedDialog *m_ui;

};

#endif // MUSICSONGCHECKTOOLSITEMSELECTEDDIALOG_H
