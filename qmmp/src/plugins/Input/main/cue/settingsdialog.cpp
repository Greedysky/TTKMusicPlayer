#include "settingsdialog.h"
#ifdef WITH_ENCA
#  include <enca.h>
#endif
#include <QSettings>
#include <QTextCodec>
#include <qmmp/qmmp.h>
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
    findCodecs();
    for(const QTextCodec *codec : qAsConst(m_codecs))
    {
        m_ui.cueEncComboBox->addItem(codec->name());
    }

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

void SettingsDialog::findCodecs()
{
    QMap<QString, QTextCodec *> codecMap;
    RegularExpression iso8859RegExp("ISO[- ]8859-([0-9]+).*");

    for(int mib : QTextCodec::availableMibs())
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
            if(iso8859RegExp.value(1).length() == 1)
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
