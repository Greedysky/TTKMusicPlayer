#ifndef MUSICBACKGROUNDSKINDIALOG_H
#define MUSICBACKGROUNDSKINDIALOG_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicBackgroundSkinDialog;
}

class MusicBackgroundListWidget;
class MusicBackgroundOnlineWidget;
class MusicBackgroundDailyWidget;
class MusicBackgroundRemoteWidget;

/*! @brief The class of the local background widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicBackgroundSkinDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicBackgroundSkinDialog)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicBackgroundSkinDialog(QWidget *parent = nullptr);
    ~MusicBackgroundSkinDialog();

    /*!
     * Set artist background picture by name.
     */
    static QPixmap setBackgroundUrl(QString &name);
    /*!
     * Theme valid check.
     */
    static bool themeValidCheck(QString &name, QString &path);
    /*!
     * Copy art file to local themes dir path.
     */
    static QString cpoyArtistFileToLocal(const QString &path);

    /*!
     * Update artist file theme.
     */
    void updateArtistFileTheme(const QString &theme);
    /*!
     * Select current item by name\alpha\listAlpha when the widget show.
     */
    void setCurrentBackgroundTheme(const QString &theme, int skin, int list);

    /*!
     * Get list background skin alpha.
     */
    int backgroundListAlpha() const;
    /*!
     * Get background window skin enable.
     */
    bool backgroundTransparentEnable() const;

    /*!
     * Set skin transparent tool text.
     */
    void setSkinTransToolText(int value);
    /*!
     * Set list transparent tool text.
     */
    void setListTransToolText(int value);

Q_SIGNALS:
    /*!
     * Update app background when user change the current background.
     */
    void currentTextChanged(const QString &text);

public Q_SLOTS:
    /*!
     * Change index to palette dialog.
     */
    void showPaletteDialog();
    /*!
     * Change index to palette dialog.
     */
    void showPaletteDialog(const QString &path);
    /*!
     * Change index to custom skin dialog.
     */
    void showCustomSkinDialog();
    /*!
     * Background list widget changed.
     */
    void backgroundListWidgetChanged(int index);
    /*!
     * Classical list widget item has clicked.
     */
    void classicalListWidgetItemClicked(int type, const QString &name);
    /*!
     * Remote background list widget item has clicked.
     */
    void remoteListWidgetItemClicked(int type, const QString &name);
    /*!
     * Update app background when user change the current background.
     */
    void currentColorChanged(const QString &path);
    /*!
     * Window transparent changed.
     */
    void windowTransparentChanged(bool state);

private:
    /*!
     * Background list widget item has clicked changed.
     */
    void listWidgetItemClicked(MusicBackgroundListWidget *item, const QString &name);
    /*!
     * Background list widget item has clicked changed.
     */
    void listWidgetItemClicked(MusicBackgroundRemoteWidget *item, const QString &name);
    /*!
     * Add all theme into list.
     */
    void addThemeListWidgetItem();
    /*!
     * Add all theme into list.
     */
    void addThemeListWidgetItem(MusicBackgroundListWidget *item, const QString &dir, bool state);
    /*!
     * Copy custom file to local themes dir path.
     */
    void cpoyFileFromLocal(const QString &path);

    /*!
     * Find all theme in local directory.
     */
    static void findThemeListByPath(const QString &dir, TTKIntList &data);
    /*!
     * Copy custom file index.
     */
    static int cpoyFileToLocalIndex();
    /*!
     * Copy custom file .
     */
    static int cpoyFileToLocal(const QString &path);

    Ui::MusicBackgroundSkinDialog *m_ui;
    int m_stackThemeIndex;
    MusicBackgroundListWidget *m_cacheBackgroundList;
    MusicBackgroundListWidget *m_stackBackgroundList;
    MusicBackgroundDailyWidget *m_dailyBackgroundList;
    MusicBackgroundOnlineWidget *m_onlineBackgroundList;

};

#endif // MUSICBACKGROUNDSKINDIALOG_H
