/***************************************************************************
 *   Copyright (C) 2010-2025 by Ilya Kotov                                 *
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

extern "C" {
#include <alsa/asoundlib.h> 
}
#include "ui_settingsdialog.h"

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

public slots:
    virtual void accept() override final;

private slots:
    void setText(int);
    void showMixerDevices(int);

private:
    void getCards();
    void getSoftDevices();
    void getCardDevices(int card);
    void getMixerDevices(QString card);
    int getMixer(snd_mixer_t **mixer, QString card);

    Ui::SettingsDialog m_ui;
    QStringList m_devices;
    QList<QString> m_cards;

};

#endif
