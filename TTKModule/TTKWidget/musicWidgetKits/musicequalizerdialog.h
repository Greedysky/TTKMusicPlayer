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
    /*!
     * Object contsructor.
     */
    explicit MusicEqualizerDialog(QWidget *parent = 0);

    virtual ~MusicEqualizerDialog();

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Current equlizer parameters changed emit.
     */
    void setEqEffect(const MusicObject::MIntList &effect);
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
    MusicObject::MIntList m_equalizeValue;
    QSignalMapper *m_signalMapper;

};

#endif // MUSICEQUALIZERDIALOG_H
