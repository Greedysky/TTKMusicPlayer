#ifndef MUSICSONGSLISTPLAYEDWIDGET_H
#define MUSICSONGSLISTPLAYEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

class QMenu;
class QPushButton;
class MusicGifLabelWidget;

/*! @brief The class of the song list played widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListPlayedWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsListPlayedWidget(int index, QWidget *parent = 0);

    ~MusicSongsListPlayedWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Update item artist label by name.
     */
    void setParameter(const QString &name);

Q_SIGNALS:
    /*!
     * Mouse is the item geometry, emit row and column.
     */
    void enterChanged(int row, int column);

public Q_SLOTS:
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAt();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    /*!
     * Create more menu information.
     */
    void createMoreMenu(QMenu *menu);

    int m_currentPlayIndex;
    QWidget *m_parentClass;
    QLabel *m_textLabel;
    MusicGifLabelWidget *m_gifLabel;
    QPushButton *m_deleteButton, *m_downloadButton, *m_moreButton;

};

#endif // MUSICSONGSLISTPLAYEDWIDGET_H
