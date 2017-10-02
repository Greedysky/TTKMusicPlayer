#ifndef MUSICTOPLISTFOUNDWIDGET_H
#define MUSICTOPLISTFOUNDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfoundabstractwidget.h"
#include "musicqueryfoundtablewidget.h"

/*! @brief The class of the toplist music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicTopListFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTopListFoundTableWidget(QWidget *parent = 0);

    virtual ~MusicTopListFoundTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set network query input.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);

};


/*! @brief The class of toplist music found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicTopListFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTopListFoundWidget(QWidget *parent = 0);

    virtual ~MusicTopListFoundWidget();

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
     * Create the current toplist info item.
     */
    void createToplistInfoItem(const MusicPlaylistItem &item);
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
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

    QLabel *m_iconLabel, *m_nameLabel, *m_playCountLabel;
    QLabel *m_updateTimeLabel, *m_descriptionLabel, *m_songItemsLabel;

    MusicTopListFoundTableWidget *m_toplistTableWidget;

};

#endif // MUSICTOPLISTFOUNDWIDGET_H
