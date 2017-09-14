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
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsItemSelectedTableWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Create all items.
     */
    void createAllItems(MusicSongItems *items);
    /*!
     * Get selected items.
     */
    MusicObject::MIntList getSelectedItems() const;

public Q_SLOTS:
    /*!
     * Select all items.
     */
    void selectedAllItems(bool check);

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
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsItemSelectedDialog(QWidget *parent = 0);

    virtual ~MusicSongCheckToolsItemSelectedDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Create all items.
     */
    void createAllItems(MusicSongItems *items);

Q_SIGNALS:
    /*!
     * Item lists changed.
     */
    void itemListsChanged(const MusicObject::MIntList &items);

public Q_SLOTS:
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicSongCheckToolsItemSelectedDialog *m_ui;

};

#endif // MUSICSONGCHECKTOOLSITEMSELECTEDDIALOG_H
