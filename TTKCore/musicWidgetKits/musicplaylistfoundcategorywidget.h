#ifndef MUSICPLAYLISTFOUNDCATEGORYWIDGET_H
#define MUSICPLAYLISTFOUNDCATEGORYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"
#include "musiccategoryconfigmanager.h"

/*! @brief The class of the playlist music found category item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundCategoryItem : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPlaylistFoundCategoryItem(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setCategory(const PlaylistCategory &category);
    /*!
     * Set current category.
     */

Q_SIGNALS:
    void categoryChanged(const PlaylistCategoryItem &category);
    /*!
     * Current category changed.
     */

public Q_SLOTS:
    void buttonClicked(int index);
    /*!
     * Current category item clicked.
     */

protected:
    PlaylistCategory m_category;

};



/*! @brief The class of the playlist music found category widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundCategoryWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicPlaylistFoundCategoryWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicPlaylistFoundCategoryWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setCategory(const QString &server, QObject *obj);
    /*!
     * Set current category by input server.
     */
    void closeMenu();
    /*!
     * Close the menu dialog.
     */

public Q_SLOTS:
    void popupMenu();
    /*!
     * To popup menu.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

};

#endif // MUSICPLAYLISTFOUNDCATEGORYWIDGET_H
