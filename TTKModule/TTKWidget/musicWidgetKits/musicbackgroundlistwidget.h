#ifndef MUSICBACKGROUNDLISTWIDGET_H
#define MUSICBACKGROUNDLISTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QGridLayout>
#include "musicuiobject.h"
#include "musicbackgroundimage.h"

#define CURRENT_ITEMS_COUNT         47

/*! @brief The class of the background list item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundListItem : public QLabel
{
    Q_OBJECT
public:
    explicit MusicBackgroundListItem(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    inline void setFileName(const QString &name) { m_name = name; }
    /*!
     * Set item file name.
     */
    inline QString getFileName() const { return m_name; }
    /*!
     * Get item file name.
     */
    inline void setFilePath(const QString &path) { m_path = path; }
    /*!
     * Set item file name.
     */
    inline QString getFilePath() const { return m_path; }
    /*!
     * Get item file name.
     */

    void updatePixImage();
    /*!
     * Update pix image.
     */
    void updatePixImage(const MusicBackgroundImage &image);
    /*!
     * Update pix image.
     */
    bool contains(const MusicSkinConfigItem &item) const;
    /*!
     * Current item contains or not.
     */

    void setSelect(bool s);
    /*!
     * Select the current item.
     */
    inline bool isSelect() const { return m_isSelected; }
    /*!
     * Get current selected item state.
     */

    void setSelectEnable(bool s);
    /*!
     * Select the current item enable or not.
     */
    inline bool isSelectEnable() const { return m_selectedMask; }
    /*!
     * Get current selected item state.
     */

    void setCloseEnable(bool s);
    /*!
     * Close option set the current item.
     */
    inline bool isCloseEnable() const { return m_closeSet; }
    /*!
     * Get close option set the current item.
     */

    void setShowNameEnable(bool s);
    /*!
     * Show title name set the current item.
     */
    inline bool isShowNameEnable() const { return m_showNameMask; }
    /*!
     * Get show title name set the current item.
     */

Q_SIGNALS:
    void closeClicked(MusicBackgroundListItem *item);
    /*!
     * Current item close clicked.
     */
    void itemClicked(MusicBackgroundListItem *item);
    /*!
     * Current item clicked.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_printMask, m_isSelected, m_selectedMask;
    bool m_closeMask, m_closeSet, m_showNameMask;
    QString m_name, m_path;
    MusicSkinConfigItem m_imageInfo;

};


/*! @brief The class of the background list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBackgroundListWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBackgroundListWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setCurrentItemName(const QString &name);
    /*!
     * Select current item by name when the widget show.
     */

    void clearSelectState();
    /*!
     * Clear select state.
     */
    void clearAllItems();
    /*!
     * Clear All Items.
     */

    void createItem(const QString &name, const QString &path, bool state);
    /*!
     * Create item by name and path.
     */
    void createItem(const QString &icon, bool state);
    /*!
     * Create item by icon.
     */
    void updateItem(const MusicBackgroundImage &image, const QString &path);
    /*!
     * Update item by backgroud image.
     */

    bool contains(const QString &name) const;
    /*!
     * Current item contains or not.
     */
    bool contains(const MusicBackgroundImage &image) const;
    /*!
     * Current item contains or not.
     */

    int find(MusicBackgroundListItem *item) const;
    /*!
     * Current item index at container.
     */
    MusicBackgroundListItem* find(const QString &name) const;
    /*!
     * Find item by name.
     */
    MusicBackgroundListItem* find(const MusicBackgroundImage &image) const;
    /*!
     * Find item by image.
     */

    void updateLastedItem();
    /*!
     * Update lasted item state.
     */

    inline int count() const { return m_items.count(); }
    /*!
     * Item count.
     */

Q_SIGNALS:
    void itemClicked(const QString &name);
    /*!
     * Current item clicked.
     */

private Q_SLOTS:
    void itemCloseClicked(MusicBackgroundListItem *item);
    /*!
     * Current item close clicked.
     */
    void itemHasClicked(MusicBackgroundListItem *item);
    /*!
     * Current item has clicked.
     */

protected:
    QGridLayout *m_layout;
    MusicBackgroundListItem *m_currentItem;
    QList<MusicBackgroundListItem*> m_items;

};

#endif // MUSICBACKGROUNDLISTWIDGET_H
