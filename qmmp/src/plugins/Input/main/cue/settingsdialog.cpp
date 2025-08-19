#include "settingsdialog.h"
#ifdef WITH_ENCA
#  include <enca.h>
#endif
#include <QSettings>
#include <qmmp/qmmp.h>
#include <qmmp/qmmptextcodec.h>
#include <qmmp/regularexpression.h>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_WIN
    setFixedHeight(205);
#elif defined Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    m_ui.cueEncComboBox->addItems(QmmpTextCodec::availableCharsets());

#ifdef WITH_ENCA
    size_t n = 0;
    const char **langs = enca_get_languages(&n);
    for(size_t i = 0; i < n; ++i)
        m_ui.encaAnalyserComboBox->addItem(langs[i]);
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CUE");
    int pos = m_ui.cueEncComboBox->findText(settings.value("encoding", "UTF-8").toString());
    m_ui.cueEncComboBox->setCurrentIndex(pos);
#ifdef WITH_ENCA
    m_ui.autoCharsetCheckBox->setChecked(settings.value("use_enca", false).toBool());
    pos = m_ui.encaAnalyserComboBox->findText(settings.value("enca_lang", langs[n-1]).toString());
    m_ui.encaAnalyserComboBox->setCurrentIndex(pos);
#else
    m_ui.autoCharsetCheckBox->setEnabled(false);
#endif
    m_ui.dirtyCueCheckBox->setChecked(settings.value("dirty_cue", false).toBool());
    settings.endGroup();
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("CUE");
    settings.setValue("encoding", m_ui.cueEncComboBox->currentText());
#ifdef WITH_ENCA
    settings.setValue("use_enca", m_ui.autoCharsetCheckBox->isChecked());
    settings.setValue("enca_lang", m_ui.encaAnalyserComboBox->currentText());
#endif
    settings.setValue("dirty_cue", m_ui.dirtyCueCheckBox->isChecked());
    settings.endGroup();
    QDialog::accept();
}
