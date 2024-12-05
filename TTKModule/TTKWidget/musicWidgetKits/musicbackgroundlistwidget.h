#ifndef MUSICBACKGROUNDLISTWIDGET_H
#define MUSICBACKGROUNDLISTWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include <QLabel>
#include <QGridLayout>
#include "musicbackgroundconfigmanager.h"

/*! @brief The class of the background list item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundListItem : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBackgroundListItem)
public:
    /*!
     * Object constructor.
     */
    explicit MusicBackgroundListItem(QWidget *parent = nullptr);

    /*!
     * Set item file name.
     */
    inline void setFileName(const QString &name) { m_name = name; }
    /*!
     * Get item file name.
     */
    inline QString fileName() const { return m_name; }
    /*!
     * Set item file name.
     */
    inline void setFilePath(const QString &path) { m_path = path; }
    /*!
     * Get item file name.
     */
    inline QString filePath() const { return m_path; }

    /*!
     * Update pix image.
     */
    void updatePixmap();
    /*!
     * Update pix image.
     */
    void updatePixmap(const MusicBackgroundImage &image);
    /*!
     * Current item contains or not.
     */
    bool contains(const MusicSkinItem &item) const;

    /*!
     * Select the current item.
     */
    void setSelected(bool v);
    /*!
     * Get current selected item state.
     */
    inline bool isSelected() const { return m_isSelected; }

    /*!
     * Select the current item enable or not.
     */
    void setSelectEnabled(bool v);
    /*!
     * Get current selected item state.
     */
    inline bool isSelectEnabled() const { return m_selectedEnabled; }

    /*!
     * Close option set the current item.
     */
    void setCloseEnabled(bool v);
    /*!
     * Get close option set the current item.
     */
    inline bool isCloseEnabled() const { return m_closeEnabled; }

    /*!
     * Show title name set the current item.
     */
    void setShowNameEnabled(bool v);
    /*!
     * Get show title name set the current item.
     */
    inline bool isShowNameEnabled() const { return m_showNameEnabled; }

Q_SIGNALS:
    /*!
     * Current item close clicked.
     */
    void closeClicked(MusicBackgroundListItem *item);
    /*!
     * Current item clicked.
     */
    void itemClicked(MusicBackgroundListItem *item);

private:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void paintEvent(QPaintEvent *event) override final;

    bool m_isSelected, m_printMask;
    bool m_selectedEnabled, m_closeEnabled, m_showNameEnabled;
    QString m_name, m_path;
    MusicSkinItem m_imageInfo;

};


/*! @brief The class of the background list widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundListWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBackgroundListWidget)
public:
    enum Module
    {
        CachedModule,
        StackedModule,
        DailyModule,
        OnlineModule
    };

    /*!
     * Object constructor.
     */
    explicit MusicBackgroundListWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicBackgroundListWidget();

    /*!
     * Select current item by name when the widget show.
     */
    void setCurrentItemName(const QString &name);

    /*!
     * Set background list rtpe.
     */
    inline void setType(Module type) { m_type = type; }
    /*!
     * Get background list rtpe.
     */
    inline Module type() const { return m_type; }

    /*!
     * Clear select state.
     */
    void clearState();
    /*!
     * Clear All Items.
     */
    void clearItems();

    /*!
     * Create cell item by input data.
     */
    void addCellItem(const QString &icon, bool state);
    /*!
     * Create cell item by input data.
     */
    void addCellItem(const QString &name, const QString &path, bool state);

    /*!
     * Update item by backgroud image.
     */
    void updateItem(const MusicBackgroundImage &image, const QString &path);

    /*!
     * Current item contains or not.
     */
    bool contains(const QString &name) const;
    /*!
     * Current item contains or not.
     */
    bool contains(const MusicBackgroundImage &image) const;

    /*!
     * Current item index at container.
     */
    int find(MusicBackgroundListItem *item) const;
    /*!
     * Find item by name.
     */
    MusicBackgroundListItem* find(const QString &name) const;
    /*!
     * Find item by image.
     */
    MusicBackgroundListItem* find(const MusicBackgroundImage &image) const;

    /*!
     * Update last item state.
     */
    void updateLastItem();

    /*!
     * Item count.
     */
    inline int count() const { return m_items.count(); }

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void itemClicked(int type, const QString &name);

private Q_SLOTS:
    /*!
     * Current item close clicked.
     */
    void itemCloseClicked(MusicBackgroundListItem *item);
    /*!
     * Current item has clicked.
     */
    void currentItemClicked(MusicBackgroundListItem *item);

private:
    Module m_type;
    QGridLayout *m_gridLayout;
    MusicBackgroundListItem *m_currentItem;
    QList<MusicBackgroundListItem*> m_items;

};

#endif // MUSICBACKGROUNDLISTWIDGET_H
