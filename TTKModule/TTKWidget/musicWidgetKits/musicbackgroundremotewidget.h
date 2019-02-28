#ifndef MUSICBACKGROUNDREMOTEWIDGET_H
#define MUSICBACKGROUNDREMOTEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QWidget>
#include "musicbackgroundlistwidget.h"
#include "musicdownloadbackgroundremotethread.h"

class QPushButton;
class QListWidgetItem;
class MusicDownloadQueueCache;

/*! @brief The class of the remote background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundRemoteWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBackgroundRemoteWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundRemoteWidget(QWidget *parent = nullptr);

    ~MusicBackgroundRemoteWidget();

    /*!
     * Abort the current download thread.
     */
    void abort();

    /*!
     * Output remote skin.
     */
    virtual void outputRemoteSkin(MusicBackgroundImage &image, const QString &data) = 0;

public Q_SLOTS:
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const QString &data);
    /*!
     * Send download data from net.
     */
    virtual void downLoadDataChanged(const MusicSkinRemoteGroups &data);

protected:
    /*!
     * Start to download data.
     */
    void startToDownload(const QString &prefix);

    int m_currentIndex;
    MusicSkinRemoteGroups m_groups;
    MusicBackgroundListWidget *m_listWidget;
    MusicDownloadQueueCache *m_downloadQueue;
    MusicDownloadBackgroundRemoteThread *m_queryThread;

};


/*! @brief The class of the thunder background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundThunderWidget : public MusicBackgroundRemoteWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBackgroundThunderWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundThunderWidget(QWidget *parent = nullptr);

    virtual ~MusicBackgroundThunderWidget();

    /*!
     * Init the current download object.
     */
    void init();

    /*!
     * Create functions widget.
     */
    QWidget* createFunctionsWidget(bool revert, QWidget *object = nullptr);
    /*!
     * Output remote skin.
     */
    virtual void outputRemoteSkin(MusicBackgroundImage &image, const QString &data) override;

public Q_SLOTS:
    /*!
     * Remote background type selected by index.
     */
    void buttonClicked(int index);
    /*!
     * Send download data from net.
     */
    virtual void downLoadDataChanged(const MusicSkinRemoteGroups &data) override;

protected:
    /*!
     * Create button.
     */
    QPushButton *createButton(const QString &name);
    /*!
     * Button style changed.
     */
    void buttonStyleChanged();

    QWidget *m_functionsWidget;
    QList<QPushButton*> m_functionsItems;

};



/*! @brief The class of the daily background widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicBackgroundDailyWidget : public MusicBackgroundRemoteWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBackgroundDailyWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundDailyWidget(QWidget *parent = nullptr);

    /*!
     * Init the current download object.
     */
    void init();

    /*!
     * Output remote skin.
     */
    virtual void outputRemoteSkin(MusicBackgroundImage &image, const QString &data) override;

public Q_SLOTS:
    /*!
     * Send download data from net.
     */
    virtual void downLoadDataChanged(const MusicSkinRemoteGroups &data) override;

};

#endif // MUSICBACKGROUNDREMOTEWIDGET_H
