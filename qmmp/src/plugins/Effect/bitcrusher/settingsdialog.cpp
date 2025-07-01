#include "settingsdialog.h"
#include "bitcrusherplugin.h"

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
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ui.depthValue->setValue(settings.value("Bitcrusher/depth", 32).toDouble());
    m_ui.downsampleValue->setValue(settings.value("Bitcrusher/downsample", 1.0).toDouble());
}

void SettingsDialog::accept()
{
    const double depth = m_ui.depthValue->value();
    const double downsample = m_ui.downsampleValue->value();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Bitcrusher/depth", depth);
    settings.setValue("Bitcrusher/downsample", downsample);

    if(BitcrusherPlugin::instance())
    {
        BitcrusherPlugin::instance()->setDepth(depth);
        BitcrusherPlugin::instance()->setDownsample(downsample);
    }
    QDialog::accept();
}
