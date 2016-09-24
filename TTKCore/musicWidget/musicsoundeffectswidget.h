#ifndef MUSICSOUNDEFFECTSWIDGET_H
#define MUSICSOUNDEFFECTSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    enum Type
    {
        Null,       /// sound null effect
        BS2B,       /// sound BS2B effect
        Crossfade,  /// sound Crossfade effect
        Stereo,     /// sound Stereo effect
        LADSPA,     /// sound LADSPA effect
        SoX         /// sound SoX Resampler effect
    };
    explicit MusicSoundEffectsItemWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSoundEffectsItemWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setText(const QString &text);
    /*!
     * Set sound effect plugin name.
     */
    void setType(Type type);
    /*!
     * Set sound effect plugin type.
     */

    void setPluginEnable(bool enable);
    /*!
     * Set plugin enable or not.
     */
    bool pluginEnable() const;
    /*!
     * Get plugin enable or not.
     */

public Q_SLOTS:
    void setPluginEnable();
    /*!
     * Set plugin enable or not.
     */
    void soundEffectCheckBoxChanged(bool state);
    /*!
     * Sound effect checkBox changed.
     */
    void soundEffectValueChanged();
    /*!
     * Sound effect button value changed.
     */

protected:
    QString transformQStringFromEnum();
    /*!
     * Transform string from enum.
     */

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
public:
    explicit MusicSoundEffectsWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    virtual ~MusicSoundEffectsWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setParentConnect(QObject *object);
    /*!
     * Set parent connection.
     */

Q_SIGNALS:
    void volumeChanged(int value);
    /*!
     * Set current player volume.
     */
    void setEqInformation();
    /*!
     * Read equalizer effect from config file.
     */

public Q_SLOTS:
    void equalizerButtonChanged(bool state);
    /*!
     * Equalizer button state changed.
     */
    void equalizerButtonChanged();
    /*!
     * Equalizer button state changed.
     */
    void stateComboBoxChanged(int index);
    /*!
     * State combobox index changed.
     */
    void volumeSliderChanged(int value);
    /*!
     * Volume slider changed.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void readSoundEffect();
    /*!
     * Read sound effect function.
     */
    void writeSoundEffect();
    /*!
     * Write sound effect function.
     */

    Ui::MusicSoundEffectsWidget *ui;

};

#endif // MUSICSOUNDEFFECTSWIDGET_H
