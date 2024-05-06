#include "settingsdialog.h"
#include "archivereader.h"

#include <QSettings>
#include <qmmp/qmmp.h>

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
    settings.beginGroup("Archive");
    m_ui.cachePath->setText(settings.value("unpack_path").toString());
    m_ui.cacheSize->setValue(settings.value("max_size", 64).toInt());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Archive");
    settings.setValue("unpack_path", m_ui.cachePath->text());
    settings.setValue("max_size", m_ui.cacheSize->value());
    settings.endGroup();
    QDialog::accept();
}

void SettingsDialog::on_button_clicked()
{
    ArchiveReader::removeRecursively(m_ui.cachePath->text());
}
