/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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
#include <QFileInfo>
#include <qmmp/qmmp.h>
#include <sidplayfp/SidConfig.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(SidDatabase *db, QWidget *parent) : QDialog(parent)
{
    m_db = db;
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");

    m_ui.useHVSCCheckBox->setChecked(settings.value("use_hvsc", false).toBool());
    QString hvsc_default_path = Qmmp::configDir() + "/Songlengths.txt";
    m_ui.hvscPathLineEdit->setText(settings.value("hvsc_path", hvsc_default_path).toString());
    m_ui.defaultLengthSpinBox->setValue(settings.value("song_length", 180).toInt());

    m_ui.sampleRateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.sampleRateComboBox->addItem(tr("48000 Hz"), 48000);
    int i = m_ui.sampleRateComboBox->findData(settings.value("sample_rate", 44100).toInt());
    m_ui.sampleRateComboBox->setCurrentIndex(i);

    m_ui.emuComboBox->addItem("ReSID", "resid");
    m_ui.emuComboBox->addItem("ReSIDfp", "residfp");
    i = m_ui.emuComboBox->findData(settings.value("engine", "residfp").toString());
    m_ui.emuComboBox->setCurrentIndex(i);

    m_ui.fastResampligCheckBox->setChecked(settings.value("fast_resampling", false).toBool());

    m_ui.resamplingComboBox->addItem("Interpolate", SidConfig::INTERPOLATE);
    m_ui.resamplingComboBox->addItem("Resample interpolate", SidConfig::RESAMPLE_INTERPOLATE);
    i = m_ui.resamplingComboBox->findData(settings.value("resampling_method", SidConfig::INTERPOLATE).toInt());
    m_ui.resamplingComboBox->setCurrentIndex(i);

    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");
    settings.setValue("use_hvsc", m_ui.useHVSCCheckBox->isChecked());
    settings.setValue("hvsc_path", m_ui.hvscPathLineEdit->text());
    settings.setValue("song_length", m_ui.defaultLengthSpinBox->value());
    int i = m_ui.sampleRateComboBox->currentIndex();
    if(i >= 0)
        settings.setValue("sample_rate", m_ui.sampleRateComboBox->itemData(i));
    if((i = m_ui.emuComboBox->currentIndex()) >= 0)
        settings.setValue("engine", m_ui.emuComboBox->itemData(i));
    settings.setValue("fast_resampling", m_ui.fastResampligCheckBox->isChecked());
    if((i = m_ui.resamplingComboBox->currentIndex()) >= 0)
        settings.setValue("resampling_method", m_ui.resamplingComboBox->itemData(i));
    m_db->close();
    if(m_ui.useHVSCCheckBox->isChecked())
    {
        if(!m_db->open(qPrintable(m_ui.hvscPathLineEdit->text())))
            qWarning("SettingsDialog: %s", m_db->error());
    }
    settings.endGroup();
    QDialog::accept();
}
