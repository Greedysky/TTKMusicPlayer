#include "settingsdialog.h"

#include <QSettings>
#include <qmmp/qmmp.h>
#include <qmmp/qmmptextcodec.h>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_WIN
    setFixedHeight(310);
#elif defined Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    m_ui.id3v1EncComboBox->addItems(QmmpTextCodec::availableCharsets());
    m_ui.id3v2EncComboBox->addItems(QmmpTextCodec::availableCharsets());

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MPEG");

    int pos = m_ui.id3v1EncComboBox->findText(settings.value("ID3v1_encoding", "GB18030").toString());
    m_ui.id3v1EncComboBox->setCurrentIndex(pos);
    pos = m_ui.id3v2EncComboBox->findText(settings.value("ID3v2_encoding", "UTF-8").toString());
    m_ui.id3v2EncComboBox->setCurrentIndex(pos);

    m_ui.firstTagComboBox->setCurrentIndex(settings.value("tag_1", ID3v2).toInt());
    m_ui.secondTagComboBox->setCurrentIndex(settings.value("tag_2", APE).toInt());
    m_ui.thirdTagComboBox->setCurrentIndex(settings.value("tag_3", ID3v1).toInt());
    m_ui.mergeTagsCheckBox->setChecked(settings.value("merge_tags", false).toBool());
    m_ui.detectEncodingCheckBox->setChecked(settings.value("detect_encoding", false).toBool());

    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MPEG");
    settings.setValue("ID3v1_encoding", m_ui.id3v1EncComboBox->currentText());
    settings.setValue("ID3v2_encoding", m_ui.id3v2EncComboBox->currentText());
    settings.setValue("tag_1", m_ui.firstTagComboBox->currentIndex());
    settings.setValue("tag_2", m_ui.secondTagComboBox->currentIndex());
    settings.setValue("tag_3", m_ui.thirdTagComboBox->currentIndex());
    settings.setValue("merge_tags", m_ui.mergeTagsCheckBox->isChecked());
    settings.setValue("detect_encoding", m_ui.detectEncodingCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}
