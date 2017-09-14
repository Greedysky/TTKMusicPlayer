#ifndef MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
#define MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstracttablewidget.h"

class QPushButton;

/*! @brief The class of the search popup table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchPopTableWidget(QWidget *parent = 0);

    virtual ~MusicLocalSongSearchPopTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Clear All Items.
     */
    void clearAllItems();
    /*!
     * Create popup table item by index and name and time.
     */
    void createItems(int index, const QString &name, const QString &time);

Q_SIGNALS:
    /*!
     * User click the current item, emit the current text.
     */
    void setText(const QString &text);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

};


/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchPopWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchPopWidget(QWidget *parent = 0);

    ~MusicLocalSongSearchPopWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Create popup search history table items.
     */
    void createItems();

Q_SIGNALS:
    /*!
     * User click the current item, emit the current text.
     */
    void setText(const QString &text);

public Q_SLOTS:
    /*!
     * Clear the search history.
     */
    void clearButtonClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Transform utc time to local time before.
     */
    QString utcTimeToLocal(const QString &time) const;

    QPushButton *m_clearButton;
    MusicLocalSongSearchPopTableWidget *m_popTableWidget;

};

#endif // MUSICLOCALSONGSEARCHPOPTABLEWIDGET_H
