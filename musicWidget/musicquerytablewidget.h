#ifndef MUSICQUERYTABLEWIDGET_H
#define MUSICQUERYTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"
#include "musicnetworkthread.h"
#include "musicdownloadquerysinglethread.h"

#include <QMenu>

class QActionGroup;
class MusicQueryTableDelegate;

class MUSIC_WIDGET_EXPORT MusicQueryTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicQueryTableWidget(QWidget *parent = 0);
    virtual ~MusicQueryTableWidget();

    virtual void startSearchQuery(const QString &text) = 0;
    virtual void musicDownloadLocal(int row) = 0;
    MIntList getSelectedItems() const;

Q_SIGNALS:
    void showDownLoadInfoFor(MusicObject::DownLoadType type);
    void restartSearchQuery(const QString &name);

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    virtual void clearAllItems() = 0;
    virtual void creatSearchedItems(const QString &songname,
                                    const QString &artistname,
                                    const QString &time) = 0;
    virtual void itemDoubleClicked(int row, int column) = 0;
    virtual void actionGroupClick(QAction *action);
    void setSelectedAllItems(bool all);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override {}
    void createContextMenu(QMenu &menu);
    int findActionGroup(QAction *action);

    QActionGroup *m_actionGroup;
    MusicDownLoadQueryThreadAbstract *m_downLoadManager;
    MusicQueryTableDelegate *m_checkBoxDelegate;

};

#endif // MUSICQUERYTABLEWIDGET_H
