#ifndef MUSICSPECTRUMLAYOUTWIDGET_H
#define MUSICSPECTRUMLAYOUTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include <QCheckBox>
#include <QBoxLayout>
#include "musicclickedlabel.h"
#include "musictoolmenuwidget.h"

/*! @brief The class of the music spectrum layout item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumLayoutItem : public MusicClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumLayoutItem)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpectrumLayoutItem(QWidget *parent = nullptr);

    virtual ~MusicSpectrumLayoutItem();

    /*!
     * Create spectrum item.
     */
    void addItem(const QString &item, const QString &tip);
    /*!
     * Set spectrum item check state.
     */
    void setCheck(bool check);
    /*!
     * Get spectrum item check state.
     */
    bool isChecked() const;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    bool m_isSelected;
};


/*! @brief The class of the music spectrum layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumLayoutWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumLayoutWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpectrumLayoutWidget(QWidget *parent = nullptr);

    virtual ~MusicSpectrumLayoutWidget();

Q_SIGNALS:
    /*!
     * Spectrum item check state changed.
     */
    void stateChanged(bool &state, const QString &name);

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    virtual void popupMenu() override;
    /*!
     * Label selected state changed.
     */
    void labelClicked(int index);

protected:
    typedef QPair<QString, QString> ItemInfo;
    typedef QList< ItemInfo > ItemInfos;

protected:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const = 0;
    /*!
     * Create all widget in layout.
     */
    void initWidget();
    /*!
     * Create all spectrum items.
     */
    void addItems(const ItemInfos &items);

    bool m_exclusive;
    QVBoxLayout *m_containLayout;
    QList<MusicSpectrumLayoutItem*> m_items;

};


/*! @brief The class of the music spectrum normal layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumNormalLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumNormalLayoutWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpectrumNormalLayoutWidget(QWidget *parent = nullptr);

    virtual ~MusicSpectrumNormalLayoutWidget();

protected:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override;

};


/*! @brief The class of the music spectrum plus layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumPlusLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumPlusLayoutWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpectrumPlusLayoutWidget(QWidget *parent = nullptr);

    virtual ~MusicSpectrumPlusLayoutWidget();

protected:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override;

};


/*! @brief The class of the music spectrum florid layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSpectrumFloridLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumFloridLayoutWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpectrumFloridLayoutWidget(QWidget *parent = nullptr);

    virtual ~MusicSpectrumFloridLayoutWidget();

protected:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override;

};

#endif // MUSICSPECTRUMLAYOUTWIDGET_H
