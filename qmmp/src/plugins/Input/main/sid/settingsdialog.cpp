#include "settingsdialog.h"

#include <QSettings>
#include <qmmp/qmmp.h>
#include <sidplayfp/SidConfig.h>

SettingsDialog::SettingsDialog(SidDatabase *db, QWidget *parent)
    : QDialog(parent)
{
    m_db = db;
    m_ui.setupUi(this);
#ifdef Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");

    m_ui.useHVSCCheckBox->setChecked(settings.value("use_hvsc", false).toBool());
    m_ui.hvscPathLineEdit->setText(settings.value("hvsc_path", HVSC_PATH).toString());
    m_ui.useLengthCheckBox->setChecked(settings.value("use_length", false).toBool());
    m_ui.defaultLengthSpinBox->setValue(settings.value("song_length", 180).toInt());

    m_ui.sampleRateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.sampleRateComboBox->addItem(tr("48000 Hz"), 48000);
    int i = m_ui.sampleRateComboBox->findData(settings.value("sample_rate", 48000).toInt());
    m_ui.sampleRateComboBox->setCurrentIndex(i);

    m_ui.emuComboBox->addItem("ReSID", "resid");
    m_ui.emuComboBox->addItem("ReSIDfp", "residfp");
    i = m_ui.emuComboBox->findData(settings.value("engine", "residfp").toString());
    m_ui.emuComboBox->setCurrentIndex(i);

    m_ui.fastResampligCheckBox->setChecked(settings.value("fast_resampling", false).toBool());

    m_ui.resamplingComboBox->addItem("Interpolate", SidConfig::INTERPOLATE);
    m_ui.resamplingComboBox->addItem("Resample interpolate", SidConfig::RESAMPLE_INTERPOLATE);
    i = m_ui.resamplingComboBox->findData(settings.value("resampling_method", SidConfig::INTERPOLATE).toInt());
    m_ui.resamplingComboBox->setCurrentIndex(i);

    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");

    settings.setValue("use_hvsc", m_ui.useHVSCCheckBox->isChecked());
    settings.setValue("hvsc_path", m_ui.hvscPathLineEdit->text());
    settings.setValue("use_length", m_ui.useLengthCheckBox->isChecked());
    settings.setValue("song_length", m_ui.defaultLengthSpinBox->value());

    int i = m_ui.sampleRateComboBox->currentIndex();
    if(i >= 0)
        settings.setValue("sample_rate", m_ui.sampleRateComboBox->itemData(i));
    if((i = m_ui.emuComboBox->currentIndex()) >= 0)
        settings.setValue("engine", m_ui.emuComboBox->itemData(i));
    settings.setValue("fast_resampling", m_ui.fastResampligCheckBox->isChecked());
    if((i = m_ui.resamplingComboBox->currentIndex()) >= 0)
        settings.setValue("resampling_method", m_ui.resamplingComboBox->itemData(i));
    m_db->close();

    if(m_ui.useHVSCCheckBox->isChecked() && !m_db->open(qPrintable(m_ui.hvscPathLineEdit->text())))
    {
        qWarning("SettingsDialog: %s", m_db->error());
    }

    settings.endGroup();
    QDialog::accept();
}
