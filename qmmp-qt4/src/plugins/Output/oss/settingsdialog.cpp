/***************************************************************************
 *   Copyright (C) 2007 by Zhuravlev Uriy                                  *
 *   stalkerg@gmail.com                                                    *
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
#include <qmmp/qmmp.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog ( QWidget *parent )
        : QDialog ( parent )
{
    m_ui.setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OSS");
    m_ui.lineEdit->insert(settings.value("device","/dev/dsp").toString());
    m_ui.lineEdit_2->insert(settings.value("mixer_device","/dev/mixer").toString());
    m_ui.bufferSpinBox->setValue(settings.value("buffer_time",500).toInt());
    m_ui.periodSpinBox->setValue(settings.value("period_time",100).toInt());

    settings.endGroup();
}


SettingsDialog::~SettingsDialog()
{}



void SettingsDialog::accept()
{
    qDebug("SettingsDialog (OSS):: writeSettings()");
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OSS");
    settings.setValue("device", m_ui.lineEdit->text());
    settings.setValue("buffer_time",m_ui.bufferSpinBox->value());
    settings.setValue("period_time",m_ui.periodSpinBox->value());
    settings.setValue("mixer_device", m_ui.lineEdit_2->text());
    settings.endGroup();
    QDialog::accept();
}
