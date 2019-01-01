#ifndef MUSICSOUNDEFFECTSWIDGET_H
#define MUSICSOUNDEFFECTSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

namespace Ui {
class MusicSoundEffectsWidget;
}

/*! @brief The class of the sound effect item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSoundEffectsItemWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundEffectsItemWidget)
public:
    enum Type
    {
        Null,       /*!< sound null effect*/
        BS2B,       /*!< sound BS2B effect*/
        Crossfade,  /*!< sound Crossfade effect*/
        Stereo,     /*!< sound Stereo effect*/
        LADSPA,     /*!< sound LADSPA effect*/
        SoX,        /*!< sound SoX Resampler effect*/
        SRC         /*!< sound srconverter effect*/
    };
    /*!
     * Object contsructor.
     */
    explicit MusicSoundEffectsItemWidget(QWidget *parent = nullptr);

    ~MusicSoundEffectsItemWidget();

    /*!
     * Set sound effect plugin name.
     */
    void setText(const QString &text);
    /*!
     * Set sound effect plugin type.
     */
    void setType(Type type);

    /*!
     * Set plugin enable or not.
     */
    void setPluginEnable(bool enable);
    /*!
     * Get plugin enable or not.
     */
    bool pluginEnable() const;

    /*!
     * Sound effect changed.
     */
    static void soundEffectChanged(Type type, bool enable);

public Q_SLOTS:
    /*!
     * Set plugin enable or not.
     */
    void setPluginEnable();
    /*!
     * Sound effect checkBox changed.
     */
    void soundEffectCheckBoxChanged(bool state);
    /*!
     * Sound effect button value changed.
     */
    void soundEffectValueChanged();

protected:
    /*!
     * Transform string from enum.
     */
    static QString transformQStringFromEnum(Type type);

    Type m_type;
    bool m_enable;
    QLabel *m_textLabel;
    QPushButton *m_settingButton, *m_openButton;

};


/*! @brief The class of the sound effect widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSoundEffectsWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundEffectsWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundEffectsWidget(QWidget *parent = nullptr);

    virtual ~MusicSoundEffectsWidget();

    /*!
     * Set parent connection.
     */
    void setParentConnect(QObject *object);

Q_SIGNALS:
    /*!
     * Set current player volume.
     */
    void volumeChanged(int value);
    /*!
     * Read equalizer effect from config file.
     */
    void setEqInformation();

public Q_SLOTS:
    /*!
     * Equalizer button state changed.
     */
    void equalizerButtonChanged(bool state);
    /*!
     * Equalizer button state changed.
     */
    void equalizerButtonChanged();
    /*!
     * State combobox index changed.
     */
    void stateComboBoxChanged(int index);
    /*!
     * Volume slider changed.
     */
    void volumeSliderChanged(int value);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Read sound effect function.
     */
    void readSoundEffect();
    /*!
     * Write sound effect function.
     */
    void writeSoundEffect();

    Ui::MusicSoundEffectsWidget *m_ui;

};

#endif // MUSICSOUNDEFFECTSWIDGET_H
