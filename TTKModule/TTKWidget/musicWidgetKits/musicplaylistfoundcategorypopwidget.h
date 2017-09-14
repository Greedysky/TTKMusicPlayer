#ifndef MUSICPLAYLISTFOUNDCATEGORYPOPWIDGET_H
#define MUSICPLAYLISTFOUNDCATEGORYPOPWIDGET_H

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
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistFoundCategoryItem(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current category.
     */
    void setCategory(const MusicPlaylistCategory &category);

Q_SIGNALS:
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicPlaylistCategoryItem &category);

public Q_SLOTS:
    /*!
     * Current category item clicked.
     */
    void buttonClicked(int index);

protected:
    MusicPlaylistCategory m_category;

};



/*! @brief The class of the playlist music found category pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundCategoryPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistFoundCategoryPopWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current category by input server.
     */
    void setCategory(const QString &server, QObject *obj);
    /*!
     * Close the menu dialog.
     */
    void closeMenu();

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    void popupMenu();

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

};

#endif // MUSICPLAYLISTFOUNDCATEGORYPOPWIDGET_H
