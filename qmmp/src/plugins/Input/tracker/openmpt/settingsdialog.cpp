#include "settingsdialog.h"
#include "decoder_openmpt.h"

#include <QSettings>

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
    const QMap<QString, int> interpolators(OpenMPTHelper::interpolators());
    for(auto itr = interpolators.begin(); itr != interpolators.end(); ++itr)
    {
        m_ui.interpolate_combo->addItem(itr.key(), itr.value());
    }

    restoreDefaults();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OpenMPT");
    settings.setValue("interpolator", m_ui.interpolate_combo->itemData(m_ui.interpolate_combo->currentIndex()).toInt());
    settings.setValue("stereo_separation", m_ui.stereo_separation->value());
    settings.setValue("use_file_name", m_ui.use_filename->isChecked());
    settings.endGroup();

    if(DecoderOpenMPT::instance())
    {
        DecoderOpenMPT::instance()->readSettings();
    }
    QDialog::accept();
}

void SettingsDialog::restoreDefaults()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OpenMPT");
    setInterpolator(settings.value("interpolator", INTERP_WINDOWED).toInt());
    m_ui.stereo_separation->setSliderPosition(settings.value("stereo_separation", 100).toInt());
    m_ui.use_filename->setChecked(settings.value("use_file_name", 0).toBool());
    settings.endGroup();
}

void SettingsDialog::setInterpolator(int interpolator)
{
    const int i = m_ui.interpolate_combo->findData(interpolator);
    if(i != -1)
    {
        m_ui.interpolate_combo->setCurrentIndex(i);
    }
}
