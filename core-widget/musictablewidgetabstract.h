#ifndef MUSICTABLEWIDGETABSTRACT_H
#define MUSICTABLEWIDGETABSTRACT_H

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

class MUSIC_WIDGET_EXPORT MusicTableWidgetAbstract : public QTableWidget
{
    Q_OBJECT
public:
    explicit MusicTableWidgetAbstract(QWidget *parent = 0);
    ~MusicTableWidgetAbstract();

    virtual void clearAllItems();
    void setTransparent(int angle);
    void setRowColor(int row, const QColor &color) const;

signals:

public slots:
    virtual void listCellEntered(int row, int column);
    virtual void listCellClicked(int row, int column) = 0;

protected:
    QColor m_defaultBkColor;
    int m_previousColorRow;

};

#endif // MUSICTABLEWIDGETABSTRACT_H
