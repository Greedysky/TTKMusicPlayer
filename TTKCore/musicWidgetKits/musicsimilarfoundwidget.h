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

public Q_SLOTS:
    virtual void createSearchedItems(const MusicSearchedItem &songItem) override;
    /*!
     * Create searched items.
     */

};



/*! @brief The class of similar music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSimilarFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicSimilarFoundWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSimilarFoundWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void setSongName(const QString &name) override;
    /*!
     * Set current name to search founds.
     */

    virtual void resizeWindow() override;
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
    void createLabels();
    /*!
     * Create init interface lables.
     */

    MusicSimilarFoundTableWidget *m_similarTableWidget;

};

#endif // MUSICSIMILARFOUNDWIDGET_H
