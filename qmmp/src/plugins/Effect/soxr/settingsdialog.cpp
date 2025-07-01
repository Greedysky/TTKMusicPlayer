#include "settingsdialog.h"

#include <QSettings>
#include <qmmp/qmmp.h>
#include <QAbstractButton>
#include <soxr.h>

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
    m_ui.srSpinBox->setValue(settings.value("SOXR/sample_rate", 48000).toInt());

    m_ui.qualityComboBox->addItem(tr("Quick"), SOXR_QQ);
    m_ui.qualityComboBox->addItem(tr("Low"), SOXR_LQ);
    m_ui.qualityComboBox->addItem(tr("Medium"), SOXR_MQ);
    m_ui.qualityComboBox->addItem(tr("High"), SOXR_HQ);
    m_ui.qualityComboBox->addItem(tr("Very High"), SOXR_VHQ);

    int index = m_ui.qualityComboBox->findData(settings.value("SOXR/quality", SOXR_HQ).toInt());
    if(index >= 0 && index < m_ui.qualityComboBox->count())
        m_ui.qualityComboBox->setCurrentIndex(index);
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("SOXR/sample_rate", m_ui.srSpinBox->value());
    settings.setValue("SOXR/quality", m_ui.qualityComboBox->itemData(m_ui.qualityComboBox->currentIndex()).toInt());
    QDialog::accept();
}
