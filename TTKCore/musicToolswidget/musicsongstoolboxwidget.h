#ifndef MUSICSONGSTOOLBOXWIDGET_H
#define MUSICSONGSTOOLBOXWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QBoxLayout>
#include "musicglobaldefine.h"

class MusicSongsToolItemRenamedWidget;
/*! @brief The class of the tool box top widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsToolBoxTopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsToolBoxTopWidget(int index, const QString &text, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongsToolBoxTopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setItemExpand(bool expand);
    /*!
     * Set Item expand.
     */

    void setTitle(const QString &text);
    /*!
     * Set top label title.
     */
    QString getTitle(bool suffix = false);
    /*!
     * Get top label title.
     */

Q_SIGNALS:
    void mousePressAt(int index);
    /*!
     * Current top widget is pressed.
     */
    void deleteRowItem(int index);
    /*!
     * Delete selected play list item.
     */
    void renameFinished(int index, const QString &name);
    /*!
     * Open rename selected play list item widget.
     */

public Q_SLOTS:
    void deleteRowItem();
    /*!
     * Delete selected play list item.
     */
    void changRowItemName();
    /*!
     * Open rename selected play list item widget.
     */
    void setChangItemName(const QString &name);
    /*!
     * Rename item artist label is finised.
     */
    void showMenu();
    /*!
     * Show menu items.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_index;
    QString m_suffixString;
    QLabel *m_labelIcon, *m_labelText;
    MusicSongsToolItemRenamedWidget *m_renameLine;

};


/*! @brief The class of the tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsToolBoxWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsToolBoxWidgetItem(int index, const QString &text, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongsToolBoxWidgetItem();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    QWidget *item(int index);
    /*!
     * Get widget item.
     */
    void addItem(QWidget *item);
    /*!
     * Add widget item.
     */
    void removeItem(QWidget *item);
    /*!
     * Remove widget item.
     */

    void setTitle(const QString &text);
    /*!
     * Set title text.
     */
    QString getTitle() const;
    /*!
     * Get title text.
     */

    void setItemHide(bool hide);
    /*!
     * Set item widget to hide or not.
     */
    bool itemHide() const;
    /*!
     * Get item widget hide state.
     */

    int count() const;
    /*!
     * Get item's count.
     */

Q_SIGNALS:
    void addNewRowItem();
    /*!
     * Add new play list item.
     */
    void deleteRowItem(int index);
    /*!
     * Delete selected play list item.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Open rename selected play list item widget.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    QVBoxLayout *m_layout;
    QList<QWidget*> m_itemList;
    MusicSongsToolBoxTopWidget *m_topWidget;

};


class QScrollArea;

/*! @brief The class of the tool box widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsToolBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsToolBoxWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSongsToolBoxWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void addItem(QWidget *item, const QString &text);
    /*!
     * Add widget item.
     */
    void removeItem(QWidget *item);
    /*!
     * Remove widget item.
     */

    void setTitle(QWidget *item, const QString &text);
    /*!
     * Set title text.
     */
    QString getTitle(QWidget *item) const;
    /*!
     * Get title text.
     */

    void resizeScrollIndex(int index) const;
    /*!
     * Reset scroll index by given position.
     */
    int currentIndex() const;
    /*!
     * Get current index.
     */
    int count() const;
    /*!
     * Get item's count.
     */

public Q_SLOTS:
    void setCurrentIndex(int index);
    /*!
     * Set current index.
     */
    void mousePressAt(int index);
    /*!
     * Current top widget is pressed.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_currentIndex;
    QVBoxLayout *m_layout;
    QScrollArea *m_scrollArea;
    QList<MusicSongsToolBoxWidgetItem*> m_itemList;

};

#endif // MUSICSONGSTOOLBOXWIDGET_H
