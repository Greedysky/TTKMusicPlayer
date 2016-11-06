/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
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
#include <bs2b/bs2b.h>
#include <qmmp/qmmp.h>
#include "bs2bplugin.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui.feedSlider->setRange(BS2B_MINFEED, BS2B_MAXFEED);
    ui.freqSlider->setRange(BS2B_MINFCUT, BS2B_MAXFCUT);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_level = settings.value("bs2b/level", BS2B_DEFAULT_CLEVEL).toUInt();
    ui.feedSlider->setValue(m_level >> 16);
    ui.freqSlider->setValue(m_level & 0xffff);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("bs2b/level", ui.feedSlider->value() << 16 | ui.freqSlider->value());
    QDialog::accept();
}

void SettingsDialog::SettingsDialog::reject()
{
    if (Bs2bPlugin::instance()) //restore crossfeed settings
        Bs2bPlugin::instance()->setCrossfeedLevel(m_level);
    QDialog::reject();
}

void SettingsDialog::on_freqSlider_valueChanged (int value)
{
    ui.freqLabel->setText(QString(tr("%1 Hz, %2 us")).arg(value).arg(bs2b_level_delay(value)));
    if (Bs2bPlugin::instance())
        Bs2bPlugin::instance()->setCrossfeedLevel(ui.feedSlider->value() << 16 | ui.freqSlider->value());
}

void SettingsDialog::on_feedSlider_valueChanged (int value)
{
    ui.feedLabel->setText(QString(tr("%1 dB")).arg((double)value/10));
    if (Bs2bPlugin::instance())
        Bs2bPlugin::instance()->setCrossfeedLevel(ui.feedSlider->value() << 16 | ui.freqSlider->value());
}

void SettingsDialog::on_defaultButton_pressed()
{
    ui.feedSlider->setValue(BS2B_DEFAULT_CLEVEL >> 16);
    ui.freqSlider->setValue(BS2B_DEFAULT_CLEVEL & 0xffff);
}

void SettingsDialog::on_cmButton_pressed ()
{
    ui.feedSlider->setValue(BS2B_CMOY_CLEVEL >> 16);
    ui.freqSlider->setValue(BS2B_CMOY_CLEVEL & 0xffff);
}

void SettingsDialog::on_jmButton_pressed ()
{
    ui.feedSlider->setValue(BS2B_JMEIER_CLEVEL >> 16);
    ui.freqSlider->setValue(BS2B_JMEIER_CLEVEL & 0xffff);
}
