#include <QTextCodec>
#include <QSettings>
#include <qmmp/qmmp.h>
#include <qmmp/regularwrapper.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(bool using_rusxmms, QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

#ifndef Q_OS_LINUX
    setMinimumHeight(320);
    setMaximumHeight(320);
#endif

    findCodecs();
    for(const QTextCodec *codec : qAsConst(m_codecs))
    {
        m_ui.id3v1EncComboBox->addItem(codec->name());
        m_ui.id3v2EncComboBox->addItem(codec->name());
    }

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MPEG");

    m_ui.enableCrcCheckBox->setChecked(settings.value("enable_crc", false).toBool());

    int pos = m_ui.id3v1EncComboBox->findText(settings.value("ID3v1_encoding", "ISO-8859-1").toString());
    m_ui.id3v1EncComboBox->setCurrentIndex(pos);
    pos = m_ui.id3v2EncComboBox->findText(settings.value("ID3v2_encoding", "UTF-8").toString());
    m_ui.id3v2EncComboBox->setCurrentIndex(pos);

    m_ui.firstTagComboBox->setCurrentIndex(settings.value("tag_1", ID3v2).toInt());
    m_ui.secondTagComboBox->setCurrentIndex(settings.value("tag_2", APE).toInt());
    m_ui.thirdTagComboBox->setCurrentIndex(settings.value("tag_3", ID3v1).toInt());
    m_ui.mergeTagsCheckBox->setChecked(settings.value("merge_tags", false).toBool());

    settings.endGroup();

    if(using_rusxmms)
    {
        m_ui.id3v1EncComboBox->setEnabled(false);
        m_ui.id3v2EncComboBox->setEnabled(false);
    }
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MPEG");
    settings.setValue("enable_crc", m_ui.enableCrcCheckBox->isChecked());
    settings.setValue("ID3v1_encoding", m_ui.id3v1EncComboBox->currentText());
    settings.setValue("ID3v2_encoding", m_ui.id3v2EncComboBox->currentText());
    settings.setValue("tag_1", m_ui.firstTagComboBox->currentIndex());
    settings.setValue("tag_2", m_ui.secondTagComboBox->currentIndex());
    settings.setValue("tag_3", m_ui.thirdTagComboBox->currentIndex());
    settings.setValue("merge_tags", m_ui.mergeTagsCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::findCodecs()
{
    QMap<QString, QTextCodec *> codecMap;
    RegularWrapper iso8859RegExp("ISO[- ]8859-([0-9]+).*");

    for(const int mib : QTextCodec::availableMibs())
    {
        QTextCodec *codec = QTextCodec::codecForMib(mib);

        QString sortKey = codec->name().toUpper();
        int rank;

        if(sortKey.startsWith("UTF-8"))
        {
            rank = 1;
        }
        else if(sortKey.startsWith("UTF-16"))
        {
            rank = 2;
        }
        else if(iso8859RegExp.hasMatch(sortKey))
        {
            if(iso8859RegExp.value(1).size() == 1)
                rank = 3;
            else
                rank = 4;
        }
        else
        {
            rank = 5;
        }
        sortKey.prepend(QChar('0' + rank));

        codecMap.insert(sortKey, codec);
    }

    m_codecs = codecMap.values();
}
