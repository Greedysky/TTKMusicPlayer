/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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
#include <QTextCodec>
#include <QSettings>
#include <qmmp/qmmp.h>

#ifdef WITH_ENCA
#include <enca.h>
#endif

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    findCodecs();
    foreach (QTextCodec *codec, codecs)
        ui.cueEncComboBox->addItem(codec->name());

#ifdef WITH_ENCA
    size_t n = 0;
    const char **langs = enca_get_languages(&n);
    for (size_t i = 0; i < n; ++i)
        ui.encaAnalyserComboBox->addItem(langs[i]);
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CUE");
    int pos = ui.cueEncComboBox->findText(settings.value("encoding","UTF-8").toString());
    ui.cueEncComboBox->setCurrentIndex(pos);
#ifdef WITH_ENCA
    ui.autoCharsetCheckBox->setChecked(settings.value("use_enca", false).toBool());
    pos = ui.encaAnalyserComboBox->findText(settings.value("enca_lang", langs[n-1]).toString());
    ui.encaAnalyserComboBox->setCurrentIndex(pos);
#else
    ui.autoCharsetCheckBox->setEnabled(false);
#endif
    ui.dirtyCueCheckBox->setChecked(settings.value("dirty_cue", false).toBool());
    settings.endGroup();
}

SettingsDialog::~SettingsDialog()
{}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CUE");
    settings.setValue("encoding", ui.cueEncComboBox->currentText());
#ifdef WITH_ENCA
    settings.setValue("use_enca", ui.autoCharsetCheckBox->isChecked());
    settings.setValue("enca_lang", ui.encaAnalyserComboBox->currentText());
#endif
    settings.setValue("dirty_cue", ui.dirtyCueCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::findCodecs()
{
    QMap<QString, QTextCodec *> codecMap;
    QRegExp iso8859RegExp("ISO[- ]8859-([0-9]+).*");

    foreach (int mib, QTextCodec::availableMibs())
    {
        QTextCodec *codec = QTextCodec::codecForMib(mib);

        QString sortKey = codec->name().toUpper();
        int rank;

        if (sortKey.startsWith("UTF-8"))
        {
            rank = 1;
        }
        else if (sortKey.startsWith("UTF-16"))
        {
            rank = 2;
        }
        else if (iso8859RegExp.exactMatch(sortKey))
        {
            if (iso8859RegExp.cap(1).size() == 1)
                rank = 3;
            else
                rank = 4;
        }
        else
        {
            rank = 5;
        }
        sortKey.prepend(QChar('0' + rank));

        codecMap.insert(sortKey, codec);
    }
    codecs = codecMap.values();
}
