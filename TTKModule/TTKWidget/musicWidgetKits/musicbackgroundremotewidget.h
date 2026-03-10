#ifndef MUSICBACKGROUNDREMOTEWIDGET_H
#define MUSICBACKGROUNDREMOTEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
 ***************************************************************************/

#include "musicbackgroundlistwidget.h"
#include "musicabstractdownloadskinrequest.h"

class MusicDownloadQueueRequest;
struct MusicResultsCategoryItem;
class MusicResultsCategoryPopWidget;

/*! @brief The class of the remote background widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundRemoteWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBackgroundRemoteWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBackgroundRemoteWidget();

    /*!
     * Abort the current download request.
     */
    void abort();

    /*!
     * Output remote skin.
     * Subclass should implement this function.
     */
    virtual void outputRemoteSkin(MusicBackgroundImage &image, const QString &data) = 0;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downloadDataChanged(const QString &bytes);
    /*!
     * Download data from net finished.
     */
    virtual void downloadFinished(const MusicSkinRemoteGroupList &bytes) = 0;

protected:
    /*!
     * Start to download background data by type.
     */
    void startToRequest(MusicSkinRemoteGroup::Type type);

    int m_currentIndex;
    MusicSkinRemoteGroupList m_groups;
    MusicBackgroundListWidget *m_backgroundList;
    MusicDownloadQueueRequest *m_networkRequest;
    MusicAbstractDownloadSkinRequest *m_downloadRequest;

};



/*! @brief The class of the daily background widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundDailyWidget : public MusicBackgroundRemoteWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBackgroundDailyWidget(QWidget *parent = nullptr);

    /*!
     * Init widget and parameter.
     */
    void initialize();

    /*!
     * Output remote skin.
     */
    virtual void outputRemoteSkin(MusicBackgroundImage &image, const QString &data) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downloadFinished(const MusicSkinRemoteGroupList &bytes) override final;

private:
    int m_tryTimes;

};



/*! @brief The class of the online background widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundOnlineWidget : public MusicBackgroundRemoteWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicBackgroundOnlineWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBackgroundOnlineWidget();

    /*!
     * Init widget and parameter.
     */
    void initialize();

    /*!
     * Create functions widget.
     */
    QWidget* createFunctionsWidget(QWidget *container, QWidget *parent);

    /*!
     * Output remote skin.
     */
    virtual void outputRemoteSkin(MusicBackgroundImage &image, const QString &data) override final;

public Q_SLOTS:
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);
    /*!
     * Download data from net finished.
     */
    virtual void downloadFinished(const MusicSkinRemoteGroupList &bytes) override final;

private:
    QWidget *m_functionsWidget;
    MusicResultsCategoryPopWidget *m_categoryButton;
    QList<MusicAbstractDownloadSkinRequest*> m_downloadRequests;

};

#endif // MUSICBACKGROUNDREMOTEWIDGET_H
