#include "settingsdialog.h"

#include <QSettings>
#include <QAbstractButton>
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
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.srSpinBox->setValue(settings.value("SRC/sample_rate", 48000).toInt());
    m_ui.engineComboBox->setCurrentIndex(settings.value("SRC/engine", 0).toInt());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("SRC/sample_rate", m_ui.srSpinBox->value());
    settings.setValue("SRC/engine", m_ui.engineComboBox->currentIndex());
    QDialog::accept();
}
