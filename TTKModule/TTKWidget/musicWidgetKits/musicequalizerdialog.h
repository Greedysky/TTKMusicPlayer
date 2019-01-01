#ifndef MUSICEQUALIZERDIALOG_H
#define MUSICEQUALIZERDIALOG_H

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

#include <QDialog>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicEqualizerDialog;
}

class QSlider;
class QSignalMapper;

/*! @brief The class of the equalizer widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicEqualizerDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicEqualizerDialog)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicEqualizerDialog(QWidget *parent = nullptr);

    virtual ~MusicEqualizerDialog();

Q_SIGNALS:
    /*!
     * Current equlizer parameters changed emit.
     */
    void setEqEffect(const MIntList &effect);
    /*!
     * Set equlizer enable state changed emit.
     */
    void setEnaleEffect(bool enable);

public Q_SLOTS:
    /*!
     * Set equlizer enable.
     */
    void setEqEnable();
    /*!
     * Reset equlizer parameters.
     */
    void resetEq();
    /*!
     * Current equlizer parameters has changed by changed index.
     */
    void verticalSliderChanged(int index);
    /*!
     * Equlizer presets index changed by change index.
     */
    void eqChoiceIndexChanged(int index);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Init widget and parameters.
     */
    void init();
    /*!
     * Init and create slider.
     */
    void initSlider(QSlider *slider, int index);
    /*!
     * Enable or disable control state.
     */
    void setControlEnable(bool enable) const;
    /*!
     * Read equalizer info from config.
     */
    void readEqInformation();
    /*!
     * Write equalizer info from config.
     */
    void writeEqInformation() const;
    /*!
     * Init equalizer parameters value.
     */
    void initEqualizeValue();
    /*!
     * Emit current equalizer parameters value.
     */
    void emitParameter();

    Ui::MusicEqualizerDialog *m_ui;
    bool m_eable;
    bool m_eqChoiceSelected;
    MIntList m_equalizeValue;
    QSignalMapper *m_signalMapper;

};

#endif // MUSICEQUALIZERDIALOG_H
