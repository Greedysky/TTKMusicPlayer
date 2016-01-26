/***************************************************************************
 *   Copyright (C) 2009-2014 by Ilya Kotov                                 *
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
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    m_ui.videoComboBox->addItem(tr("default"));
    m_ui.videoComboBox->addItem("xv");
    m_ui.videoComboBox->addItem("x11");
    m_ui.videoComboBox->addItem("gl");
    m_ui.videoComboBox->addItem("gl2");
    m_ui.videoComboBox->addItem("dga");
    m_ui.videoComboBox->addItem("sdl");
    m_ui.videoComboBox->addItem("null");
    m_ui.audioComboBox->addItem(tr("default"));
    m_ui.audioComboBox->addItem("oss");
    m_ui.audioComboBox->addItem("alsa");
    m_ui.audioComboBox->addItem("pulse");
    m_ui.audioComboBox->addItem("jack");
    m_ui.audioComboBox->addItem("nas");
    m_ui.audioComboBox->addItem("null");
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("mplayer");
    m_ui.audioComboBox->setEditText(settings.value("ao","default").toString().replace("default", tr("default")));
    m_ui.videoComboBox->setEditText(settings.value("vo","default").toString().replace("default", tr("default")));
    m_ui.autoSyncCheckBox->setChecked(settings.value("autosync", false).toBool());
    m_ui.syncFactorSpinBox->setValue(settings.value("autosync_factor", 100).toInt());
    m_ui.cmdOptionsLineEdit->setText(settings.value("cmd_options").toString());
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("mplayer");
    settings.setValue("ao",m_ui.audioComboBox->currentText().replace(tr("default"), "default"));
    settings.setValue("vo",m_ui.videoComboBox->currentText().replace(tr("default"), "default"));
    settings.setValue("autosync",m_ui.autoSyncCheckBox->isChecked());
    settings.setValue("autosync_factor",m_ui.syncFactorSpinBox->value());
    settings.setValue("cmd_options",m_ui.cmdOptionsLineEdit->text());
    settings.endGroup();
    QDialog::accept();
}
