/***************************************************************************
 *   Copyright (C) 2008 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QSettings>
#include <QDir>

#include <libmodplug/stdafx.h>
#include <libmodplug/it_defs.h>
#include <libmodplug/sndfile.h>

#include "decoder_modplug.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ModPlug");
    //general
    ui.noiseCheckBox->setChecked(settings.value("NoiseReduction", false).toBool());
    ui.fileNameCheckBox->setChecked(settings.value("UseFileName", false).toBool());
    ui.amigaCheckBox->setChecked(settings.value("GrabAmigaMOD", true).toBool());
    //settings.value("Oversampling", true).toBool();
    //settings.value("VolumeRamp", true).toBool();
    //settings.value("FastInfo", true).toBool();
    //channels number
    if (settings.value("Channels", 2).toInt() == 2)
        ui.stereoRadioButton->setChecked(true);
    else
        ui.monoRadioButton->setChecked(true);
    //bits number
    if (settings.value("Bits", 16).toInt() == 8)
        ui.bit8RadioButton->setChecked(true);
    else
        ui.bit16RadioButton->setChecked(true);
    //resampling frequency
    int freq = settings.value("Frequency", 44100).toInt();
    if (freq == 48000)
        ui.khz48RadioButton->setChecked(true);
    else if (freq == 44100)
        ui.khz44RadioButton->setChecked(true);
    else if (freq == 22050)
        ui.khz22RadioButton->setChecked(true);
    else
        ui.khz11RadioButton->setChecked(true);
    //resampling mode
    int res = settings.value("ResamplineMode", SRCMODE_POLYPHASE).toInt();
    if (res == SRCMODE_NEAREST)
        ui.resampNearestRadioButton->setChecked(true);
    else if (res == SRCMODE_LINEAR)
        ui.resampLinearRadioButton->setChecked(true);
    else if (res == SRCMODE_SPLINE)
        ui.resampSplineRadioButton->setChecked(true);
    else
        ui.resampPolyphaseRadioButton->setChecked(true);
    //reverberation
    ui.reverbGroupBox->setChecked(settings.value("Reverb", false).toBool());
    ui.reverbDepthSlider->setValue(settings.value("ReverbDepth", 30).toInt());
    ui.reverbDelaySlider->setValue(settings.value("ReverbDelay", 100).toInt());
    //surround
    ui.surGroupBox->setChecked(settings.value("Surround", true).toBool());
    ui.surDepthSlider->setValue(settings.value("SurroundDepth", 20).toInt());
    ui.surDelaySlider->setValue(settings.value("SurroundDelay", 20).toInt());
    //bass
    ui.bassGroupBox->setChecked(settings.value("Megabass", false).toBool());
    ui.bassAmountSlider->setValue(settings.value("BassAmount", 40).toInt());
    ui.bassRangeSlider->setValue(settings.value("BassRange", 30).toInt());
    //preamp
    ui.preampGroupBox->setChecked(settings.value("PreAmp", false).toBool());
    connect(ui.preampSlider, SIGNAL(valueChanged(int)), SLOT(setPreamp(int)));
    ui.preampSlider->setValue(int(settings.value("PreAmpLevel", 0.0f).toDouble()*10));
    //looping
    int l = settings.value("LoopCount", 0).toInt();
    if (l == 0)
        ui.dontLoopRadioButton->setChecked(true);
    else if (l < 0)
        ui.loopForeverRadioButton->setChecked(true);
    else
    {
        ui.loopRadioButton->setChecked(true);
        ui.loopSpinBox->setValue(l);
    }
    settings.endGroup();
    connect(ui.buttonBox, SIGNAL(clicked (QAbstractButton *)), SLOT(exec(QAbstractButton *)));
}


SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ModPlug");
    //general
    settings.setValue("NoiseReduction", ui.noiseCheckBox->isChecked());
    settings.setValue("UseFileName", ui.fileNameCheckBox->isChecked());
    settings.setValue("GrabAmigaMOD", ui.amigaCheckBox->isChecked());
    //settings.value("Oversampling", true).toBool();
    //settings.value("VolumeRamp", true).toBool();
    //settings.value("FastInfo", true).toBool();
    //channels number
    settings.setValue("Channels", ui.stereoRadioButton->isChecked() ? 2 : 1 );
    //bits number
    settings.setValue("Bits", ui.bit8RadioButton->isChecked() ? 8 : 16 );
    //resampling frequency
    if (ui.khz48RadioButton->isChecked())
        settings.setValue("Frequency", 48000);
    else if (ui.khz44RadioButton->isChecked())
        settings.setValue("Frequency", 44100);
    else if (ui.khz22RadioButton->isChecked())
        settings.setValue("Frequency", 22050);
    else
        settings.setValue("Frequency", 11025);
    //resampling mode
    if (ui.resampNearestRadioButton->isChecked())
        settings.setValue("ResamplineMode", SRCMODE_NEAREST);
    else if (ui.resampLinearRadioButton->isChecked())
        settings.setValue("ResamplineMode", SRCMODE_LINEAR);
    else if (ui.resampSplineRadioButton->isChecked())
        settings.setValue("ResamplineMode", SRCMODE_SPLINE);
    else
        settings.setValue("ResamplineMode", SRCMODE_POLYPHASE);
    //reverberation
    settings.setValue("Reverb",  ui.reverbGroupBox->isChecked());
    settings.setValue("ReverbDepth", ui.reverbDepthSlider->value());
    settings.setValue("ReverbDelay", ui.reverbDelaySlider->value());
    //surround
    settings.setValue("Surround", ui.surGroupBox->isChecked());
    settings.setValue("SurroundDepth", ui.surDepthSlider->value());
    settings.setValue("SurroundDelay", ui.surDelaySlider->value());
    //bass
    settings.setValue("Megabass", ui.bassGroupBox->isChecked());
    settings.setValue("BassAmount", ui.bassAmountSlider->value());
    settings.setValue("BassRange", ui.bassRangeSlider->value());
    //preamp
    settings.setValue("PreAmp", ui.preampGroupBox->isChecked());
    connect(ui.preampSlider, SIGNAL(valueChanged(int)), SLOT(setPreamp(int)));
    settings.setValue("PreAmpLevel", (double) ui.preampSlider->value()/10);
    //looping
    if (ui.dontLoopRadioButton->isChecked())
        settings.setValue("LoopCount", 0);
    else if (ui.loopForeverRadioButton->isChecked())
        settings.setValue("LoopCount", -1);
    else
        settings.setValue("LoopCount", ui.loopSpinBox->value());
    settings.endGroup();
    //apply settings for the created decoder
    if (DecoderModPlug::instance())
    {
        //DecoderModPlug::instance()->mutex()->lock();
        DecoderModPlug::instance()->readSettings();
        //DecoderModPlug::instance()->mutex()->unlock();
    }
}

void SettingsDialog::setPreamp(int preamp)
{
    ui.preampLabel->setText(QString("%1").arg((double) preamp/10));
}

void SettingsDialog::exec(QAbstractButton *button)
{
    switch ((int) ui.buttonBox->buttonRole(button))
    {
    case QDialogButtonBox::AcceptRole:
        writeSettings();
        accept();
        break;
    case QDialogButtonBox::ApplyRole:
        writeSettings();
        break;
    }
}
