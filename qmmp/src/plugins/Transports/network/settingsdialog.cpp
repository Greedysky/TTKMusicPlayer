#include "settingsdialog.h"

#include <QSettings>
#include <QAbstractButton>
#include <qmmp/qmmp.h>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_OS_WIN
    setFixedHeight(75);
#elif defined Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Network");
    m_ui.bufferSizeSpinBox->setValue(settings.value("buffer_size", 256).toInt());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Network");
    settings.setValue("buffer_size", m_ui.bufferSizeSpinBox->value());
    settings.endGroup();
    QDialog::accept();
}

