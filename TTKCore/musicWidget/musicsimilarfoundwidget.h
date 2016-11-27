#ifndef MUSICSIMILARFOUNDWIDGET_H
#define MUSICSIMILARFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicqueryfoundtablewidget.h"

/*! @brief The class of the similar music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSimilarFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
public:
    explicit MusicSimilarFoundTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSimilarFoundTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Set network query input.
     */

    void resizeWindow();
    /*!
     * Resize window bound by widgte resize called.
     */

public Q_SLOTS:
    virtual void createSearchedItems(const QString &songname, const QString &artistname,
                                     const QString &time) override;
    /*!
     * Create searched items.
     */

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Override the widget event.
     */

};



/*! @brief The class of similar music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSimilarFoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSimilarFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSimilarFoundWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setSongName(const QString &name);
    /*!
     * Set current name to search founds.
     */

    void resizeWindow();
    /*!
     * Resize window bound by widgte resize called.
     */

public Q_SLOTS:
    void queryAllFinished();
    /*!
     * Query all quality musics is finished.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Send recieved data from net.
     */
    void urlHasChanged(const QString &imageUrl);
    /*!
     * Data download finished and send to shared on web.
     */
    void playButtonClicked();
    /*!
     * Play button clicked now.
     */
    void downloadButtonClicked();
    /*!
     * Download button clicked now.
     */
    void addButtonClicked();
    /*!
     * Add button clicked now.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createLabels();
    /*!
     * Create init interface lables.
     */

    QString m_songNameFull;
    QWidget *m_mainWindow;
    QLabel *m_statusLabel;
    QList<QLabel*> m_iconLabels;
    MusicSimilarFoundTableWidget *m_similarTableWidget;

};

#endif // MUSICSIMILARFOUNDWIDGET_H
