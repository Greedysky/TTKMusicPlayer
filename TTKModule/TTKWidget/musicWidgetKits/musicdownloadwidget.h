#ifndef MUSICDOWNLOADWIDGET_H
#define MUSICDOWNLOADWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "musicabstractmovewidget.h"
#include "musicabstracttablewidget.h"
#include "musicabstractqueryrequest.h"

namespace Ui {
class MusicDownloadWidget;
}

class QLabel;

#define TABLE_ITEM_ROLE Qt::UserRole + 1

/*! @brief The class of the music song atrribute.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicDownloadTableItemRole
{
    int m_bitrate;
    QString m_format;
    QString m_size;

    MusicDownloadTableItemRole()
        : m_bitrate(-1)
    {

    }

    MusicDownloadTableItemRole(int b, const QString &f, const QString &s)
        : m_bitrate(b),
          m_format(f),
          m_size(s)
    {

    }

    inline bool isEmpty() const
    {
        return m_bitrate == -1 && m_format.isEmpty() && m_size.isEmpty();
    }

    inline bool isEqual(const MusicDownloadTableItemRole &r) const
    {
        return m_bitrate == r.m_bitrate && m_format == r.m_format && m_size == r.m_size;
    }
};


/*! @brief The class of the download table item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadTableItem : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadTableItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadTableItem(QWidget *parent = nullptr);
    ~MusicDownloadTableItem();

    /*!
     * Set current item icon.
     */
    void setIcon(const QString &name);
    /*!
     * Set current item other infomation(size\type\time).
     */
    void setInformation(const QString &info);
    /*!
     * Set current item text name.
     */
    void setText(const QString &text);

private:
    QLabel *m_text, *m_information, *m_icon;

};


/*! @brief The class of the download table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadTableWidget(QWidget *parent = nullptr);
    ~MusicDownloadTableWidget();

    /*!
     * Create current item by given bitrate\type\icon.
     */
    void addCellItem(const MusicObject::MusicSongProperty &prop, const QString &type, const QString &icon);
    /*!
     * Get current bitrate from item.
     */
    MusicDownloadTableItemRole currentItemRole() const;

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;

private:
    QList<QObject*> m_items;

};


/*! @brief The class of the download widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadWidget(QWidget *parent = nullptr);
    ~MusicDownloadWidget();

    /*!
     * Set current name to search and download musics.
     */
    void setSongName(const QString &name, MusicAbstractQueryRequest::QueryType type);
    /*!
     * Set current name to search and download musics.
     */
    void setSongName(const MusicObject::MusicSongInformation &info, MusicAbstractQueryRequest::QueryType type);

Q_SIGNALS:
    /*!
     * Data download changed.
     */
    void dataDownloadChanged();

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void downLoadFinished();
    /*!
     * Change data download save path.
     */
    void downloadDirSelected();
    /*!
     * Start to download data from net.
     */
    void startRequest();
    /*!
     * Data download is finished.
     */
    void dataDownloadFinished();

private:
    /*!
     * Create all widget in layout.
     */
    void initialize();
    /*!
     * Enable or disable control state.
     */
    void controlEnabled(bool enable);
    /*!
     * Set widget fixed height.
     */
    void setFixedHeightWidget(QWidget *w, int height);
    /*!
     * Set widget moved into given pos.
     */
    void setMoveWidget(QWidget *w, int pos);

    /*!
     * Get match music song infomation.
     */
    MusicObject::MusicSongInformation matchMusicSongInformation();
    /*!
     * Create current all items.
     */
    void addCellItems(const MusicObject::MusicSongPropertyList &props);
    /*!
     * Strat to download music.
     */
    void startRequestMusic();
    /*!
     * Strat to download music.
     */
    void startRequestMusic(const MusicObject::MusicSongInformation &info);
    /*!
     * Strat to download movie.
     */
    void startRequestMovie();
    /*!
     * Strat to download movie.
     */
    void startRequestMovie(const MusicObject::MusicSongInformation &info);

    Ui::MusicDownloadWidget *m_ui;
    bool m_querySingleInfo;
    MusicAbstractQueryRequest *m_networkRequest;
    MusicObject::MusicSongInformation m_songInfo;
    MusicAbstractQueryRequest::QueryType m_queryType;

};

#endif // MUSICDOWNLOADWIDGET_H
