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
#include <qmmp/qmmp.h>
#include "stereoplugin.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_level = settings.value("extra_stereo/intensity", 1.0).toDouble();
    ui.intensitySlider->setValue(m_level * 100 / 10.0);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("extra_stereo/intensity", ui.intensitySlider->value() * 10.0 / 100);
    QDialog::accept();
}

void SettingsDialog::SettingsDialog::reject()
{
    if (StereoPlugin::instance()) //restore settings
        StereoPlugin::instance()->setIntensity(m_level);
    QDialog::reject();
}

void SettingsDialog::on_intensitySlider_valueChanged (int value)
{
    double level = value * 10.0 / 100;
    ui.intensityLabel->setText(QString(tr("%1")).arg(level));
    if (StereoPlugin::instance())
        StereoPlugin::instance()->setIntensity(level);
}
