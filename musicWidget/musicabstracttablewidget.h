#ifndef MUSICABSTRACTTABLEWIDGET_H
#define MUSICABSTRACTTABLEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTableWidget>
#include <QHeaderView>
#include "musiclibexportglobal.h"
#include "musicobject.h"
#include "musicuiobject.h"

class MUSIC_WIDGET_EXPORT MusicAbstractTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MusicAbstractTableWidget(QWidget *parent = 0);
    virtual ~MusicAbstractTableWidget();

    void clear();

public slots:
    virtual void listCellEntered(int row, int column);
    virtual void listCellClicked(int row, int column) = 0;

protected:
    void setTransparent(int angle);
    void setRowColor(int row, const QColor &color) const;

    QColor m_defaultBkColor;
    int m_previousColorRow;
    int m_previousClickRow;

};

#endif // MUSICABSTRACTTABLEWIDGET_H
