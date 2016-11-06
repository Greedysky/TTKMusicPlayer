/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
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
#include <QApplication>
#include <QStyle>
#include <QStandardItemModel>
#include <QFormLayout>
#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <qmmp/qmmp.h>
#include "ui_settingsdialog.h"
#include "ladspaslider.h"
#include "ladspabutton.h"
#include "ladspahost.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
        : QDialog(parent)
{
    m_ui = new Ui::SettingsDialog;
    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_ui->loadButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    m_ui->unloadButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));
    m_ui->configureButton->setIcon(QIcon::fromTheme("configure"));

    m_model = new QStandardItemModel(0, 2, this);
    m_model->setHeaderData(0, Qt::Horizontal, tr("UID"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_ui->pluginsTreeView->setModel(m_model);

    if(!LADSPAHost::instance())
        new LADSPAHost(qApp);

    QList <LADSPAPlugin *> plugin_list = LADSPAHost::instance()->plugins();

    for(int i = 0; i < plugin_list.size(); ++i)
    {
        m_model->insertRow(i);
        m_model->setData(m_model->index(i, 0), (uint) plugin_list[i]->unique_id);
        m_model->setData(m_model->index(i, 1), plugin_list[i]->name);
    }
    m_ui->pluginsTreeView->resizeColumnToContents (0);
    m_ui->pluginsTreeView->resizeColumnToContents (1);
    updateRunningPlugins();
}

SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

void SettingsDialog::on_loadButton_clicked()
{
    LADSPAHost *l = LADSPAHost::instance();
    QModelIndex index = m_ui->pluginsTreeView->currentIndex ();
    if(index.isValid())
    {
        l->load(l->plugins().at(index.row()));
        updateRunningPlugins();
    }
}

void SettingsDialog::on_unloadButton_clicked()
{
    LADSPAHost *l = LADSPAHost::instance();
    QModelIndex index = m_ui->runningListWidget->currentIndex ();
    if(index.isValid())
    {
        l->unload(l->effects().at(index.row()));
        updateRunningPlugins();
    }
}

void SettingsDialog::on_configureButton_clicked()
{
    LADSPAHost *l = LADSPAHost::instance();
    QModelIndex index = m_ui->runningListWidget->currentIndex ();
    if(!index.isValid())
        return;

    LADSPAEffect *effect = l->effects().at(index.row());
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(effect->plugin->desc->Name);
    QFormLayout *formLayout = new QFormLayout(dialog);
    LADSPAButton *button = 0;
    LADSPASlider *slider = 0;
    QLabel *label = 0;

    foreach(LADSPAControl *c, effect->controls)
    {
        switch ((int) c->type)
        {
        case LADSPAControl::BUTTON:
            button = new LADSPAButton(&c->value, dialog);
            button->setText(c->name);
            formLayout->addRow(button);
            break;
        case LADSPAControl::SLIDER:
            slider = new LADSPASlider(c->min, c->max, c->step, &c->value, dialog);
            formLayout->addRow(c->name, slider);
            break;
        case LADSPAControl::LABEL:
            label = new QLabel(this);
            label->setText(QString("%1").arg(c->value));
            label->setFrameStyle(QFrame::StyledPanel);
            label->setFrameShadow(QFrame::Sunken);
            formLayout->addRow(c->name, label);
        }
    }
    if (effect->controls.isEmpty())
    {
        QLabel *label = new QLabel(tr("This LADSPA plugin has no user controls"), dialog);
        formLayout->addRow(label);
    }
    dialog->setLayout(formLayout);
    dialog->setFixedSize(dialog->sizeHint());
    dialog->exec();
    dialog->deleteLater();
}

void SettingsDialog::accept()
{
    QDialog::accept();
}

void SettingsDialog::updateRunningPlugins()
{
    m_ui->runningListWidget->clear();
    foreach(LADSPAEffect *e, LADSPAHost::instance()->effects())
        m_ui->runningListWidget->addItem(e->plugin->desc->Name);
}
