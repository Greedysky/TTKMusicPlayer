#ifndef MUSICFUNCTIONTOOLBOXWIDGET_H
#define MUSICFUNCTIONTOOLBOXWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
 ***************************************************************************/

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

static constexpr int RENAME_WIDTH = 240;

/*! @brief The class of the function tool box top widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFunctionToolBoxTopWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFunctionToolBoxTopWidget(int index, const QString &text, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicFunctionToolBoxTopWidget();

    /*!
     * Set index.
     */
    inline void setIndex(int index) noexcept { m_index = index; }
    /*!
     * Get index.
     */
    inline int index() const noexcept { return m_index; }

    /*!
     * Get active.
     */
    bool isActive();

    /*!
     * Set expand.
     */
    void setExpand(bool expand);
    /*!
     * Get expand.
     */
    bool isExpand() const;

    /*!
     * Set top label title.
     */
    void setTitle(const QString &text);
    /*!
     * Get top label title.
     */
    QString title(bool suffix);

Q_SIGNALS:
    /*!
     * Current top widget is pressed.
     */
    void itemIndexChanged(int index);
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int start, int end);

protected:
    /*!
     * Check the item is enable or not.
     */
    virtual bool isItemEnabled() const;
    /*!
     * Override the widget event.
     */
    virtual void dragEnterEvent(QDragEnterEvent *event) override final;
    virtual void dragMoveEvent(QDragMoveEvent *event) override final;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override final;
    virtual void dropEvent(QDropEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void paintEvent(QPaintEvent *event) override;

    int m_index;
    QLabel *m_labelIcon, *m_labelText;

    bool m_isDrawTopState, m_isDrawMoveState;
    bool m_isBlockMoveExpand, m_isActive;
    QPoint m_pressedPos;

};



/*! @brief The class of the function tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFunctionToolBoxWidgetItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFunctionToolBoxWidgetItem(QWidget *parent = nullptr);
    explicit MusicFunctionToolBoxWidgetItem(int index, const QString &text, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicFunctionToolBoxWidgetItem();

    /*!
     * Get widget item.
     */
    QWidget *item() const noexcept;
    /*!
     * Create cell item by input item.
     */
    void addCellItem(QWidget *item);

    /*!
     * Set title text.
     */
    void setTitle(const QString &text);

    /*!
     * Set item widget to hide or not.
     */
    void setExpand(bool expand);
    /*!
     * Get item widget expand state.
     */
    bool isExpand() const;

    /*!
     * Get item widget is in active state.
     */
    bool isActive() const;

Q_SIGNALS:
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int start, int end);

protected:
    /*!
     * Init widget and parameter.
     */
    void initialize();
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;

    QVBoxLayout *m_layout;
    QWidget *m_item;
    MusicFunctionToolBoxTopWidget *m_topWidget;

};


/*! @brief The class of the normal tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicNormalToolBoxWidgetItem : public MusicFunctionToolBoxWidgetItem
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicNormalToolBoxWidgetItem(int index, const QString &text, QWidget *parent = nullptr);

};


class QScrollArea;


/*! @brief The class of the function tool box widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFunctionToolBoxWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFunctionToolBoxWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicFunctionToolBoxWidget();

    /*!
     * Create cell item by input item.
     */
    void addCellItem(QWidget *item, const QString &text);
    /*!
     * Remove widget item.
     */
    void removeItem(QWidget *item);
    /*!
     * Swap widget item.
     */
    void swapItem(int start, int end);

    /*!
     * Set title text.
     */
    void setTitle(QWidget *item, const QString &text);

    /*!
     * Reset scroll index by given position.
     */
    void resizeScrollIndex(int index) const;

    /*!
     * Get current index.
     */
    int currentIndex() const noexcept;
    /*!
     * Get item's count.
     */
    int count() const noexcept;

public Q_SLOTS:
    /*!
     * Set current index.
     */
    void setCurrentIndex(int index);
    /*!
     * Current top widget is pressed.
     */
    void itemIndexChanged(int index);
    /*!
     * Set background transparent.
     */
    void setTransparent(int alpha);

protected:
    /*!
     * Create item.
     */
    virtual MusicFunctionToolBoxWidgetItem* initialItem(QWidget *item, const QString &text) = 0;
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;
    /*!
     * Found mapped index in container.
     */
    int foundMappedIndex(int index);

    int m_currentIndex;
    int m_itemIndexRaise;
    QVBoxLayout *m_layout;
    QScrollArea *m_scrollArea;
    QWidget *m_contentsWidget;

protected:
    struct Data
    {
        int m_itemIndex;
        MusicFunctionToolBoxWidgetItem *m_itemWidget;
    };
    QList<Data> m_items;

};

#endif // MUSICFUNCTIONTOOLBOXWIDGET_H
