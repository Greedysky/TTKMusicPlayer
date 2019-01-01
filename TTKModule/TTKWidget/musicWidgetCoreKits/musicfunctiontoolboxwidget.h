#ifndef MUSICFUNCTIONTOOLBOXWIDGET_H
#define MUSICFUNCTIONTOOLBOXWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

#define RENAME_WIDTH    220

/*! @brief The class of the function tool box top widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFunctionToolBoxTopWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFunctionToolBoxTopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFunctionToolBoxTopWidget(int index, const QString &text, QWidget *parent = nullptr);

    ~MusicFunctionToolBoxTopWidget();

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

Q_SIGNALS:
    /*!
     * Current top widget is pressed.
     */
    void mousePressAt(int index);
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int before, int after);

protected:
    /*!
     * Check the item is enable or not.
     */
    virtual bool isItemEnable() const;
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

    int m_index;
    QString m_suffixString;
    QLabel *m_labelIcon, *m_labelText;

    bool m_isDrawTopState, m_isDrawMoveState;
    bool m_isBlockMoveExpand;
    QPoint m_pressPosAt;

};



/*! @brief The class of the function tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFunctionToolBoxWidgetItem : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFunctionToolBoxWidgetItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFunctionToolBoxWidgetItem(QWidget *parent = nullptr);
    explicit MusicFunctionToolBoxWidgetItem(int index, const QString &text, QWidget *parent = nullptr);

    ~MusicFunctionToolBoxWidgetItem();

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
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int before, int after);

protected:
    /*!
     * Init parameter.
     */
    void init();
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    QVBoxLayout *m_layout;
    QList<QWidget*> m_itemList;
    MusicFunctionToolBoxTopWidget *m_topWidget;

};


/*! @brief The class of the normal tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicNormalToolBoxWidgetItem : public MusicFunctionToolBoxWidgetItem
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicNormalToolBoxWidgetItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicNormalToolBoxWidgetItem(int index, const QString &text, QWidget *parent = nullptr);

};


class QScrollArea;

/*! @brief The class of the tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_TOOL_EXPORT MusicToolBoxWidgetItem
{
    int m_itemIndex;
    MusicFunctionToolBoxWidgetItem* m_widgetItem;

    MusicToolBoxWidgetItem()
    {
        m_itemIndex = -1;
        m_widgetItem = nullptr;
    }
}MusicToolBoxWidgetItem;


/*! @brief The class of the function tool box widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFunctionToolBoxWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFunctionToolBoxWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFunctionToolBoxWidget(QWidget *parent = nullptr);

    ~MusicFunctionToolBoxWidget();

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
     * Create item.
     */
    virtual MusicFunctionToolBoxWidgetItem* createItem(QWidget *item, const QString &text);
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

#endif // MUSICFUNCTIONTOOLBOXWIDGET_H
