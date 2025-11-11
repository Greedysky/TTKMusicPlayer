#ifndef MUSICSETTINGWIDGET_H
#define MUSICSETTINGWIDGET_H

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

#include "musicsettingmanager.h"
#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"

class QComboBox;
class MusicGlobalHotKeyEdit;

namespace Ui {
class MusicSettingWidget;
}

/*! @brief The class of the function tool item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicFunctionItem
{
    QString m_icon;
    QString m_name;

    MusicFunctionItem(const QString &icon, const QString &name) noexcept
        : m_icon(icon),
          m_name(name)
    {

    }
};
TTK_DECLARE_LIST(MusicFunctionItem);

/*! @brief The class of the list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFunctionTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFunctionTableWidget(QWidget *parent = nullptr);

    /*!
     * Add table list items by index and icons and paths.
     */
    void addCellItems(int index, const MusicFunctionItemList &items);

Q_SIGNALS:
    /*!
     * Current list index changed.
     */
    void currentIndexChanged(int index);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;

private:
    int m_listIndex;

};


/*! @brief The class of the setting parameters widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSettingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    enum class Lrc
    {
        Interior, /*!< lrc interior type*/
        Desktop   /*!< lrc desktop type*/
    };

    enum class Proxy
    {
        Test,   /*!< proxy test type*/
        Apply   /*!< proxy apply type*/
    };

    enum SettingType
    {
        Normal,        /*!< Normal Setting Type*/
        Hotkey,        /*!< Hotkey Setting Type*/
        Download,      /*!< Download Setting Type*/
        Spectrum,      /*!< Spectrum Setting Type*/
        Other,         /*!< Other Setting Type*/
        DesktopLrc,    /*!< DesktopLrc Setting Type*/
        InteriorLrc,   /*!< InteriorLrc Setting Type*/
        SoundEffect,   /*!< SoundEffect Setting Type*/
        Audio,         /*!< Audio Setting Type*/
        Network,       /*!< Network Setting Type*/
    };

    /*!
     * Object constructor.
     */
    explicit MusicSettingWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSettingWidget();

    /*!
     * Init widget and parameter.
     */
    void initialize();

Q_SIGNALS:
    /*!
     * Parameters setting changed.
     */
    void parameterSettingChanged();

public Q_SLOTS:
    /*!
     * Override close function.
     */
    void close();

    /*!
     * Change to desktop lrc widget.
     */
    void changeDesktopLrcWidget();
    /*!
     * Change to interior lrc widget.
     */
    void changeInteriorLrcWidget();
    /*!
     * Change to download widget.
     */
    void changeDownloadWidget();

private Q_SLOTS:
    /*!
     * Clear all function tables selection.
     */
    void clearFunctionTableSelection();
    /*!
     * File association changed.
     */
    void fileAssociationChanged();
    /*!
     * Set global hotkey box changed.
     */
    void globalHotkeyBoxChanged(bool state);

    /*!
     * Set clean download cache.
     */
    void downloadCacheClean();
    /*!
     * Set download file name rules.
     */
    void downloadFileNameRuleChanged(QAction *action);
    /*!
     * Server plugin manager changed.
     */
    void serverPluginChanged();
    /*!
     * Set enable or disable download cache.
     */
    void downloadGroupCached(int index);
    /*!
     * Set enable or disable download speed limit.
     */
    void downloadGroupSpeedLimit(int index);
    /*!
     * Set select download dir or lrc dir.
     */
    void downloadDirChanged(int index);

    /*!
     * Version update check changed.
     */
    void rippleVersionUpdateChanged();
    /*!
     * Ripple spectrum color changed.
     */
    void rippleSpectrumColorChanged();
    /*!
     * Ripple spectrum transparent changed by value.
     */
    void rippleSpectrumTransChanged(int value);
    /*!
     * Ripple spectrum enable changed.
     */
    void rippleSpectrumEnableClicked(bool state);
    /*!
     * Ripple low power mode enable changed.
     */
    void rippleLowPowerEnableBoxClicked(bool state);
    /*!
     * Plugin manager changed.
     */
    void otherPluginManagerChanged();

    /*!
     * Interior lrc foreground change.
     */
    void interiorLrcFrontgroundChanged();
    /*!
     * Interior lrc background change.
     */
    void interiorLrcBackgroundChanged();
    /*!
     * Default lrc color change by index.
     */
    void defaultLrcColorChanged(int value);
    /*!
     * Interior lrc transparent changed by value.
     */
    void interiorLrcTransChanged(int value);
    /*!
     * Show interior lrc preview.
     */
    void showInteriorLrcDemo();
    /*!
     * Reset interior parameter.
     */
    void resetInteriorParameter();

    /*!
     * Desktop lrc foreground change.
     */
    void desktopFrontgroundChanged();
    /*!
     * Desktop lrc background change.
     */
    void desktopBackgroundChanged();
    /*!
     * Default desktop lrc color change by index.
     */
    void defaultDesktopLrcColorChanged(int value);
    /*!
     * Desktop lrc transparent changed by value.
     */
    void desktopLrcTransChanged(int value);
    /*!
     * Show desktop lrc preview.
     */
    void showDesktopLrcDemo();
    /*!
     * Reset desktop parameter.
     */
    void resetDesktopParameter();

    /*!
     * Set network proxy control enabled or not.
     */
    void setNetworkProxyControl(int enable);
    /*!
     * Test current proxy available.
     */
    void testNetworkProxy();
    /*!
     * Test current proxy available changed.
     */
    void testProxyStateChanged(bool state);
    /*!
     * Test current network connection available.
     */
    void testNetworkConnection();
    /*!
     * Check current network connection available.
     */
    void checkNetworkConnection();
    /*!
     * Test current network connection available changed.
     */
    void testNetworkConnectionStateChanged(const QString &name);

    /*!
     * Music fade in and out changed.
     */
    void fadeInAndOutClicked(bool state);

    /*!
     * Save the parameter setting results.
     */
    void saveParameterSettings();

private Q_SLOTS:
    /*!
     * Set scroll widget page index.
     */
    void setScrollWidgetPageIndex(int index);
    /*!
     * Scroll widget index changed.
     */
    void scrollWidgetValueChanged(int value);

private:
    /*!
     * Init scroll widget page widget.
     */
    void initScrollWidgetPage();
    /*!
     * Init normal setting stack widget.
     */
    void initNormalSettingWidget();
    /*!
     * Init download stack widget.
     */
    void initDownloadWidget();
    /*!
     * Init spectrum setting stack widget.
     */
    void initSpectrumSettingWidget();
    /*!
     * Init other setting stack widget.
     */
    void initOtherSettingWidget();
    /*!
     * Init desktop lrc stack widget.
     */
    void initDesktopLrcWidget();
    /*!
     * Init interior lrc stack widget.
     */
    void initInteriorLrcWidget();
    /*!
     * Init sound effect stack widget.
     */
    void initSoundEffectWidget();
    /*!
     * Init audio setting stack widget.
     */
    void initAudioSettingWidget();
    /*!
     * Init network stack widget.
     */
    void initNetworkWidget();

private:
    /*!
     * Select function table index.
     */
    void selectFunctionTableIndex(int row, int column);
    /*!
     * Set lrc color value by type and value type.
     */
    void lcrColorValue(Lrc key, const QString &type, QLabel *obj);
    /*!
     * Set lrc default color by type and index.
     */
    void lrcColorByDefault(Lrc key, int index);
    /*!
     * Set lrc transparent by type and value.
     */
    void lrcTransparentValue(Lrc key, int value) const;
    /*!
     * Set network proxy by type.
     */
    bool setNetworkProxyByType(Proxy type);

    Ui::MusicSettingWidget *m_ui;

    QVector<MusicGlobalHotKeyEdit*> m_hotKeyEdits;

};

#endif // MUSICSETTINGWIDGET_H
