#include <QSettings>
#include <QAbstractButton>
#include <qmmp/qmmp.h>
#include "settingsdialog.h"

extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
}

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_OS_WIN
    setFixedHeight(350);
#elif defined Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const QStringList &disabledFilters = settings.value("FFMPEG/disabled_filters").toStringList();

    m_ui.wmaCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_WMAV1));
    m_ui.wmaCheckBox->setChecked(!disabledFilters.contains("*.wma") && avcodec_find_decoder(AV_CODEC_ID_WMAV1));
    m_ui.apeCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_APE));
    m_ui.apeCheckBox->setChecked(!disabledFilters.contains("*.ape") && avcodec_find_decoder(AV_CODEC_ID_APE));
    m_ui.ttaCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_TTA));
    m_ui.ttaCheckBox->setChecked(!disabledFilters.contains("*.tta") && avcodec_find_decoder(AV_CODEC_ID_TTA));
    m_ui.aacCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_AAC));
    m_ui.aacCheckBox->setChecked(!disabledFilters.contains("*.aac") && avcodec_find_decoder(AV_CODEC_ID_AAC));
    m_ui.mp4CheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_AAC) || avcodec_find_decoder(AV_CODEC_ID_ALAC));
    m_ui.mp4CheckBox->setChecked(!disabledFilters.contains("*.m4a") && (avcodec_find_decoder(AV_CODEC_ID_AAC) || avcodec_find_decoder(AV_CODEC_ID_ALAC)));
    m_ui.raCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_RA_288));
    m_ui.raCheckBox->setChecked(!disabledFilters.contains("*.ra") && avcodec_find_decoder(AV_CODEC_ID_RA_288));
    m_ui.shCheckBox->setChecked(avcodec_find_decoder(AV_CODEC_ID_SHORTEN));
    m_ui.shCheckBox->setChecked(!disabledFilters.contains("*.shn") && avcodec_find_decoder(AV_CODEC_ID_SHORTEN));
    m_ui.ac3CheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_EAC3));
    m_ui.ac3CheckBox->setChecked(!disabledFilters.contains("*.ac3") && avcodec_find_decoder(AV_CODEC_ID_EAC3));
    m_ui.dtsCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_DTS));
    m_ui.dtsCheckBox->setChecked(!disabledFilters.contains("*.dts") && avcodec_find_decoder(AV_CODEC_ID_DTS));
    m_ui.mkaCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_TRUEHD));
    m_ui.mkaCheckBox->setChecked(!disabledFilters.contains("*.mka") && avcodec_find_decoder(AV_CODEC_ID_TRUEHD));
    m_ui.vqfCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_TWINVQ));
    m_ui.vqfCheckBox->setChecked(!disabledFilters.contains("*.vqf") && avcodec_find_decoder(AV_CODEC_ID_TWINVQ));
    m_ui.takCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_TAK));
    m_ui.takCheckBox->setChecked(!disabledFilters.contains("*.tak") && avcodec_find_decoder(AV_CODEC_ID_TAK));
    m_ui.spxCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_TRUESPEECH));
    m_ui.spxCheckBox->setChecked(!disabledFilters.contains("*.spx") && avcodec_find_decoder(AV_CODEC_ID_TRUESPEECH));
    m_ui.webmCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_OPUS));
    m_ui.webmCheckBox->setChecked(!disabledFilters.contains("*.webm") && avcodec_find_decoder(AV_CODEC_ID_OPUS));
    m_ui.dsdCheckBox->setEnabled(avcodec_find_decoder(AV_CODEC_ID_DSD_LSBF));
    m_ui.dsdCheckBox->setChecked(!disabledFilters.contains("*.dsdiff") && avcodec_find_decoder(AV_CODEC_ID_DSD_LSBF));
}

void SettingsDialog::accept()
{
    QStringList disabledFilters;
    if(!m_ui.wmaCheckBox->isChecked())
        disabledFilters << "*.wma";
    if(!m_ui.apeCheckBox->isChecked())
        disabledFilters << "*.ape";
    if(!m_ui.ttaCheckBox->isChecked())
        disabledFilters << "*.tta";
    if(!m_ui.mp4CheckBox->isChecked())
        disabledFilters << "*.m4a" << "*.m4b";
    if(!m_ui.aacCheckBox->isChecked())
        disabledFilters << "*.aac";
    if(!m_ui.raCheckBox->isChecked())
        disabledFilters << "*.ra";
    if(!m_ui.shCheckBox->isChecked())
        disabledFilters << "*.shn";
    if(!m_ui.ac3CheckBox->isChecked())
        disabledFilters << "*.ac3";
    if(!m_ui.dtsCheckBox->isChecked())
        disabledFilters << "*.dts";
    if(!m_ui.mkaCheckBox->isChecked())
        disabledFilters << "*.mka";
    if(!m_ui.vqfCheckBox->isChecked())
        disabledFilters << "*.vqf";
    if(!m_ui.takCheckBox->isChecked())
        disabledFilters << "*.tak";
    if(!m_ui.spxCheckBox->isChecked())
        disabledFilters << "*.spx";
    if(!m_ui.webmCheckBox->isChecked())
        disabledFilters << "*.webm";
    if(!m_ui.dsdCheckBox->isChecked())
        disabledFilters << "*.dsf" << "*.dsdiff";

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("FFMPEG/disabled_filters", disabledFilters);
    QDialog::accept();
}
