#ifndef MUSICBACKGROUNDLISTWIDGET_H
#define MUSICBACKGROUNDLISTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define MUSIC_BG_ROLE       Qt::UserRole + 1
#define MUSIC_FILEHASH_ROLE Qt::UserRole + 2

#include <QListWidget>
#include "musicuiobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the background list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusicBackgroundListWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBackgroundListWidget();

    void setCurrentItemName(const QString &name);
    /*!
     * Select current item by name when the widget show.
     */
    void clearAllItems();
    /*!
     * Clear All Items.
     */

    void createItem(const QString &name, const QString &path);
    /*!
     * Create item by name and path.
     */
    void createItem(const QString &name, const QIcon &icon);
    /*!
     * Create item by name and icon.
     */

    bool contains(const QString &name);
    /*!
     * List contains the item by given name, yes retuen true.
     */
    bool contains(QListWidgetItem *item);
    /*!
     * List contains the item, yes retuen true.
     */

    int indexOf(const QString &name);
    /*!
     * Get index of item in list by given name.
     */
    int indexOf(QListWidgetItem *item);
    /*!
     * Get index of item in list by given item.
     */

    QString fileHash(const QString &name);
    /*!
     * Get file md5 hash.
     */

public Q_SLOTS:
    void reCreateItem(const QString &name);
    /*!
     * ReCreate item when background download finished.
     */

};

#endif // MUSICBACKGROUNDLISTWIDGET_H
