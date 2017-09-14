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
    /*!
     * Object contsructor.
     */
    explicit MusicPagingWidgetObject(QObject *parent = 0);

    virtual ~MusicPagingWidgetObject();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Create paging items.
     */
    QWidget* createPagingWidget(QWidget *parent, int total);
    /*!
     * Reset page to origin.
     */
    void reset(int total);
    /*!
     * Start to page by given index and total.
     */
    void paging(int index, int total);
    /*!
     * Get current page index.
     */
    int currentIndex() const;

Q_SIGNALS:
    /*!
     * Mapped the clicked page index.
     */
    void mapped(int index);

protected:
    int m_currentPage;
    QWidget *m_pagingWidget;
    QList<MusicClickedLabel*> m_pagingItems;

};

#endif // MUSICPAGINGWIDGETOBJECT_H
