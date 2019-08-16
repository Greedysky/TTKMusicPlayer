#include "musicsoundkmicrowidget.h"
#include "ui_musicsoundkmicrowidget.h"
#include "musicsoundkmicrosearchwidget.h"
#include "musicfunctionuiobject.h"
#include "musiccoremplayer.h"
#include "musicsettingmanager.h"
#include "musiclrcanalysis.h"
#include "musiclrcmanagerforinline.h"
#include "musicstringutils.h"
#include "musicdownloadsourcethread.h"
#include "musicvideouiobject.h"
#include "musictinyuiobject.h"
#include "musicuiobject.h"
#include "musictoolsetsuiobject.h"
#include "musicmessagebox.h"
#include "musicaudiorecordercore.h"
#include "musiccodecutils.h"
#include "musicotherdefine.h"
#include "musicfileutils.h"
#include "musictime.h"
#include "musicsinglemanager.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic ignored "-Wparentheses"
#endif

MusicSoundKMicroWidget::MusicSoundKMicroWidget(QWidget *parent)
    : MusicAbstractMoveWidget(false, parent),
    m_ui(new Ui::MusicSoundKMicroWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->stackedWidget->setStyleSheet(MusicUIObject::MBackgroundStyle02);
    m_ui->topWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06);
    m_ui->controlWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06);
    m_ui->timeLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->timeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    m_ui->transferButton->setStyleSheet(MusicUIObject::MKGRecordTransfer);

    m_queryMovieMode = true;
    m_stateButtonOn = true;
    m_intervalTime = 0;
    m_recordCore = nullptr;

    recordStateChanged(false);
    setButtonStyle(true);
    setStateButtonStyle(true);

    m_ui->gifLabel->setType(MusicGifLabelWidget::Gif_Record_red);
    m_ui->loadingLabel->setType(MusicGifLabelWidget::Gif_Cicle_Blue);
    m_ui->loadingLabel->hide();

    m_ui->volumeButton->setValue(100);
    m_ui->volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_mediaPlayer = new MusicCoreMPlayer(this);
    m_searchWidget = new MusicSoundKMicroSearchWidget(this);
    m_searchWidget->connectTo(this);
    m_searchWidget->show();

    m_ui->winTipsButton->setStyleSheet(MusicUIObject::MKGTinyBtnWintopOff);

    m_analysis = new MusicLrcAnalysis(this);
    m_analysis->setLineMax(5);
    m_ui->musicPage->connectTo(this);

    for(int i=0; i<m_analysis->getLineMax(); ++i)
    {
        MusicLrcManagerForInline *w = new MusicLrcManagerForInline(this);
        w->setLrcPerWidth(-10);
        m_ui->musicPage->addWidget(w);
        m_musicLrcContainer.append(w);
    }

    m_recordCore = new MusicAudioRecorderCore(this);
    m_ui->transferButton->setAudioCore(m_recordCore);

#ifdef Q_OS_UNIX
    m_ui->stateButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->winTipsButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->winTipsButton, SIGNAL(clicked()), SLOT(tipsButtonChanged()));
    connect(m_ui->stateButton, SIGNAL(clicked()), SLOT(stateButtonChanged()));
    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(playButtonChanged()));
    connect(m_mediaPlayer, SIGNAL(finished(int)), SLOT(playFinished()));
    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_ui->timeSlider, SIGNAL(sliderReleasedAt(int)), SLOT(setPosition(int)));
    connect(m_ui->volumeButton, SIGNAL(musicVolumeChanged(int)), SLOT(volumeChanged(int)));
    connect(m_ui->recordButton, SIGNAL(clicked()), SLOT(recordButtonClicked()));
}

MusicSoundKMicroWidget::~MusicSoundKMicroWidget()
{
    delete m_ui;
}

void MusicSoundKMicroWidget::setButtonStyle(bool style) const
{
    m_ui->playButton->setStyleSheet(style ? MusicUIObject::MKGVideoBtnPlay : MusicUIObject::MKGVideoBtnPause);
}

void MusicSoundKMicroWidget::setStateButtonStyle(bool style)  const
{
    m_ui->stateButton->setStyleSheet(style ? MusicUIObject::MKGVideoBtnOrigin : MusicUIObject::MKGVideoBtnOriginOff);
}

void MusicSoundKMicroWidget::startSeachKMicro(const QString &name)
{
    m_searchWidget->startSeachKMicro(name);
}

void MusicSoundKMicroWidget::volumeChanged(int volume)
{
    m_mediaPlayer->setVolume(volume);
}

void MusicSoundKMicroWidget::positionChanged(qint64 position)
{
    m_ui->timeSlider->setValue(position*MT_S2MS);
    m_ui->timeLabel->setText(QString("%1/%2").arg(MusicTime::msecTime2LabelJustified(position*MT_S2MS)).arg(MusicTime::msecTime2LabelJustified(m_ui->timeSlider->maximum())));

    if(!m_queryMovieMode && !m_analysis->isEmpty())
    {
        QString currentLrc, laterLrc;
        if(m_analysis->findText(m_ui->timeSlider->value(), m_ui->timeSlider->maximum(), currentLrc, laterLrc, m_intervalTime))
        {
            if(currentLrc != m_musicLrcContainer[m_analysis->getMiddle()]->text())
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
    m_ui->timeSlider->setRange(0, duration*MT_S2MS);
    m_ui->timeLabel->setText(QString("%1/%2").arg(MUSIC_TIME_INIT).arg(MusicTime::msecTime2LabelJustified(duration*MT_S2MS)));

    multiMediaChanged();
}

void MusicSoundKMicroWidget::playFinished()
{
    m_mediaPlayer->stop();
    if(m_ui->gifLabel->isRunning())
    {
        MusicMessageBox message;
        message.setText(tr("Record Finished"));
        message.exec();

        recordStateChanged(false);

        const QString &filename = MusicUtils::File::getSaveFileDialog(this, "Wav(*.wav)");
        if(!filename.isEmpty())
        {
            m_recordCore->addWavHeader(MusicUtils::Codec::toLocal8Bit(filename));
        }
    }
}

void MusicSoundKMicroWidget::setPosition(int position)
{
    m_mediaPlayer->setPosition(position/MT_S2MS);
    m_analysis->setSongSpeedChanged(position);
}

void MusicSoundKMicroWidget::playButtonChanged()
{
    m_mediaPlayer->play();
    switch(m_mediaPlayer->state())
    {
        case MusicObject::PS_PlayingState:
            setButtonStyle(false);
            break;
        case MusicObject::PS_PausedState:
            setButtonStyle(true);
            break;
        default: break;
    }

    if(!m_queryMovieMode)
    {
        if(m_mediaPlayer->state() == MusicObject::PS_PlayingState)
        {
            m_musicLrcContainer[m_analysis->getMiddle()]->startLrcMask(m_intervalTime);
        }
        else
        {
            m_musicLrcContainer[m_analysis->getMiddle()]->stopLrcMask();
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
    if(m_ui->winTipsButton->styleSheet().contains(MusicUIObject::MKGTinyBtnWintopOff))
    {
        m_ui->winTipsButton->setStyleSheet(MusicUIObject::MKGTinyBtnWintopOn);
        m_searchWidget->hide();
    }
    else
    {
        m_ui->winTipsButton->setStyleSheet(MusicUIObject::MKGTinyBtnWintopOff);
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
        m_mediaPlayer->setMedia(MusicCoreMPlayer::VideoCategory, url, (int)m_ui->videoPage->winId());
        m_mediaPlayer->play();
    }
    else
    {
        m_ui->stackedWidget->setCurrentIndex(SOUND_KMICRO_INDEX_1);
        m_mediaPlayer->setMedia(MusicCoreMPlayer::MusicCategory, url);
        m_mediaPlayer->play();

        //
        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        download->startToDownload(lrcUrl);
    }
}

void MusicSoundKMicroWidget::downLoadFinished(const QByteArray &data)
{
    m_analysis->setLrcData(data);

    for(int i=0; i<m_analysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText( QString() );
    }
    setItemStyleSheet(0, -3, 90);
    setItemStyleSheet(1, -6, 35);
    setItemStyleSheet(2, -10, 0);
    setItemStyleSheet(3, -6, 35);
    setItemStyleSheet(4, -3, 90);
}

void MusicSoundKMicroWidget::updateAnimationLrc()
{
    for(int i=0; i<m_analysis->getLineMax(); ++i)
    {
        m_musicLrcContainer[i]->setText(m_analysis->getText(i));
    }
    m_analysis->setCurrentIndex(m_analysis->getCurrentIndex() + 1);
    m_musicLrcContainer[m_analysis->getMiddle()]->startLrcMask(m_intervalTime);
}

void MusicSoundKMicroWidget::recordButtonClicked()
{
    if(m_ui->gifLabel->isRunning())
    {
        MusicMessageBox message;
        message.setText(tr("Recording Now, Stop It?"));
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
            MusicMessageBox message;
            message.setText(tr("Input Error"));
            message.exec();
        }
    }
}

void MusicSoundKMicroWidget::closeEvent(QCloseEvent *event)
{
    MusicAbstractMoveWidget::closeEvent(event);
    M_SINGLE_MANAGER_PTR->removeObject(getClassName());

    qDeleteAll(m_musicLrcContainer);
    delete m_analysis;
    delete m_mediaPlayer;
    delete m_searchWidget;
    delete m_recordCore;
}

void MusicSoundKMicroWidget::paintEvent(QPaintEvent *event)
{
    MusicAbstractMoveWidget::paintEvent(event);
    m_searchWidget->move( geometry().topRight() + QPoint(5, -4) );
}

void MusicSoundKMicroWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicAbstractMoveWidget::mouseMoveEvent(event);
    m_searchWidget->move( geometry().topRight() + QPoint(5, -4) );
}

void MusicSoundKMicroWidget::multiMediaChanged()
{
    if(m_queryMovieMode)
    {
        m_mediaPlayer->setMultiVoice(m_stateButtonOn ? 0 : 1);
    }
    else
    {
        m_stateButtonOn ? m_mediaPlayer->setRightVolume() : m_mediaPlayer->setLeftVolume();
    }

    volumeChanged(m_ui->volumeButton->value());
}

void MusicSoundKMicroWidget::setItemStyleSheet(int index, int size, int transparent)
{
    MusicLrcManagerForInline *w = m_musicLrcContainer[index];
    w->setFontSize(size);

    const int value = qBound<int>(0, 100 - transparent, 100);
    w->setFontTransparent(value);
    w->setTransparent(value);

    if(M_SETTING_PTR->value("LrcColorChoiced").toInt() != -1)
    {
        const MusicLrcColor::LrcColorType index = MStatic_cast(MusicLrcColor::LrcColorType, M_SETTING_PTR->value("LrcColorChoiced").toInt());
        const MusicLrcColor &cl = MusicLrcColor::mapIndexToColor(index);
        w->setLinearGradientColor(cl);
    }
    else
    {
        const MusicLrcColor cl(MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcFrontgroundColorChoiced").toString()),
                               MusicUtils::String::readColorConfig(M_SETTING_PTR->value("LrcBackgroundColorChoiced").toString()));
        w->setLinearGradientColor(cl);
    }
}

void MusicSoundKMicroWidget::recordStateChanged(bool state)
{
    if(state && m_mediaPlayer->state() != MusicObject::PS_StoppedState)
    {
        m_ui->gifLabel->start();
        m_ui->recordButton->setStyleSheet(MusicUIObject::MKGRerecord);
        if(m_recordCore)
        {
            m_recordCore->onRecordStart();
            if(m_recordCore->error())
            {
                return;
            }
        }
    }
    else
    {
        m_ui->gifLabel->stop();
        m_ui->recordButton->setStyleSheet(MusicUIObject::MKGRecord);
        if(m_recordCore)
        {
            m_recordCore->onRecordStop();
        }
    }
}
