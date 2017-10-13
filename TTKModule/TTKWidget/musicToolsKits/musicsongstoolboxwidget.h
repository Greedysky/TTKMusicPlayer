#ifndef MUSICSONGSTOOLBOXWIDGET_H
#define MUSICSONGSTOOLBOXWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QLabel>
#include <QBoxLayout>
#include "musicglobaldefine.h"

class MusicSort;
class MusicSongsToolItemRenamedWidget;

/*! @brief The class of the tool box top widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsToolBoxTopWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsToolBoxTopWidget(int index, const QString &text, QWidget *parent = 0);

    ~MusicSongsToolBoxTopWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set Item index.
     */
    inline void setItemIndex(int index) { m_index = index; }
    /*!
     * Get Item index.
     */
    inline int getItemIndex() const { return m_index; }

    /*!
     * Set Item expand.
     */
    void setItemExpand(bool expand);
    /*!
     * Get Item expand.
     */
    bool isItemExpand() const;

    /*!
     * Set top label title.
     */
    void setTitle(const QString &text);
    /*!
     * Get top label title.
     */
    QString getTitle(bool suffix = false);

    /*!
     * Set music sort.
     */
    inline void setMusicSort(MusicSort *sort) { m_musicSort = sort;}

Q_SIGNALS:
    /*!
     * Add new play list item.
     */
    void addNewRowItem();
    /*!
     * Current top widget is pressed.
     */
    void mousePressAt(int index);
    /*!
     * Delete selected play list item.
     */
    void deleteRowItem(int index);
    /*!
     * Delete all items in play list item.
     */
    void deleteRowItemAll(int index);
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Add new music file or files to list.
     */
    void addNewFiles(int index);
    /*!
     * Add new music dir to list.
     */
    void addNewDir(int index);
    /*!
     * Music list songs sort by type.
     */
    void musicListSongSortBy(int index);
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int before, int after);

public Q_SLOTS:
    /*!
     * Delete selected play list item.
     */
    void deleteRowItem();
    /*!
     * Delete all items in play list item.
     */
    void deleteRowItemAll();
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemName();
    /*!
     * Rename item artist label is finised.
     */
    void setChangItemName(const QString &name);
    /*!
     * Add new music file or files to list.
     */
    void addNewFiles();
    /*!
     * Add new music dir to list.
     */
    void addNewDir();
    /*!
     * Export music songs by item list.
     */
    void exportSongsItemList();
    /*!
     * Show menu items.
     */
    void showMenu();
    /*!
     * Music list songs sort by type.
     */
    void musicListSongSortBy(QAction *action);
    /*!
     * Show share list dialog.
     */
    void showShareListDialog();
    /*!
     * Show enhance lossless dialog.
     */
    void showEnhanceLosslessDialog();

protected:
    /*!
     * Check the item is enable or not.
     */
    bool isItemEnable() const;
    /*!
     * Override the widget event.
     */
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    int m_index;
    QString m_suffixString;
    QLabel *m_labelIcon, *m_labelText;
    MusicSongsToolItemRenamedWidget *m_renameLine;

    bool m_isDrawTopState, m_isDrawMoveState;
    bool m_isBlockMoveExpand;
    QPoint m_pressPosAt;

    MusicSort *m_musicSort;
};


/*! @brief The class of the tool box mask widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsToolBoxMaskWidget : public MusicSongsToolBoxTopWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsToolBoxMaskWidget(QWidget *parent = 0);

    virtual ~MusicSongsToolBoxMaskWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    QTimer *m_updateTiemr;

};


/*! @brief The class of the tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsToolBoxWidgetItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsToolBoxWidgetItem(int index, const QString &text, QWidget *parent = 0);

    ~MusicSongsToolBoxWidgetItem();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get widget item.
     */
    QWidget *item(int index);
    /*!
     * Add widget item.
     */
    void addItem(QWidget *item);
    /*!
     * Remove widget item.
     */
    void removeItem(QWidget *item);

    /*!
     * Set title text.
     */
    void setTitle(const QString &text);
    /*!
     * Get title text.
     */
    QString getTitle() const;

    /*!
     * Set music sort.
     */
    void setMusicSort(MusicSort *sort);

    /*!
     * Set item widget to hide or not.
     */
    void setItemExpand(bool expand);
    /*!
     * Get item widget expand state.
     */
    bool itemExpand() const;

    /*!
     * Get item's count.
     */
    int count() const;

Q_SIGNALS:
    /*!
     * Add new play list item.
     */
    void addNewRowItem();
    /*!
     * Delete selected play list item.
     */
    void deleteRowItem(int index);
    /*!
     * Delete all items in play list item.
     */
    void deleteRowItemAll(int index);
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Add new music file or files to list.
     */
    void addNewFiles(int index);
    /*!
     * Add new music dir to list.
     */
    void addNewDir(int index);
    /*!
     * Music list songs sort by type.
     */
    void musicListSongSortBy(int index);
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int before, int after);

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    QVBoxLayout *m_layout;
    QList<QWidget*> m_itemList;
    MusicSongsToolBoxTopWidget *m_topWidget;

};


class QScrollArea;

/*! @brief The class of the tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOL_EXPORT MusicToolBoxWidgetItem
{
    int m_itemIndex;
    MusicSongsToolBoxWidgetItem* m_widgetItem;

    MusicToolBoxWidgetItem()
    {
        m_itemIndex = -1;
        m_widgetItem = nullptr;
    }
}MusicToolBoxWidgetItem;

/*! @brief The class of the tool box widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsToolBoxWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsToolBoxWidget(QWidget *parent = 0);

    ~MusicSongsToolBoxWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Add widget item.
     */
    void addItem(QWidget *item, const QString &text);
    /*!
     * Remove widget item.
     */
    void removeItem(QWidget *item);
    /*!
     * Swap widget item.
     */
    void swapItem(int before, int after);

    /*!
     * Set title text.
     */
    void setTitle(QWidget *item, const QString &text);
    /*!
     * Get title text.
     */
    QString getTitle(QWidget *item) const;

    /*!
     * Set music sort.
     */
    void setMusicSort(QWidget *item, MusicSort *sort);

    /*!
     * Reset scroll index by given position.
     */
    void resizeScrollIndex(int index) const;
    /*!
     * Get current index.
     */
    int currentIndex() const;
    /*!
     * Get item's count.
     */
    int count() const;

public Q_SLOTS:
    /*!
     * Set current index.
     */
    void setCurrentIndex(int index);
    /*!
     * Current top widget is pressed.
     */
    void mousePressAt(int index);
    /*!
     * Set background transparent.
     */
    void setTransparent(int alpha);

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Found mapped index in container.
     */
    int foundMappingIndex(int index);

    int m_currentIndex, m_itemIndexRaise;
    QVBoxLayout *m_layout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentsWidget;
    QList<MusicToolBoxWidgetItem> m_itemList;

};

#endif // MUSICSONGSTOOLBOXWIDGET_H
