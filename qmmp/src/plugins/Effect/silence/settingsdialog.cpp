#include "settingsdialog.h"
#include "silenceplugin.h"

#include <QSettings>
#include <QAbstractButton>

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
    m_ui.value->setValue(settings.value("Silence/threshold", -40).toDouble());
}

void SettingsDialog::accept()
{
    const double value = m_ui.value->value();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Silence/threshold", value);

    if(SilencePlugin::instance())
    {
        SilencePlugin::instance()->setThreshold(value);
    }
    QDialog::accept();
}
