#ifndef MUSICPAGINGWIDGETOBJECT_H
#define MUSICPAGINGWIDGETOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

class MusicClickedLabel;

/*! @brief The class of the paging widget object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPagingWidgetObject : public QObject
{
    Q_OBJECT
public:
    explicit MusicPagingWidgetObject(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicPagingWidgetObject();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    QWidget* createPagingWidget(QWidget *parent, int total);
    /*!
     * Create paging items.
     */
    void reset(int total);
    /*!
     * Reset page to origin.
     */
    void paging(int index, int total);
    /*!
     * Start to page by given index and total.
     */
    int currentIndex() const;
    /*!
     * Get current page index.
     */

Q_SIGNALS:
    void mapped(int index);
    /*!
     * Mapped the clicked page index.
     */

protected:
    int m_currentPage;
    QWidget *m_pagingWidget;
    QList<MusicClickedLabel*> m_pagingItems;

};

#endif // MUSICPAGINGWIDGETOBJECT_H
