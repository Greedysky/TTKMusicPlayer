#ifndef MUSICSPECTRUMLAYOUTWIDGET_H
#define MUSICSPECTRUMLAYOUTWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include <QCheckBox>
#include <QBoxLayout>
#include "musictoolmenuwidget.h"
#include "ttkclickedlabel.h"

/*! @brief The class of the music spectrum layout item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumLayoutItem : public TTKClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumLayoutItem)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSpectrumLayoutItem(QWidget *parent = nullptr);

    /*!
     * Create cell item by input data.
     */
    void addCellItem(const QString &item, const QString &tip);
    /*!
     * Set spectrum item check state.
     */
    void setChecked(bool checked);
    /*!
     * Get spectrum item check state.
     */
    bool isChecked() const;

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    bool m_isSelected;
};


/*! @brief The class of the music spectrum layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumLayoutWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumLayoutWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSpectrumLayoutWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSpectrumLayoutWidget();

Q_SIGNALS:
    /*!
     * Spectrum item check state changed.
     */
    void stateChanged(bool &state, const QString &name);

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    virtual void popupMenu() override final;
    /*!
     * Label selected state changed.
     */
    void labelClicked(int index);

protected:
    using SpectrumInfo = QPair<QString, QString>;
    using SpectrumInfoList = QList<SpectrumInfo>;

protected:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const = 0;
    /*!
     * Create all widget in layout.
     */
    void initialize();
    /*!
     * Create cell items by input data.
     */
    void addCellItems(const SpectrumInfoList &items);

    bool m_exclusive;
    QVBoxLayout *m_containLayout;
    QList<MusicSpectrumLayoutItem*> m_items;

};


/*! @brief The class of the music spectrum normal layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumNormalLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumNormalLayoutWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSpectrumNormalLayoutWidget(QWidget *parent = nullptr);

private:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override final;

};


/*! @brief The class of the music spectrum plus layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumPlusLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumPlusLayoutWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSpectrumPlusLayoutWidget(QWidget *parent = nullptr);

private:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override final;

};


/*! @brief The class of the music spectrum flow layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumFlowLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumFlowLayoutWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSpectrumFlowLayoutWidget(QWidget *parent = nullptr);

private:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override final;

};


/*! @brief The class of the music spectrum wave layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumWaveLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumWaveLayoutWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSpectrumWaveLayoutWidget(QWidget *parent = nullptr);

private:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override final;

};


/*! @brief The class of the music spectrum florid layout widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumFloridLayoutWidget : public MusicSpectrumLayoutWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumFloridLayoutWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSpectrumFloridLayoutWidget(QWidget *parent = nullptr);

private:
    /*!
     * Spectrum types list.
     */
    virtual QStringList spectrumTypeList() const override final;

};

#endif // MUSICSPECTRUMLAYOUTWIDGET_H
