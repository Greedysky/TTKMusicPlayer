/***************************************************************************
 *   Copyright (C) 2015 by Ilya Kotov                                      *
 *   forkotov02@ya.ru                                                      *
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
#include <xmp.h>
#include "decoder_xmp.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    //prepare combobox
    m_ui.srateComboBox->addItem(tr("22050 Hz"), 22050);
    m_ui.srateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.srateComboBox->addItem(tr("48000 Hz"), 48000);
    m_ui.intTypeComboBox->addItem(tr("Nearest neighbor"), XMP_INTERP_NEAREST);
    m_ui.intTypeComboBox->addItem(tr("Linear"), XMP_INTERP_LINEAR);
    m_ui.intTypeComboBox->addItem(tr("Cubic spline"), XMP_INTERP_SPLINE);
    //load settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Xmp");
    m_ui.ampFactorSpinBox->setValue(settings.value("amp_factor", 1).toInt());
    m_ui.stereoMixingSpinBox->setValue(settings.value("stereo_mix", 70).toInt());
    int index = m_ui.intTypeComboBox->findData(settings.value("interpolation", XMP_INTERP_LINEAR).toInt());
    if(index >= 0)
        m_ui.intTypeComboBox->setCurrentIndex(index);
    index = m_ui.srateComboBox->findData(settings.value("sample_rate", 44100).toInt());
    if(index >= 0)
        m_ui.srateComboBox->setCurrentIndex(index);
    m_ui.lowPassCheckBox->setChecked(settings.value("lowpass", false).toBool());
    m_ui.vblankCheckBox->setChecked(settings.value("vblank", false).toBool());
    m_ui.fx9BugCheckBox->setChecked(settings.value("fx9bug", false).toBool());
    settings.endGroup();

    connect(m_ui.buttonBox, SIGNAL(clicked (QAbstractButton *)), SLOT(exec(QAbstractButton *)));
}


SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::writeSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Xmp");
    settings.setValue("amp_factor", m_ui.ampFactorSpinBox->value());
    settings.setValue("stereo_mix", m_ui.stereoMixingSpinBox->value());
    int index = m_ui.intTypeComboBox->currentIndex();
    if(index >= 0)
        settings.setValue("interpolation", m_ui.intTypeComboBox->itemData(index));
    index = m_ui.srateComboBox->currentIndex();
    if(index >= 0)
        settings.setValue("sample_rate", m_ui.srateComboBox->itemData(index));
    settings.setValue("lowpass", m_ui.lowPassCheckBox->isChecked());
    settings.setValue("vblank", m_ui.vblankCheckBox->isChecked());
    settings.setValue("fx9bug", m_ui.fx9BugCheckBox->isChecked());
    settings.endGroup();
    //apply settings for the created decoder
    if (DecoderXmp::instance())
    {
        //DecoderXmp::instance()->mutex()->lock();
        DecoderXmp::instance()->readSettings();
        //DecoderXmp::instance()->mutex()->unlock();
    }
}

void SettingsDialog::exec(QAbstractButton *button)
{
    switch ((int) m_ui.buttonBox->buttonRole(button))
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
