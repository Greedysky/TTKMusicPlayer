/***************************************************************************
 *   Copyright (C) 2010-2013 by Ilya Kotov                                 *
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
#include <QStringList>
#include <qmmp/qmmp.h>
#include "wildmidihelper.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    QStringList files = WildMidiHelper::instance()->configFiles();
    QString conf_path = files.isEmpty() ? QString() : files.first();
    m_ui.confPathComboBox->addItems(files);
    m_ui.confPathComboBox->setEditText(settings.value("conf_path", conf_path).toString());
    m_ui.sampleRateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.sampleRateComboBox->addItem(tr("48000 Hz"), 48000);
    int i = m_ui.sampleRateComboBox->findData(settings.value("sample_rate", 44100).toInt());
    m_ui.sampleRateComboBox->setCurrentIndex(i);
    m_ui.enhancedResamplingCheckBox->setChecked(settings.value("enhanced_resampling", false).toBool());
    m_ui.reverbCheckBox->setChecked(settings.value("reverberation", false).toBool());
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("conf_path", m_ui.confPathComboBox->currentText());
    settings.setValue("sample_rate",
                      m_ui.sampleRateComboBox->itemData(m_ui.sampleRateComboBox->currentIndex()));
    settings.setValue("enhanced_resampling", m_ui.enhancedResamplingCheckBox->isChecked());
    settings.setValue("reverberation", m_ui.reverbCheckBox->isChecked());
    settings.endGroup();
    WildMidiHelper::instance()->readSettings();
    QDialog::accept();
}
