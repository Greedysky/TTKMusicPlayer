#include "musicsoundkmicrowidget.h"
#include "ui_musicsoundkmicrowidget.h"
#include "musicsoundkmicrosearchwidget.h"
#include "musicfunctionuiobject.h"
#include "musiccoremplayer.h"
#include "musicsettingmanager.h"
#include "musiclrcanalysis.h"
#include "musiclrcmanagerforinterior.h"
#include "musicdownloadsourcerequest.h"
#include "musicvideouiobject.h"
#include "musictinyuiobject.h"
#include "musictoolsetsuiobject.h"
#include "musicmessagebox.h"
#include "musictoastlabel.h"
#include "musicaudiorecordermodule.h"
#include "musicfileutils.h"

MusicSoundKMicroWidget::MusicSoundKMicroWidget(QWidget *parent)
    : MusicAbstractMoveWidget(false, parent),
    m_ui(new Ui::MusicSoundKMicroWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->stackedWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle02);
    m_ui->topWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle06);
    m_ui->controlWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle06);
    m_ui->timeLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    m_ui->timeSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle01);
    m_ui->transferButton->setStyleSheet(MusicUIObject::MQSSRecordTransfer);

    m_queryMovieMode = true;
    m_stateButtonOn = true;
    m_intervalTime = 0;
    m_recordCore = nullptr;

    recordStateChanged(false);
    setButtonStyle(true);
    setStateButtonStyle(true);

    m_ui->gifLabel->setType(MusicGifLabelWidget::RecordRed);
    m_ui->loadingLabel->setType(MusicGifLabelWidget::CicleBlue);
    m_ui->loadingLabel->hide();

    m_ui->volumeButton->setValue(100);
    m_ui->volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_player = new MusicCoreMPlayer(this);
    m_searchWidget = new MusicSoundKMicroSearchWidget(this);
    m_searchWidget->connectTo(this);
    m_searchWidget->show();

    m_ui->winTipsButton->setStyleSheet(MusicUIObject::MQSSTinyBtnWintopOff);

    m_analysis = new MusicLrcAnalysis(this);
    m_analysis->setLineMax(5);
    m_ui->musicPage->connectTo(this);

    for(int i = 0; i < m_analysis->lineMax(); ++i)
    {
        MusicLrcManagerForInterior *w = new MusicLrcManagerForInterior(this);
        w->setLrcPerWidth(-10);
        m_ui->musicPage->addWidget(w);
        m_musicLrcContainer.append(w);
    }

    m_recordCore = new MusicAudioRecorderModule(this);
    m_ui->transferButton->setAudioCore(m_recordCore);

#ifdef Q_OS_UNIX
    m_ui->stateButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->winTipsButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->winTipsButton, SIGNAL(clicked()), SLOT(tipsButtonChanged()));
    connect(m_ui->stateButton, SIGNAL(clicked()), SLOT(stateButtonChanged()));
    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(playButtonChanged()));
    connect(m_ui->timeSlider, SIGNAL(sliderReleasedAt(int)), SLOT(setPosition(int)));
    connect(m_ui->volumeButton, SIGNAL(musicVolumeChanged(int)), SLOT(volumeChanged(int)));
    connect(m_ui->recordButton, SIGNAL(clicked()), SLOT(recordButtonClicked()));
    connect(m_player, SIGNAL(finished(int)), SLOT(playFinished()));
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
}

MusicSoundKMicroWidget::~MusicSoundKMicroWidget()
{
    G_SINGLE_MANAGER_PTR->removeObject(className());
    delete m_ui;
}

void MusicSoundKMicroWidget::setButtonStyle(bool style) const
{
    m_ui->playButton->setStyleSheet(style ? MusicUIObject::MQSSVideoBtnPlay : MusicUIObject::MQSSVideoBtnPause);
}

void MusicSoundKMicroWidget::setStateButtonStyle(bool style)  const
{
    m_ui->stateButton->setStyleSheet(style ? MusicUIObject::MQSSVideoBtnOrigin : MusicUIObject::MQSSVideoBtnOriginOff);
}

void MusicSoundKMicroWidget::startSeachKMicro(const QString &name)
{
    m_searchWidget->startSeachKMicro(name);
}

void MusicSoundKMicroWidget::volumeChanged(int volume)
{
    m_player->setVolume(volume);
}

void MusicSoundKMicroWidget::positionChanged(qint64 position)
{
    m_ui->timeSlider->setValue(position * MT_S2MS);
    m_ui->timeLabel->setText(QString("%1/%2").arg(MusicTime::msecTime2LabelJustified(position * MT_S2MS), MusicTime::msecTime2LabelJustified(m_ui->timeSlider->maximum())));

    if(!m_queryMovieMode && !m_analysis->isEmpty())
    {
        QString currentLrc, laterLrc;
        if(m_analysis->findText(m_ui->timeSlider->value(), m_ui->timeSlider->maximum(), currentLrc, laterLrc, m_intervalTime))
        {
            if(currentLrc != m_musicLrcContainer[m_analysis->lineMiddle()]->text())
            {
                if(m_analysis->isValid())
                {
                    m_ui->musicPage->start();
                }
            }
        }
    }
}

void MusicSoundKMicroWidget::durationChanged(qint64 duration)
{
    m_ui->loadingLabel->run(false);
    m_ui->timeSlider->setRange(0, duration * MT_S2MS);
    m_ui->timeLabel->setText(QString("%1/%2").arg(MUSIC_TIME_INIT, MusicTime::msecTime2LabelJustified(duration * MT_S2MS)));

    multiMediaChanged();
}

void MusicSoundKMicroWidget::playFinished()
{
    m_player->stop();
    if(m_ui->gifLabel->isRunning())
    {
        MusicToastLabel::popup(tr("Record finished"));

        recordStateChanged(false);

        const QString &path = MusicUtils::File::saveFileDialog(this, "Wav(*.wav)");
        if(!path.isEmpty())
        {
            m_recordCore->addWavHeader(qPrintable(path));
        }
    }
}

void MusicSoundKMicroWidget::setPosition(int position)
{
    m_player->setPosition(position/MT_S2MS);
    m_analysis->setSongSpeedChanged(position);
}

void MusicSoundKMicroWidget::playButtonChanged()
{
    m_player->play();
    switch(m_player->state())
    {
        case MusicObject::PlayingState: setButtonStyle(false); break;
        case MusicObject::PausedState: setButtonStyle(true); break;
        default: break;
    }

    if(!m_queryMovieMode)
    {
        if(m_player->state() == MusicObject::PlayingState)
        {
            m_musicLrcContainer[m_analysis->lineMiddle()]->startDrawLrcMask(m_intervalTime);
        }
        else
        {
            m_musicLrcContainer[m_analysis->lineMiddle()]->stopDrawLrc();
            m_ui->musicPage->stop();
        }
    }
}

void MusicSoundKMicroWidget::stateButtonChanged()
{
    m_stateButtonOn = !m_stateButtonOn;
    setStateButtonStyle(m_stateButtonOn);

    multiMediaChanged();
}

void MusicSoundKMicroWidget::tipsButtonChanged()
{
    if(m_ui->winTipsButton->styleSheet().contains(MusicUIObject::MQSSTinyBtnWintopOff))
    {
        m_ui->winTipsButton->setStyleSheet(MusicUIObject::MQSSTinyBtnWintopOn);
        m_searchWidget->hide();
    }
    else
    {
        m_ui->winTipsButton->setStyleSheet(MusicUIObject::MQSSTinyBtnWintopOff);
        m_searchWidget->show();
    }
}

void MusicSoundKMicroWidget::mediaUrlChanged(bool mv, const QString &url, const QString &lrcUrl)
{
    setButtonStyle(false);

    m_ui->loadingLabel->show();
    m_ui->loadingLabel->start();

    if(m_queryMovieMode = mv)
    {
        m_ui->stackedWidget->setCurrentIndex(SOUND_KMICRO_INDEX_0);
        m_player->setMedia(MusicCoreMPlayer::VideoCategory, url, (int)m_ui->videoPage->winId());
        m_player->play();
    }
    else
    {
        m_ui->stackedWidget->setCurrentIndex(SOUND_KMICRO_INDEX_1);
        m_player->setMedia(MusicCoreMPlayer::MusicCategory, url);
        m_player->play();

        //
        MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
        connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        download->startToDownload(lrcUrl);
    }
}

void MusicSoundKMicroWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_LOGGER_ERROR("Input byte data is empty");
        return;
    }

    m_analysis->setLrcData(bytes);
    for(int i = 0; i < m_analysis->lineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText(QString());
    }

    setItemStyleSheet(0, -3, 90);
    setItemStyleSheet(1, -6, 35);
    setItemStyleSheet(2, -10, 0);
    setItemStyleSheet(3, -6, 35);
    setItemStyleSheet(4, -3, 90);
}

void MusicSoundKMicroWidget::updateAnimationLrc()
{
    for(int i = 0; i < m_analysis->lineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText(m_analysis->text(i));
    }
    m_analysis->setCurrentIndex(m_analysis->currentIndex() + 1);
    m_musicLrcContainer[m_analysis->lineMiddle()]->startDrawLrcMask(m_intervalTime);
}

void MusicSoundKMicroWidget::recordButtonClicked()
{
    if(m_ui->gifLabel->isRunning())
    {
        MusicMessageBox message;
        message.setText(tr("Recording now, do you want to stop it?"));
        if(message.exec())
        {
            recordStateChanged(false);
        }
    }
    else
    {
        const int index = m_ui->transferButton->audioInputIndex();
        if(index != -1)
        {
            recordStateChanged(true);
        }
        else
        {
            MusicToastLabel::popup(tr("Audio input error"));
        }
    }
}

void MusicSoundKMicroWidget::closeEvent(QCloseEvent *event)
{
    MusicAbstractMoveWidget::closeEvent(event);
    G_SINGLE_MANAGER_PTR->removeObject(className());

    qDeleteAll(m_musicLrcContainer);
    delete m_analysis;
    delete m_player;
    delete m_searchWidget;
    delete m_recordCore;
}

void MusicSoundKMicroWidget::paintEvent(QPaintEvent *event)
{
    MusicAbstractMoveWidget::paintEvent(event);
    m_searchWidget->move(geometry().topRight() + QPoint(5, -4));
}

void MusicSoundKMicroWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicAbstractMoveWidget::mouseMoveEvent(event);
    m_searchWidget->move(geometry().topRight() + QPoint(5, -4));
}

void MusicSoundKMicroWidget::multiMediaChanged()
{
    if(m_queryMovieMode)
    {
        m_player->setMultiVoice(m_stateButtonOn ? 0 : 1);
    }
    else
    {
        m_stateButtonOn ? m_player->setRightVolume() : m_player->setLeftVolume();
    }

    volumeChanged(m_ui->volumeButton->value());
}

void MusicSoundKMicroWidget::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLrcManagerForInterior *w = m_musicLrcContainer[index];
    w->setFontSize(size);

    const int value = qBound<int>(0, 100 - transparent, 100);
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(G_SETTING_PTR->value("LrcColor").toInt() != -1)
    {
        const MusicLrcColor::LrcColorType index = TTKStatic_cast(MusicLrcColor::LrcColorType, G_SETTING_PTR->value("LrcColor").toInt());
        const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(index);
        w->setLinearGradientColor(cl);
    }
    else
    {
        const MusicLrcColor cl(MusicLrcColor::readColorConfig(G_SETTING_PTR->value("LrcFrontgroundColor").toString()),
                               MusicLrcColor::readColorConfig(G_SETTING_PTR->value("LrcBackgroundColor").toString()));
        w->setLinearGradientColor(cl);
    }
}

void MusicSoundKMicroWidget::recordStateChanged(bool state)
{
    if(state && m_player->state() != MusicObject::StoppedState)
    {
        m_ui->gifLabel->start();
        m_ui->recordButton->setStyleSheet(MusicUIObject::MQSSRerecord);
        if(m_recordCore)
        {
            m_recordCore->onRecordStart();
            if(m_recordCore->error())
            {
                MusicToastLabel::popup(tr("Audio init error!"));
                return;
            }
        }
    }
    else
    {
        m_ui->gifLabel->stop();
        m_ui->recordButton->setStyleSheet(MusicUIObject::MQSSRecord);
        if(m_recordCore)
        {
            m_recordCore->onRecordStop();
        }
    }
}
