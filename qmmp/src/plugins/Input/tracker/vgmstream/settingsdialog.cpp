#include "settingsdialog.h"

#include <QSettings>
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

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.loopCount->setValue(settings.value("Vgmstream/loop_count", 2).toInt());
    m_ui.fadeLength->setValue(settings.value("Vgmstream/fade_time", 10.0).toDouble());
    m_ui.fadeDelay->setValue(settings.value("Vgmstream/fade_delay", 10.0).toDouble());
    m_ui.forceLoop->setChecked(settings.value("Vgmstream/force_loop", false).toBool());
    m_ui.loopForever->setChecked(settings.value("Vgmstream/loop_forever", false).toBool());
    m_ui.tagfile->setChecked(settings.value("Vgmstream/use_tagfile", true).toBool());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Vgmstream/loop_count", m_ui.loopCount->value());
    settings.setValue("Vgmstream/fade_time", m_ui.fadeLength->value());
    settings.setValue("Vgmstream/fade_delay", m_ui.fadeDelay->value());
    settings.setValue("Vgmstream/force_loop", m_ui.forceLoop->isChecked());
    settings.setValue("Vgmstream/loop_forever", m_ui.loopForever->isChecked());
    settings.setValue("Vgmstream/use_tagfile", m_ui.tagfile->isChecked());
    QDialog::accept();
}
