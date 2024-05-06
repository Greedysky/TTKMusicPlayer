#include "settingsdialog.h"
#include "decoder_xmp.h"

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
    m_ui.srateComboBox->addItem(tr("22050 Hz"), 22050);
    m_ui.srateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.srateComboBox->addItem(tr("48000 Hz"), 48000);
    m_ui.intTypeComboBox->addItem(tr("Nearest neighbor"), XMP_INTERP_NEAREST);
    m_ui.intTypeComboBox->addItem(tr("Linear"), XMP_INTERP_LINEAR);
    m_ui.intTypeComboBox->addItem(tr("Cubic spline"), XMP_INTERP_SPLINE);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Xmp");
    m_ui.ampFactorSpinBox->setValue(settings.value("amp_factor", 1).toInt());
    m_ui.stereoMixingSpinBox->setValue(settings.value("stereo_mix", 70).toInt());

    int index = m_ui.intTypeComboBox->findData(settings.value("interpolation", XMP_INTERP_LINEAR).toInt());
    if(index >= 0)
    {
        m_ui.intTypeComboBox->setCurrentIndex(index);
    }

    index = m_ui.srateComboBox->findData(settings.value("sample_rate", 44100).toInt());
    if(index >= 0)
    {
        m_ui.srateComboBox->setCurrentIndex(index);
    }

    m_ui.lowPassCheckBox->setChecked(settings.value("lowpass", false).toBool());
    m_ui.vblankCheckBox->setChecked(settings.value("vblank", false).toBool());
    m_ui.fx9BugCheckBox->setChecked(settings.value("fx9bug", false).toBool());
    m_ui.fixLoopCheckBox->setChecked(settings.value("fixlopp", false).toBool());
    m_ui.a500CheckBox->setChecked(settings.value("a500", false).toBool());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Xmp");
    settings.setValue("amp_factor", m_ui.ampFactorSpinBox->value());
    settings.setValue("stereo_mix", m_ui.stereoMixingSpinBox->value());

    int index = m_ui.intTypeComboBox->currentIndex();
    if(index >= 0)
    {
        settings.setValue("interpolation", m_ui.intTypeComboBox->itemData(index));
    }

    index = m_ui.srateComboBox->currentIndex();
    if(index >= 0)
    {
        settings.setValue("sample_rate", m_ui.srateComboBox->itemData(index));
    }

    settings.setValue("lowpass", m_ui.lowPassCheckBox->isChecked());
    settings.setValue("vblank", m_ui.vblankCheckBox->isChecked());
    settings.setValue("fx9bug", m_ui.fx9BugCheckBox->isChecked());
    settings.setValue("fixlopp", m_ui.fixLoopCheckBox->isChecked());
    settings.setValue("a500", m_ui.a500CheckBox->isChecked());
    settings.endGroup();

    if(DecoderXMP::instance())
    {
        DecoderXMP::instance()->readSettings();
    }
    QDialog::accept();
}
