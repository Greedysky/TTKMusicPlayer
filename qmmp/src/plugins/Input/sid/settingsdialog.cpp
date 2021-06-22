#include <QSettings>
#include <QFileInfo>
#include <qmmp/qmmp.h>
#include <sidplayfp/SidConfig.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(SidDatabase *db, QWidget *parent)
    : QDialog(parent)
{
    m_db = db;
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");

    m_ui.sampleRateComboBox->addItem(tr("44100 Hz"), 44100);
    m_ui.sampleRateComboBox->addItem(tr("48000 Hz"), 48000);
    int i = m_ui.sampleRateComboBox->findData(settings.value("sample_rate", 44100).toInt());
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

    int i = m_ui.sampleRateComboBox->currentIndex();
    if(i >= 0)
        settings.setValue("sample_rate", m_ui.sampleRateComboBox->itemData(i));
    if((i = m_ui.emuComboBox->currentIndex()) >= 0)
        settings.setValue("engine", m_ui.emuComboBox->itemData(i));
    settings.setValue("fast_resampling", m_ui.fastResampligCheckBox->isChecked());
    if((i = m_ui.resamplingComboBox->currentIndex()) >= 0)
        settings.setValue("resampling_method", m_ui.resamplingComboBox->itemData(i));
    m_db->close();

    settings.endGroup();
    QDialog::accept();
}
