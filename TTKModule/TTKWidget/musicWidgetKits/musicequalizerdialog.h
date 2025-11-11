#ifndef MUSICEQUALIZERDIALOG_H
#define MUSICEQUALIZERDIALOG_H

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

#include <QDialog>
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicEqualizerDialog;
}

/*! @brief The class of the equalizer widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicEqualizerDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicEqualizerDialog)
public:
    /*!
     * Object constructor.
     */
    explicit MusicEqualizerDialog(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicEqualizerDialog();

Q_SIGNALS:
    /*!
     * Set equlizer enable state changed.
     */
    void setEnabledEffect(bool enabled);
    /*!
     * Current equlizer parameters changed.
     */
    void setEqualizerEffect(const TTKIntList &effect);

public Q_SLOTS:
    /*!
     * Set equlizer enable.
     */
    void setEnable();
    /*!
     * Reset equlizer parameters.
     */
    void reset();
    /*!
     * Current equlizer parameters has changed by changed index.
     */
    void sliderValueChanged();
    /*!
     * Equlizer presets index changed by change index.
     */
    void equlizerTypeChanged(int index);

private:
    /*!
     * Init widget and parameter.
     */
    void initialize();
    /*!
     * Enable or disable control state.
     */
    void setControlEnabled(bool enabled) const;
    /*!
     * Read equalizer info from config.
     */
    void readInformation();
    /*!
     * Write equalizer info from config.
     */
    void writeInformation() const;
    /*!
     * Init equalizer parameters value.
     */
    void initEqualizeValue();
    /*!
     * Emit current equalizer parameters value.
     */
    void parameterSubmit();

    Ui::MusicEqualizerDialog *m_ui;
    bool m_enable;
    TTKIntList m_equalizeValue;

};

#endif // MUSICEQUALIZERDIALOG_H
