#include "musicsoundtouchwidget.h"
#include "ui_musicsoundtouchwidget.h"
#include "musicaudiorecordercore.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicfileutils.h"
#include "musicwidgetutils.h"
#include "musicsinglemanager.h"

#include <QSound>
#include <QProcess>

MusicSoundTouchWidget::MusicSoundTouchWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicSoundTouchWidget)
{
    m_ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->stopButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->openButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    m_ui->tempoSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_ui->pitchSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_ui->rateSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_ui->tempoSlider->setRange(-95, 5000);
    m_ui->pitchSlider->setRange(-60, 60);
    m_ui->rateSlider->setRange(-95, 5000);
    connect(m_ui->tempoSlider, SIGNAL(valueChanged(int)), SLOT(tempoSliderValueChanged(int)));
    connect(m_ui->pitchSlider, SIGNAL(valueChanged(int)), SLOT(pitchSliderValueChanged(int)));
    connect(m_ui->rateSlider, SIGNAL(valueChanged(int)), SLOT(rateSliderValueChanged(int)));

    m_ui->playWavButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->transformButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    connect(m_ui->playWavButton, SIGNAL(clicked()), SLOT(onRecordPlay()));
    connect(m_ui->transformButton, SIGNAL(clicked()), SLOT(transformButtonClicked()));

#ifdef Q_OS_UNIX
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->stopButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playWavButton->setFocusPolicy(Qt::NoFocus);
    m_ui->transformButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_process, SIGNAL(finished(int)), SLOT(finished(int)));

    m_ui->tempoSlider->setValue(2500);
    m_ui->pitchSlider->setValue(0);
    m_ui->rateSlider->setValue(2500);
    m_ui->tempoLabelValue->setText("2500");
    m_ui->pitchLabelValue->setText("0");
    m_ui->rateLabelValue->setText("2500");

    setText(MusicObject::getAppDir() + MUSIC_RECORD_FILE);
    m_ui->playWavButton->setEnabled(false);
    m_ui->transformButton->setEnabled(false);

    m_recordCore = new MusicAudioRecorderCore(this);

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(onRecordStart()));
    connect(m_ui->stopButton, SIGNAL(clicked()), SLOT(onRecordStop()));
    connect(m_ui->openButton, SIGNAL(clicked()), SLOT(openWavButtonClicked()));
}

MusicSoundTouchWidget::~MusicSoundTouchWidget()
{
    M_SINGLE_MANAGER_PTR->removeObject(getClassName());
    delete m_recordCore;
    delete m_process;
    delete m_ui;
}

void MusicSoundTouchWidget::show()
{
    if(!QFile::exists(MAKE_SOUNDTOUCH_FULL))
    {
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return;
    }

    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}

void MusicSoundTouchWidget::onRecordStart()
{
    m_recordCore->onRecordStart();
    if(m_recordCore->error())
    {
        return;
    }

    m_ui->playButton->setEnabled(false);
    m_ui->openButton->setEnabled(false);
}

void MusicSoundTouchWidget::onRecordStop()
{
    m_recordCore->onRecordStop();
    m_ui->playButton->setEnabled(true);
    m_ui->openButton->setEnabled(true);
    m_ui->transformButton->setEnabled(true);
}

void MusicSoundTouchWidget::onRecordPlay()
{
    QSound::play(MusicObject::getAppDir() + MUSIC_RECORD_OUT_FILE);
//    m_recordCore->onRecordPlay();
}

void MusicSoundTouchWidget::tempoSliderValueChanged(int value)
{
    m_ui->tempoLabelValue->setText(QString::number(value));
}

void MusicSoundTouchWidget::pitchSliderValueChanged(int value)
{
    m_ui->pitchLabelValue->setText(QString::number(value));
}

void MusicSoundTouchWidget::rateSliderValueChanged(int value)
{
    m_ui->rateLabelValue->setText(QString::number(value));
}

void MusicSoundTouchWidget::openWavButtonClicked()
{
    const QString &filename = MusicUtils::File::getOpenFileDialog(this, "Wav(*.wav)");
    if(!filename.isEmpty())
    {
        m_ui->transformButton->setEnabled(true);
        m_recordCore->setFileName(filename);
        setText(filename);
    }
}

void MusicSoundTouchWidget::transformButtonClicked()
{
    QString input = m_recordCore->getFileName();
    if(input == MUSIC_RECORD_FILE)
    {
        m_recordCore->addWavHeader(MUSIC_RECORD_IN_FILE);
        input = MusicObject::getAppDir() + MUSIC_RECORD_IN_FILE;
    }

    QStringList key;
    key << input << (MusicObject::getAppDir() + MUSIC_RECORD_OUT_FILE)
        << QString("-tempo=%1").arg(m_ui->tempoSlider->value())
        << QString("-pitch=%1").arg(m_ui->pitchSlider->value())
        << QString("-rate=%1").arg(m_ui->rateSlider->value());
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
    m_ui->playWavButton->setEnabled(true);
}

void MusicSoundTouchWidget::setText(const QString &text)
{
    m_ui->pathLabel->setText(MusicUtils::Widget::elidedText(font(), text, Qt::ElideLeft, 390));
    m_ui->pathLabel->setToolTip(text);
}
