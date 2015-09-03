#ifndef MUSICTABLEQUERYWIDGET_H
#define MUSICTABLEQUERYWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictablewidgetabstract.h"
#include "musicdownloadquerythread.h"

#include <QItemDelegate>

class QCheckBox;

class MusicCheckBoxDelegate : public QItemDelegate
{
public:
    explicit MusicCheckBoxDelegate(QObject* parent = 0);
    ~MusicCheckBoxDelegate();

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &) const;
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
private:
    QCheckBox *m_checkBox;

};


class MUSIC_WIDGET_EXPORT MusicTableQueryWidget : public MusicTableWidgetAbstract
{
    Q_OBJECT
public:
    explicit MusicTableQueryWidget(QWidget *parent = 0);
    virtual ~MusicTableQueryWidget();

    virtual void startSearchQuery(const QString &text) = 0;
    MIntList getSelectedItems() const;

signals:
    void showDownLoadInfoFinished(const QString &name);
    void muiscSongToPlayListChanged(const QString &name);
    void showDownLoadInfoFor(DownLoadType type);

public slots:
    virtual void listCellClicked(int row, int column);
    virtual void clearAllItems() = 0;
    virtual void creatSearchedItems(const QString &songname,
                                    const QString &artistname,
                                    const QString &time) = 0;
    virtual void itemDoubleClicked(int row, int column) = 0;
    void setSelectedAllItems(bool all);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

    MusicDownLoadQueryThread *m_downLoadManager;
    MusicCheckBoxDelegate *m_checkBoxDelegate;

};

#endif // MUSICTABLEQUERYWIDGET_H
