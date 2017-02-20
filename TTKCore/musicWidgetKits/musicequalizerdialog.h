#ifndef MUSICEQUALIZERDIALOG_H
#define MUSICEQUALIZERDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    explicit MusicEqualizerDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicEqualizerDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void setEqEffect(const MusicObject::MIntList &effect);
    /*!
     * Current equlizer parameters changed emit.
     */
    void setEnaleEffect(bool enable);
    /*!
     * Set equlizer enable state changed emit.
     */

public Q_SLOTS:
    void setEqEnable();
    /*!
     * Set equlizer enable.
     */
    void resetEq();
    /*!
     * Reset equlizer parameters.
     */
    void verticalSliderChanged(int index);
    /*!
     * Current equlizer parameters has changed by changed index.
     */
    void eqChoiceIndexChanged(int index);
    /*!
     * Equlizer presets index changed by change index.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void init();
    /*!
     * Init widget and parameters.
     */
    void initSlider(QSlider *slider, int index);
    /*!
     * Init and create slider.
     */
    void setControlEnable(bool enable) const;
    /*!
     * Enable or disable control state.
     */
    void readEqInformation();
    /*!
     * Read equalizer info from config.
     */
    void writeEqInformation() const;
    /*!
     * Write equalizer info from config.
     */
    void initEqualizeValue();
    /*!
     * Init equalizer parameters value.
     */
    void emitParameter();
    /*!
     * Emit current equalizer parameters value.
     */

    Ui::MusicEqualizerDialog *m_ui;
    bool m_eable;
    bool m_eqChoiceSelected;
    MusicObject::MIntList m_equalizeValue;
    QSignalMapper *m_signalMapper;

};

#endif // MUSICEQUALIZERDIALOG_H
