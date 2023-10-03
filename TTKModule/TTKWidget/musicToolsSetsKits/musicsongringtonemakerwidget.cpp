#include "musicsongringtonemakerwidget.h"
#include "ui_musicsongringtonemakerwidget.h"
#include "musiccutsliderwidget.h"
#include "musiccoremplayer.h"
#include "musictoastlabel.h"
#include "musicsongmeta.h"
#include "musicfileutils.h"
#include "ttktime.h"

#include <QProcess>

MusicSongRingtoneMaker::MusicSongRingtoneMaker(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongRingtoneMaker),
      m_playRingtone(false),
      m_startPos(0),
      m_stopPos(TTK_HIGH_LEVEL)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->addSongButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->playSongButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->playRingButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->saveSongButton->setStyleSheet(TTK::UI::PushButtonStyle04);

    TTK::Widget::generateComboBoxFormat(m_ui->formatCombo);
    TTK::Widget::generateComboBoxFormat(m_ui->kbpsCombo);
    TTK::Widget::generateComboBoxFormat(m_ui->hzCombo);
    TTK::Widget::generateComboBoxFormat(m_ui->msCombo);
#ifdef Q_OS_UNIX
    m_ui->addSongButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playSongButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playRingButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveSongButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_player = new MusicCoreMPlayer(this);

    initialize();

    m_ui->playSongButton->setEnabled(false);
    m_ui->playRingButton->setEnabled(false);
    m_ui->saveSongButton->setEnabled(false);
    m_ui->cutSliderWidget->resizeGeometry(440, 55);

    connect(m_ui->addSongButton, SIGNAL(clicked()), SLOT(initInputPath()));
    connect(m_ui->playSongButton, SIGNAL(clicked()), SLOT(playInputSong()));
    connect(m_ui->playRingButton, SIGNAL(clicked()), SLOT(playRingtone()));
    connect(m_ui->saveSongButton, SIGNAL(clicked()), SLOT(initOutputPath()));
    connect(m_ui->cutSliderWidget, SIGNAL(posChanged(qint64,qint64)), SLOT(posChanged(qint64,qint64)));
    connect(m_ui->cutSliderWidget, SIGNAL(buttonReleaseChanged(qint64)), SLOT(buttonReleaseChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
}

MusicSongRingtoneMaker::~MusicSongRingtoneMaker()
{
    delete m_player;
    delete m_ui;
}

void MusicSongRingtoneMaker::initInputPath()
{
    QStringList supportedFormat;
    supportedFormat << "mp3" << "wav" << "wma" << "ogg" << "flac" << "ac3" << "aac";

    QString filter = "Audio Files (";
    for(int i = 0; i < supportedFormat.count(); ++i)
    {
        filter += QString("*.%1 ").arg(supportedFormat[i]);
    }
    filter = filter.trimmed() + ")";

    const QString &path = TTK::File::getOpenFileName(this, filter);
    if(path.isEmpty())
    {
        return;
    }

    m_inputFilePath = path;
    MusicSongMeta meta;
    if(meta.read(m_inputFilePath))
    {
        QString name = QFileInfo(m_inputFilePath).fileName();
        m_ui->songLabelValue->setToolTip(name);
        name = TTK::Widget::elidedText(font(), name, Qt::ElideRight, 220);
        m_ui->songLabelValue->setText(tr("SongName: %1 (%2, %3, %4)").arg(name, meta.duration(), meta.sampleRate(), meta.bitrate()));
    }
    else
    {
        m_ui->songLabelValue->setText(tr("Open file error"));
        return;
    }

    m_ui->playSongButton->setEnabled(true);
    m_ui->playRingButton->setEnabled(true);
    m_ui->saveSongButton->setEnabled(true);

    m_player->setMedia(MusicCoreMPlayer::Module::Music, m_inputFilePath);
    playInputSong();

}

void MusicSongRingtoneMaker::initOutputPath()
{
    QString value = QString("Files (*.%1)").arg(m_ui->formatCombo->currentText().toLower());
            value = TTK::File::getSaveFileName(this, value);
    if(value.isEmpty())
    {
        return;
    }

    QProcess::execute(MAKE_TRANSFORM_PATH_FULL, {
        "-i", m_inputFilePath, "-ss", QString::number(m_startPos),
        "-t", QString::number(m_stopPos), "-acodec", "copy",
        "-ab", m_ui->kbpsCombo->currentText() + "k",
        "-ar", m_ui->hzCombo->currentText(),
        "-ac", QString::number(m_ui->msCombo->currentIndex() + 1), value});
}

void MusicSongRingtoneMaker::playInputSong()
{
    m_playRingtone = false;
    playButtonStateChanged();
    m_player->play();
}

void MusicSongRingtoneMaker::playRingtone()
{
    if(!m_player->isPlaying())
    {
        m_ui->playSongButton->setText(tr("Pause"));
    }
    m_playRingtone = true;
    m_player->setPosition(m_startPos);
}

void MusicSongRingtoneMaker::positionChanged(qint64 position)
{
    m_ui->cutSliderWidget->setPosition(position);
    if(m_playRingtone && m_stopPos < position)
    {
        m_player->play();
        m_ui->playSongButton->setText(tr("Play"));
    }
}

void MusicSongRingtoneMaker::durationChanged(qint64 duration)
{
    m_ui->cutSliderWidget->setDuration(duration);
}

void MusicSongRingtoneMaker::posChanged(qint64 start, qint64 end)
{
    m_startPos = start;
    m_stopPos = end;
    m_ui->startTimeLabel->setText(tr("Begin: ") + TTKTime::toString(start, TTKTime::Entity::Second, "mm:ss:zzz"));
    m_ui->stopTimeLabel->setText(tr("End: ") + TTKTime::toString(end, TTKTime::Entity::Second, "mm:ss:zzz"));
    m_ui->ringLabelValue->setText(tr("Ring Info.\tCut Length: %1").arg(TTKTime::toString(end - start, TTKTime::Entity::Second, "mm:ss")));
}

void MusicSongRingtoneMaker::buttonReleaseChanged(qint64 pos)
{
    if(!m_player->isPlaying())
    {
        m_ui->playSongButton->setText(tr("Pause"));
    }
    m_player->setPosition(pos);
}

int MusicSongRingtoneMaker::exec()
{
    if(!QFile::exists(MAKE_TRANSFORM_PATH_FULL))
    {
        MusicToastLabel::popup(tr("Lack of plugin file"));
        return -1;
    }

    return MusicAbstractMoveDialog::exec();
}

void MusicSongRingtoneMaker::initialize() const
{
    m_ui->formatCombo->addItems({"MP3", "WAV"});
    m_ui->kbpsCombo->addItems({"32", "48", "56", "64", "80", "96", "112", "128", "192", "224", "256", "320"});
    m_ui->hzCombo->addItems({"8000", "12050", "16000", "22050", "24000", "32000", "44100", "48000"});
    m_ui->msCombo->addItems({"Mono", "Stereo"});

    m_ui->kbpsCombo->setCurrentIndex(7);
    m_ui->hzCombo->setCurrentIndex(6);
}

void MusicSongRingtoneMaker::playButtonStateChanged()
{
    if(!m_player->isPlaying())
    {
        m_ui->playSongButton->setText(tr("Pause"));
    }
    else
    {
        m_ui->playSongButton->setText(tr("Play"));
    }
}
