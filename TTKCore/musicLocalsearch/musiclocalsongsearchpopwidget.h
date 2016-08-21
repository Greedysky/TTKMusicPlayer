#ifndef MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
#define MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicabstracttablewidget.h"

#define ROW_HEIGHT 30

class QPushButton;

/*! @brief The class of the search popup table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLocalSongSearchPopTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void clearAllItems();
    /*!
     * Clear All Items.
     */
    void createItems(int index, const QString &name, const QString &time);
    /*!
     * Create popup table item by index and name and time.
     */

Q_SIGNALS:
    void setText(const QString &text);
    /*!
     * User click the current item, emit the current text.
     */

public Q_SLOTS:
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */

};


/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchPopWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicLocalSongSearchPopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void createItems();
    /*!
     * Create popup search history table items.
     */

Q_SIGNALS:
    void setText(const QString &text);
    /*!
     * User click the current item, emit the current text.
     */

public Q_SLOTS:
    void clearButtonClicked();
    /*!
     * Clear the search history.
     */

protected:
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */
    QString utcTimeToLocal(const QString &time) const;
    /*!
     * Transform utc time to local time before.
     */

    QPushButton *m_clearButton;
    MusicLocalSongSearchPopTableWidget *m_popTableWidget;

};

#endif // MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
