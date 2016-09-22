#include "musicsoundtouchwidget.h"
#include "ui_musicsoundtouchwidget.h"
#include "musicaudiorecordercore.h"
#include "musicmessagebox.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"

#include <QSound>
#include <QProcess>
#include <QFileDialog>

MusicSoundTouchWidget::MusicSoundTouchWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSoundTouchWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->openButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    ui->tempoSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->pitchSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->rateSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->tempoSlider->setRange(-95, 5000);
    ui->pitchSlider->setRange(-60, 60);
    ui->rateSlider->setRange(-95, 5000);
    connect(ui->tempoSlider, SIGNAL(valueChanged(int)), SLOT(tempoSliderValueChanged(int)));
    connect(ui->pitchSlider, SIGNAL(valueChanged(int)), SLOT(pitchSliderValueChanged(int)));
    connect(ui->rateSlider, SIGNAL(valueChanged(int)), SLOT(rateSliderValueChanged(int)));

    ui->playWavButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->transformButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    connect(ui->playWavButton, SIGNAL(clicked()), SLOT(onRecordPlay()));
    connect(ui->transformButton, SIGNAL(clicked()), SLOT(transformButtonClicked()));

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(readyReadStandardOutput()), SLOT(analysisOutput()));
    connect(m_process, SIGNAL(finished(int)), SLOT(finished(int)));

    ui->tempoSlider->setValue(2500);
    ui->pitchSlider->setValue(0);
    ui->rateSlider->setValue(2500);
    ui->tempoLabelValue->setText("2500");
    ui->pitchLabelValue->setText("0");
    ui->rateLabelValue->setText("2500");
    setText(MusicObject::getAppDir() + RECORD_FILE);
    ui->playWavButton->setEnabled(false);
    ui->transformButton->setEnabled(false);

    m_recordCore = new MusicAudioRecorderCore(this);

    connect(ui->playButton, SIGNAL(clicked()), SLOT(onRecordStart()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(onRecordStop()));
    connect(ui->openButton, SIGNAL(clicked()), SLOT(openWavButtonClicked()));
}

MusicSoundTouchWidget::~MusicSoundTouchWidget()
{
    delete m_recordCore;
    delete m_process;
    delete ui;
}

QString MusicSoundTouchWidget::getClassName()
{
    return staticMetaObject.className();
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

    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSoundTouchWidget::analysisOutput()
{
//    while(m_process->canReadLine())
//    {
//        QByteArray data = m_process->readLine();
//    }
}

void MusicSoundTouchWidget::onRecordStart()
{
    m_recordCore->onRecordStart();
    ui->playButton->setEnabled(false);
    ui->openButton->setEnabled(false);
}

void MusicSoundTouchWidget::onRecordStop()
{
    m_recordCore->onRecordStop();
    ui->playButton->setEnabled(true);
    ui->openButton->setEnabled(true);
    ui->transformButton->setEnabled(true);
}

void MusicSoundTouchWidget::onRecordPlay()
{
    QSound::play(MusicObject::getAppDir() + RECORD_OUT_FILE);
//    m_recordCore->onRecordPlay();
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

void MusicSoundTouchWidget::openWavButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("choose a filename to open under"), QDir::currentPath(), "Wav(*.wav)");
    if(!filename.isEmpty())
    {
        ui->transformButton->setEnabled(true);
        m_recordCore->setFileName(filename);
        setText(filename);
    }
}

void MusicSoundTouchWidget::transformButtonClicked()
{
    QString input = m_recordCore->getFileName();
    if(input == RECORD_FILE)
    {
        m_recordCore->addWavHeader(RECORD_IN_FILE);
        input = MusicObject::getAppDir() + RECORD_IN_FILE;
    }

    QStringList key;
    key << input << (MusicObject::getAppDir() + RECORD_OUT_FILE)
                          << QString("-tempo=%1").arg(ui->tempoSlider->value())
                          << QString("-pitch=%1").arg(ui->pitchSlider->value())
                          << QString("-rate=%1").arg(ui->rateSlider->value());
    m_process->start(MAKE_SOUNDTOUCH_FULL, key);
}

void MusicSoundTouchWidget::finished(int code)
{
    if(code != 0)
    {
        MusicMessageBox message;
        message.setText(tr("Transform wav file error!"));
        message.exec();
        return;
    }
    ui->playWavButton->setEnabled(true);
}

void MusicSoundTouchWidget::setText(const QString &text)
{
    ui->pathLabel->setText(MusicUtils::Widget::elidedText(font(), text, Qt::ElideLeft, 390) );
    ui->pathLabel->setToolTip(text);
}
