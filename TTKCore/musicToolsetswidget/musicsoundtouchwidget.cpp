#include "musicsoundtouchwidget.h"
#include "ui_musicsoundtouchwidget.h"
#include "musicbackgroundmanager.h"
#include "musicaudiorecordercore.h"
#include "musicmessagebox.h"
#include "musicobject.h"
#include "musicuiobject.h"

#include <QProcess>

MusicSoundTouchWidget::MusicSoundTouchWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSoundTouchWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->openButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);

    ui->tempoSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->pitchSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->rateSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->tempoSlider->setRange(-95, 5000);
    ui->pitchSlider->setRange(-60, 60);
    ui->rateSlider->setRange(-95, 5000);
    connect(ui->tempoSlider, SIGNAL(valueChanged(int)), SLOT(tempoSliderValueChanged(int)));
    connect(ui->pitchSlider, SIGNAL(valueChanged(int)), SLOT(pitchSliderValueChanged(int)));
    connect(ui->rateSlider, SIGNAL(valueChanged(int)), SLOT(rateSliderValueChanged(int)));

    ui->playWavButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->transformButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    connect(ui->playWavButton, SIGNAL(clicked()), SLOT(playWavButtonClicked()));
    connect(ui->transformButton, SIGNAL(clicked()), SLOT(transformButtonClicked()));

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(analysisOutput()));

    ui->tempoSlider->setValue(2500);
    ui->pitchSlider->setValue(0);
    ui->rateSlider->setValue(2500);
    ui->tempoLabelValue->setText("2500");
    ui->pitchLabelValue->setText("0");
    ui->rateLabelValue->setText("2500");

    m_recordCore = new MusicAudioRecorderCore(this);

}

MusicSoundTouchWidget::~MusicSoundTouchWidget()
{
    delete m_recordCore;
    delete m_process;
    delete ui;
}

int MusicSoundTouchWidget::exec()
{
    if(!QFile::exists(MAKE_SOUNDTOUCH_FULL))
    {
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return -1;
    }

    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

void MusicSoundTouchWidget::analysisOutput()
{

}

void MusicSoundTouchWidget::onRecordStart()
{
    m_recordCore->onRecordStart();
}

void MusicSoundTouchWidget::onRecordStop()
{
    m_recordCore->onRecordStop();
}

void MusicSoundTouchWidget::onRecordPlay()
{
    m_recordCore->onRecordPlay();
}

void MusicSoundTouchWidget::tempoSliderValueChanged(int value)
{
    ui->tempoLabelValue->setText(QString::number(value));
}

void MusicSoundTouchWidget::pitchSliderValueChanged(int value)
{
    ui->pitchLabelValue->setText(QString::number(value));
}

void MusicSoundTouchWidget::rateSliderValueChanged(int value)
{
    ui->rateLabelValue->setText(QString::number(value));
}

void MusicSoundTouchWidget::playWavButtonClicked()
{

}

void MusicSoundTouchWidget::transformButtonClicked()
{
    QStringList key;
//    key <<
//    QStringList() << "-i" << in << "-y"
//                             << "-ab" << ui->kbpsCombo->currentText() + "k"
//                             << "-ar" << ui->hzCombo->currentText()
//                             << "-ac" << QString::number(ui->msCombo->
    m_process->start(MAKE_SOUNDTOUCH_FULL, key);
}
