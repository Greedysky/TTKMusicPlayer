#ifndef MUSICBACKGROUNDREMOTEWIDGET_H
#define MUSICBACKGROUNDREMOTEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    explicit MusicBackgroundRemoteWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicBackgroundRemoteWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void init();
    /*!
     * Init the current download object.
     */
    void abort();
    /*!
     * Abort the current download thread.
     */

    QWidget* createFunctionsWidget(bool revert, QWidget *object = 0);
    /*!
     * Create functions widget.
     */
    void outputRemoteSkin(MusicBackgroundImage &image, const QString &data);
    /*!
     * Output remote skin.
     */

Q_SIGNALS:
    void showCustomSkin(const QString &path);
    /*!
     * Reset the current bg when user click remote bg emit.
     */

public Q_SLOTS:
    void buttonClicked(int index);
    /*!
     * Remote bg type selected by index.
     */
    void downLoadDataChanged(const QString &data);
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const MusicSkinRemoteGroups &data);
    /*!
     * Send download data from net.
     */

protected:
    QPushButton *createButton(const QString &name);
    /*!
     * Create button.
     */
    void buttonStyleChanged();
    /*!
     * Button style changed.
     */

    int m_currentIndex;
    QWidget *m_functionsWidget;
    QList<QPushButton*> m_functionsItems;
    MusicSkinRemoteGroups m_groups;
    MusicBackgroundListWidget *m_listWidget;
    MusicDownloadQueueCache *m_downloadQueue;
    MusicDownloadBackgroundRemoteThread *m_queryThread;

};

#endif // MUSICBACKGROUNDREMOTEWIDGET_H
