#ifndef MUSICSIMILARFOUNDWIDGET_H
#define MUSICSIMILARFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfoundabstractwidget.h"
#include "musicqueryfoundtablewidget.h"

/*! @brief The class of the similar music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSimilarFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSimilarFoundTableWidget(QWidget *parent = 0);

    virtual ~MusicSimilarFoundTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set network query input.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);

public Q_SLOTS:
    /*!
     * Create searched items.
     */
    virtual void createSearchedItems(const MusicSearchedItem &songItem) override;

};



/*! @brief The class of similar music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSimilarFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSimilarFoundWidget(QWidget *parent = 0);

    virtual ~MusicSimilarFoundWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Data download finished and send to shared on web.
     */
    void downloadUrlChanged(const QString &imageUrl);
    /*!
     * Play button clicked now.
     */
    void playButtonClicked();
    /*!
     * Download button clicked now.
     */
    void downloadButtonClicked();
    /*!
     * Add button clicked now.
     */
    void addButtonClicked();

protected:
    /*!
     * Create init interface lables.
     */
    void createLabels();

    MusicSimilarFoundTableWidget *m_similarTableWidget;

};

#endif // MUSICSIMILARFOUNDWIDGET_H
