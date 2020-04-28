#ifndef MUSICSONGITEMSELECTEDAREAWIDGET_H
#define MUSICSONGITEMSELECTEDAREAWIDGET_H

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
    TTKIntList getSelectedItems() const;

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
    void itemListsChanged(const TTKIntList &items);

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


class MusicClickedLabel;

/*! @brief The class of the song item selected area widget
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongItemSelectedAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongItemSelectedAreaWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongItemSelectedAreaWidget(QWidget *parent = nullptr);

    virtual ~MusicSongItemSelectedAreaWidget();

    /*!
     * Get selected song items.
     */
    MusicSongs getSelectedSongItems();

Q_SIGNALS:
    /*!
     * Get music datas from container.
     */
    void getMusicLists(MusicSongItems &songs);
    /*!
     * Confirm changed.
     */
    void confirmChanged();

public Q_SLOTS:
    /*!
     * Modified item button clicked.
     */
    void modifiedItemButtonClicked();
    /*!
     * Item lists changed.
     */
    void itemListsChanged(const TTKIntList &items);

protected:
    bool m_selected;
    TTKIntList m_selectedItems;

    QLabel *m_label, *m_itemLabel;
    MusicClickedLabel *m_modifiedItemButton;
};

#endif // MUSICSONGITEMSELECTEDAREAWIDGET_H
