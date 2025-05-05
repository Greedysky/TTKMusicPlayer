#include "settingsdialog.h"
#include "ladspaslider.h"
#include "ladspabutton.h"
#include "ladspahost.h"

#include <QStandardItemModel>
#include <QFormLayout>
#include <QLabel>
#include <qmmp/qmmp.h>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    m_ui.loadButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    m_ui.unloadButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));
    m_ui.configureButton->setIcon(QIcon::fromTheme("configure"));

    m_model = new QStandardItemModel(0, 2, this);
    m_model->setHeaderData(0, Qt::Horizontal, tr("UID"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    m_ui.pluginsTreeView->setModel(m_model);

    if(!LADSPAHost::instance())
    {
        new LADSPAHost(qApp);
    }

    QList<LADSPAPlugin*> plugin_list = LADSPAHost::instance()->plugins();

    for(int i = 0; i < plugin_list.count(); ++i)
    {
        m_model->insertRow(i);
        m_model->setData(m_model->index(i, 0), (uint) plugin_list[i]->unique_id);
        m_model->setData(m_model->index(i, 1), plugin_list[i]->name);
    }
    m_ui.pluginsTreeView->resizeColumnToContents(0);
    m_ui.pluginsTreeView->resizeColumnToContents(1);
    updateRunningPlugins();
}

void SettingsDialog::on_loadButton_clicked()
{
    LADSPAHost *l = LADSPAHost::instance();
    QModelIndex index = m_ui.pluginsTreeView->currentIndex();
    if(index.isValid())
    {
        l->load(l->plugins().at(index.row()));
        updateRunningPlugins();
    }
}

void SettingsDialog::on_unloadButton_clicked()
{
    LADSPAHost *l = LADSPAHost::instance();
    QModelIndex index = m_ui.runningListWidget->currentIndex();
    if(index.isValid())
    {
        l->unload(l->effects().at(index.row()));
        updateRunningPlugins();
    }
}

void SettingsDialog::on_configureButton_clicked()
{
    LADSPAHost *l = LADSPAHost::instance();
    QModelIndex index = m_ui.runningListWidget->currentIndex();
    if(!index.isValid())
        return;

    LADSPAEffect *effect = l->effects().at(index.row());
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(effect->plugin->desc->Name);
    QFormLayout *formLayout = new QFormLayout(dialog);
    LADSPAButton *button = nullptr;
    LADSPASlider *slider = nullptr;
    QLabel *label = nullptr;

    for(LADSPAControl *c : qAsConst(effect->controls))
    {
        switch((int) c->type)
        {
        case LADSPAControl::Button:
            button = new LADSPAButton(&c->value, dialog);
            button->setText(c->name);
            formLayout->addRow(button);
            break;
        case LADSPAControl::Slider:
            slider = new LADSPASlider(c->min, c->max, c->step, &c->value, dialog);
            formLayout->addRow(c->name, slider);
            break;
        case LADSPAControl::Label:
            label = new QLabel(this);
            label->setText(QString("%1").arg(c->value));
            label->setFrameStyle(QFrame::StyledPanel);
            label->setFrameShadow(QFrame::Sunken);
            formLayout->addRow(c->name, label);
        }
    }

    if(effect->controls.isEmpty())
    {
        label = new QLabel(tr("This LADSPA plugin has no user controls"), dialog);
        formLayout->addRow(label);
    }

    dialog->setLayout(formLayout);
    dialog->setFixedSize(dialog->sizeHint());
    dialog->exec();
    dialog->deleteLater();
}

void SettingsDialog::updateRunningPlugins()
{
    m_ui.runningListWidget->clear();
    for(LADSPAEffect *e : qAsConst(LADSPAHost::instance()->effects()))
        m_ui.runningListWidget->addItem(e->plugin->desc->Name);
}
