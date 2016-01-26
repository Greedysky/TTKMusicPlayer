/***************************************************************************
 *   Copyright (C) 2008-2013 by Ilya Kotov                                 *
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

extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#if (LIBAVUTIL_VERSION_INT >= ((51<<16)+(32<<8)+0))
#include <libavutil/dict.h>
#endif
}
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QStringList filters;
    filters << "*.wma";
    filters << "*.ape";
    filters = settings.value("FFMPEG_legacy/filters", filters).toStringList();
    avcodec_register_all();
    av_register_all();
    ui.wmaCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_WMAV1));
    ui.wmaCheckBox->setChecked(filters.contains("*.wma") && avcodec_find_decoder(CODEC_ID_WMAV1));
    ui.apeCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_APE));
    ui.apeCheckBox->setChecked(filters.contains("*.ape") && avcodec_find_decoder(CODEC_ID_APE));
    ui.ttaCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_TTA));
    ui.ttaCheckBox->setChecked(filters.contains("*.tta") && avcodec_find_decoder(CODEC_ID_TTA));
    ui.aacCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_AAC));
    ui.aacCheckBox->setChecked(filters.contains("*.aac") && avcodec_find_decoder(CODEC_ID_AAC));
    ui.mp3CheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_MP3));
    ui.mp3CheckBox->setChecked(filters.contains("*.mp3") && avcodec_find_decoder(CODEC_ID_MP3));
    ui.mp4CheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_AAC));
    ui.mp4CheckBox->setChecked(filters.contains("*.m4a") && (avcodec_find_decoder(CODEC_ID_AAC)
                                                             || avcodec_find_decoder(CODEC_ID_ALAC)));
    ui.raCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_RA_288));
    ui.raCheckBox->setChecked(filters.contains("*.ra") && avcodec_find_decoder(CODEC_ID_RA_288));
    ui.shCheckBox->setChecked(filters.contains("*.shn") && avcodec_find_decoder(CODEC_ID_SHORTEN));
    ui.ac3CheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_EAC3));
    ui.ac3CheckBox->setChecked(filters.contains("*.ac3") && avcodec_find_decoder(CODEC_ID_EAC3));
    ui.dtsCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_DTS));
    ui.dtsCheckBox->setChecked(filters.contains("*.dts") && avcodec_find_decoder(CODEC_ID_DTS));
#if (LIBAVCODEC_VERSION_INT >= ((53<<16)+(42<<8)+4))
    ui.mkaCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_TRUEHD));
    ui.mkaCheckBox->setChecked(filters.contains("*.mka") && avcodec_find_decoder(CODEC_ID_TRUEHD));
    ui.vqfCheckBox->setEnabled(avcodec_find_decoder(CODEC_ID_TWINVQ));
    ui.vqfCheckBox->setChecked(filters.contains("*.vqf") && avcodec_find_decoder(CODEC_ID_TWINVQ));
#else
    ui.mkaCheckBox->hide();
    ui.vqfCheckBox->hide();
#endif
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::accept()
{
    QStringList filters;
    if (ui.mp3CheckBox->isChecked())
        filters << "*.mp3";
    if (ui.wmaCheckBox->isChecked())
        filters << "*.wma";
    if (ui.apeCheckBox->isChecked())
        filters << "*.ape";
    if (ui.ttaCheckBox->isChecked())
        filters << "*.tta";
    if (ui.aacCheckBox->isChecked())
        filters << "*.aac";
    if (ui.mp4CheckBox->isChecked())
        filters << "*.m4a";
    if (ui.raCheckBox->isChecked())
        filters << "*.ra";
    if (ui.shCheckBox->isChecked())
        filters << "*.shn";
    if (ui.ac3CheckBox->isChecked())
        filters << "*.ac3";
    if (ui.dtsCheckBox->isChecked())
        filters << "*.dts";
    if (ui.mkaCheckBox->isChecked())
        filters << "*.mka";
    if (ui.vqfCheckBox->isChecked())
        filters << "*.vqf";
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("FFMPEG_legacy/filters", filters);
    QDialog::accept();
}
