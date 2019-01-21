#ifndef MUSICSONGITEMSELECTEDDIALOG_H
#define MUSICSONGITEMSELECTEDDIALOG_H

#include "musicsong.h"
#include "musicabstractmovedialog.h"
#include "musicfillitemtablewidget.h"

/*! @brief The class of the song item selected table widget
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongItemSelectedTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongItemSelectedTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongItemSelectedTableWidget(QWidget *parent = nullptr);

    /*!
     * Create all items.
     */
    void createAllItems(MusicSongItems *items);
    /*!
     * Get selected items.
     */
    MIntList getSelectedItems() const;

public Q_SLOTS:
    /*!
     * Select all items.
     */
    void selectedAllItems(bool check);

};


namespace Ui {
class MusicSongItemSelectedDialog;
}

/*! @brief The class of the song item selected dialog widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongItemSelectedDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongItemSelectedDialog)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongItemSelectedDialog(QWidget *parent = nullptr);

    virtual ~MusicSongItemSelectedDialog();

    /*!
     * Create all items.
     */
    void createAllItems(MusicSongItems *items);

Q_SIGNALS:
    /*!
     * Item lists changed.
     */
    void itemListsChanged(const MIntList &items);

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
    Ui::MusicSongItemSelectedDialog *m_ui;

};

#endif // MUSICSONGITEMSELECTEDDIALOG_H
