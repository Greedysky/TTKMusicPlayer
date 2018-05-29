#ifndef MUSICSETTINGWIDGET_H
#define MUSICSETTINGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "musicabstractmovedialog.h"
#include "musicuiobject.h"
#include "musicsettingmanager.h"
#include "musicabstracttablewidget.h"

class QComboBox;

namespace Ui {
class MusicSettingWidget;
}

/*! @brief The class of the function tool item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_WIDGET_EXPORT MusicFunctionItem
{
    MusicFunctionItem()
    {

    }

    MusicFunctionItem(const QString &icon, const QString &name)
    {
        m_icon = icon;
        m_name = name;
    }

    QString m_icon;
    QString m_name;
}MusicFunctionItem;
TTK_DECLARE_LISTS(MusicFunctionItem)

/*! @brief The class of the list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFunctionTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFunctionTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFunctionTableWidget(QWidget *parent = 0);

    /*!
     * Add table list items by index and icons and paths.
     */
    void addFunctionItems(int index, const MusicFunctionItems &items);

Q_SIGNALS:
    /*!
     * Current list index changed.
     */
    void currentIndexChanged(int index);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override;

    int m_listIndex;
};


/*! @brief The class of the setting parameters widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSettingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSettingWidget)
public:
    enum Type
    {
        Inline, /*!< lrc inline type*/
        Desktop /*!< lrc desktop type*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicSettingWidget(QWidget *parent = 0);

    virtual ~MusicSettingWidget();

    /*!
     * Init controller parameter to widget.
     */
    void initControllerParameter();

Q_SIGNALS:
    /*!
     * Parameters setting changed emit.
     */
    void parameterSettingChanged();

public Q_SLOTS:
    /*!
     * Clear all function tables selection.
     */
    void clearFunctionTableSelection();
    /*!
     * Set global hotkey box changed.
     */
    void globalHotkeyBoxChanged(bool state);

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
    void downloadDirSelected(int index);

    /*!
     * Version update check changed.
     */
    void otherVersionUpdateChanged();

    /*!
     * Change to desktop lrc widget.
     */
    void changeDesktopLrcWidget();
    /*!
     * Change to inline lrc widget.
     */
    void changeInlineLrcWidget();
    /*!
     * Change to download widget.
     */
    void changeDownloadWidget();

    /*!
     * Inline lrc foreground change.
     */
    void inlineLrcFgChanged();
    /*!
     * Inline lrc background change.
     */
    void inlineLrcBgChanged();
    /*!
     * Default lrc color change by index.
     */
    void defaultLrcColorChanged(int index);
    /*!
     * Inline lrc transparent changed by index.
     */
    void inlineLrcTransChanged(int index);
    /*!
     * Show inline lrc preview.
     */
    void showInlineLrcDemo();
    /*!
     * Reset inline parameter.
     */
    void resetInlineParameter();

    /*!
     * Desktop lrc foreground change.
     */
    void desktopFgChanged();
    /*!
     * Desktop lrc background change.
     */
    void desktopBgChanged();
    /*!
     * Default desktop lrc color change by index.
     */
    void defaultDesktopLrcColorChanged(int index);
    /*!
     * Desktop lrc transparent changed by index.
     */
    void desktopLrcTransChanged(int index);
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
    void musicFadeInAndOutClicked(bool state);

    /*!
     * Save the change results.
     */
    void commitTheResults();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Select function table index.
     */
    void selectFunctionTableIndex(int row, int col);
    /*!
     * Init normal setting stack widget.
     */
    void initNormalSettingWidget();
    /*!
     * Init other setting stack widget.
     */
    void initOtherSettingWidget();
    /*!
     * Init download stack widget.
     */
    void initDownloadWidget();
    /*!
     * Init desktop lrc stack widget.
     */
    void initDesktopLrcWidget();
    /*!
     * Init inline lrc stack widget.
     */
    void initInlineLrcWidget();
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
    /*!
     * Set lrc color value by type and value type.
     */
    void lcrColorValue(Type key, const QString &type, QLabel *obj);
    /*!
     * Set lrc default color by type and index.
     */
    void lrcColorByDefault(Type key, int index);
    /*!
     * Set lrc transparent by type and value.
     */
    void lrcTransparentValue(Type key, int value) const;
    /*!
     * Apply current proxy available.
     */
    bool applyNetworkProxy();
    /*!
     * Set network proxy by type.
     */
    bool setNetworkProxyByType(int type);

    Ui::MusicSettingWidget *m_ui;
    QList<QColor> m_lrcSelectedFg, m_lrcSelectedBg;
    QList<QColor> m_DlrcSelectedFg, m_DlrcSelectedBg;

};

#endif // MUSICSETTINGWIDGET_H
