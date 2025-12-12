#include "settingsdialog.h"
#include "decoder_cdaudio.h"

#include <QDir>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_WIN
    setFixedHeight(300);
#elif defined Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CDAudio");
    const int speed = settings.value("speed", 0).toInt();
    m_ui.deviceLineEdit->setText(settings.value("device").toString());
    m_ui.deviceCheckBox->setChecked(!m_ui.deviceLineEdit->text().isEmpty());
    m_ui.speedCheckBox->setChecked(speed > 0);
    m_ui.speedSpinBox->setValue(speed);
    m_ui.cdtextCheckBox->setChecked(settings.value("cdtext", true).toBool());
#ifdef WITH_LIBCDDB
    m_ui.cddbGroupBox->setChecked(settings.value("use_cddb", false).toBool());
    m_ui.httpCheckBox->setChecked(settings.value("cddb_http", false).toBool());
    m_ui.serverLineEdit->setText(settings.value("cddb_server", "gnudb.org").toString());
    m_ui.pathLineEdit->setText(settings.value("cddb_path").toString());
    m_ui.portLineEdit->setText(settings.value("cddb_port", 8880).toString());
#else
    m_ui->cddbGroupBox->setVisible(false);
#endif
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CDAudio");
    if(m_ui.deviceCheckBox->isChecked())
    {
        settings.setValue("device", m_ui.deviceLineEdit->text());
    }
    else
    {
        settings.remove("device");
    }
    
    if(m_ui.speedCheckBox->isChecked())
    {
        settings.setValue("speed", m_ui.speedSpinBox->value());
    }
    else
    {
        settings.setValue("speed", 0);
    }
    
    settings.setValue("cdtext", m_ui.cdtextCheckBox->isChecked());
    settings.setValue("cdtext", m_ui.cdtextCheckBox->isChecked());
#ifdef WITH_LIBCDDB
    settings.setValue("use_cddb", m_ui.cddbGroupBox->isChecked());
    settings.setValue("cddb_http", m_ui.httpCheckBox->isChecked());
    settings.setValue("cddb_server",  m_ui.serverLineEdit->text());
    settings.setValue("cddb_path", m_ui.pathLineEdit->text());
    settings.setValue("cddb_port", m_ui.portLineEdit->text());
#endif
    settings.endGroup();
    settings.sync();

    DecoderCDAudio::clearTrackCache();
    QDialog::accept();
}

void SettingsDialog::on_clearCacheButton_clicked()
{
    QDir dir(Qmmp::cacheDir());
    dir.cd("cddbcache");
    
    const QStringList &list = dir.entryList({"*"}, QDir::Files);
    for(const QString &name : qAsConst(list))
    {
        dir.remove(name);
    }
}
