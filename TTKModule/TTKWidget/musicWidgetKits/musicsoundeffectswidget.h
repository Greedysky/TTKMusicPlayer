#ifndef MUSICSOUNDEFFECTSWIDGET_H
#define MUSICSOUNDEFFECTSWIDGET_H

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

#include "musicqmmputils.h"
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSoundEffectsWidget;
}

/*! @brief The class of the sound effect item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSoundEffectsItemWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSoundEffectsItemWidget(const MusicPluginProperty &property, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSoundEffectsItemWidget();

    /*!
     * Get plugin module type.
     */
    inline QString type() const { return m_property.m_type; }
    /*!
     * Set plugin enable or not.
     */
    void setPluginEnabled(bool enabled);
    /*!
     * Get plugin enable or not.
     */
    bool pluginEnabled() const;

public Q_SLOTS:
    /*!
     * Set plugin enable or not.
     */
    void setPluginEnabled();
    /*!
     * Sound effect button value changed.
     */
    void soundEffectValueChanged();

private:
    bool m_enabled;
    QLabel *m_textLabel;
    QPushButton *m_settingButton, *m_openButton;
    MusicPluginProperty m_property;

};


/*! @brief The class of the sound effect widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSoundEffectsWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundEffectsWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSoundEffectsWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSoundEffectsWidget();

    /*!
     * Update sound effect config.
     */
    static void updateConfig(bool v);

public Q_SLOTS:
    /*!
     * State combobox index changed.
     */
    void stateComboBoxChanged(int index);

private:
    /*!
     * Init widget and parameter.
     */
    void initialize();

    Ui::MusicSoundEffectsWidget *m_ui;
    QList<MusicSoundEffectsItemWidget*> m_items;

};

#endif // MUSICSOUNDEFFECTSWIDGET_H
