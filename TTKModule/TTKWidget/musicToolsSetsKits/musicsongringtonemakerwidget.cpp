#include "musicsongringtonemakerwidget.h"
#include "ui_musicsongringtonemakerwidget.h"
#include "musiccutsliderwidget.h"
#include "musiccoremplayer.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicsongtag.h"
#include "musicfileutils.h"
#include "musicwidgetutils.h"
#include "musictime.h"

#include <QProcess>
#include <QAbstractItemView>
#include <QStyledItemDelegate>

MusicSongRingtoneMaker::MusicSongRingtoneMaker(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongRingtoneMaker)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->addSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->playSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->playRingButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->saveSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->formatCombo->setItemDelegate(new QStyledItemDelegate(m_ui->formatCombo));
    m_ui->formatCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->formatCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->kbpsCombo->setItemDelegate(new QStyledItemDelegate(m_ui->kbpsCombo));
    m_ui->kbpsCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->kbpsCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->hzCombo->setItemDelegate(new QStyledItemDelegate(m_ui->hzCombo));
    m_ui->hzCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->hzCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    m_ui->msCombo->setItemDelegate(new QStyledItemDelegate(m_ui->msCombo));
    m_ui->msCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->msCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

#ifdef Q_OS_UNIX
    m_ui->addSongButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playSongButton->setFocusPolicy(Qt::NoFocus);
    m_ui->playRingButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveSongButton->setFocusPolicy(Qt::NoFocus);
#endif

    initControlParameter();

    m_startPos = 0;
    m_stopPos = DEFAULT_LEVEL_HIGHER;
    m_playRingtone = false;

    m_ui->playSongButton->setEnabled(false);
    m_ui->playRingButton->setEnabled(false);
    m_ui->saveSongButton->setEnabled(false);
    m_ui->cutSliderWidget->resizeWindow(440, 55);
    m_mediaPlayer = new MusicCoreMPlayer(this);

    connect(m_ui->addSongButton, SIGNAL(clicked()), SLOT(initInputPath()));
    connect(m_ui->playSongButton, SIGNAL(clicked()), SLOT(playInputSong()));
    connect(m_ui->playRingButton, SIGNAL(clicked()), SLOT(playRingtone()));
    connect(m_ui->saveSongButton, SIGNAL(clicked()), SLOT(initOutputPath()));
    connect(m_ui->cutSliderWidget, SIGNAL(posChanged(qint64,qint64)), SLOT(posChanged(qint64,qint64)));
    connect(m_ui->cutSliderWidget, SIGNAL(buttonReleaseChanged(qint64)), SLOT(buttonReleaseChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
}

MusicSongRingtoneMaker::~MusicSongRingtoneMaker()
{
    delete m_mediaPlayer;
    delete m_ui;
}

void MusicSongRingtoneMaker::initInputPath()
{
    QStringList supportedFormat;
    supportedFormat << "mp3" << "wav" << "wma" << "ogg" << "flac" << "ac3" << "aac";

    QString filter = "Files (";
    for(int i=0; i<supportedFormat.count(); ++i)
    {
        filter += QString("*.%1 ").arg(supportedFormat[i]);
    }
    filter = filter.trimmed() + ")";

    const QString &path = MusicUtils::File::getOpenFileDialog(this, filter);
    if(path.isEmpty())
    {
        return;
    }

    m_inputFilePath = path;
    MusicSongTag tag;
    if(tag.read(m_inputFilePath))
    {
        QString name = QFileInfo(m_inputFilePath).fileName();
        m_ui->songLabelValue->setToolTip( name );
        name = MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 220);
        m_ui->songLabelValue->setText(tr("SongName: %1 ( %2, %3, %4)").arg(name).arg(tag.getLengthString()).arg(tag.getSampleRate()).arg(tag.getBitrate()));
    }
    else
    {
        m_ui->songLabelValue->setText(tr("Open File Error!"));
        return;
    }

    m_ui->playSongButton->setEnabled(true);
    m_ui->playRingButton->setEnabled(true);
    m_ui->saveSongButton->setEnabled(true);

    m_mediaPlayer->setMedia(MusicCoreMPlayer::MusicCategory, m_inputFilePath);
    playInputSong();

}

void MusicSongRingtoneMaker::initOutputPath()
{
    QString value = QString("Files (*.%1 )").arg(m_ui->formatCombo->currentText().toLower());
            value = MusicUtils::File::getSaveFileDialog(this, value);
    if(value.isEmpty())
    {
        return;
    }

    QProcess::execute(MAKE_TRANSFORM_FULL, QStringList()
        << "-i" << m_inputFilePath << "-ss" << QString::number(m_startPos)
        << "-t" << QString::number(m_stopPos) << "-acodec" << "copy"
        << "-ab" << m_ui->kbpsCombo->currentText() + "k"
        << "-ar" << m_ui->hzCombo->currentText()
        << "-ac" << QString::number(m_ui->msCombo->currentIndex() + 1) << value );
}

void MusicSongRingtoneMaker::playInputSong()
{
    m_playRingtone = false;
    playButtonStateChanged();
    m_mediaPlayer->play();
}

void MusicSongRingtoneMaker::playRingtone()
{
    if(!m_mediaPlayer->isPlaying())
    {
        m_ui->playSongButton->setText(tr("Stop"));
    }
    m_playRingtone = true;
    m_mediaPlayer->setPosition(m_startPos);
}

void MusicSongRingtoneMaker::positionChanged(qint64 position)
{
    m_ui->cutSliderWidget->setPosition(position);
    if(m_playRingtone && m_stopPos < position)
    {
        m_mediaPlayer->play();
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
    m_ui->startTimeLabel->setText( tr("Begin: ") + MusicTime::toString(start, MusicTime::All_Sec, "mm:ss:zzz") );
    m_ui->stopTimeLabel->setText( tr("End: ") + MusicTime::toString(end, MusicTime::All_Sec, "mm:ss:zzz") );
    m_ui->ringLabelValue->setText( tr("Ring Info.\tCut Length: %1").arg(MusicTime::toString(end - start, MusicTime::All_Sec, "mm:ss")));
}

void MusicSongRingtoneMaker::buttonReleaseChanged(qint64 pos)
{
    if(!m_mediaPlayer->isPlaying())
    {
        m_ui->playSongButton->setText(tr("Stop"));
    }
    m_mediaPlayer->setPosition(pos);
}

int MusicSongRingtoneMaker::exec()
{
    if(!QFile::exists(MAKE_TRANSFORM_FULL))
    {
        MusicMessageBox message;
        message.setText(tr("Lack of plugin file!"));
        message.exec();
        return -1;
    }

    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSongRingtoneMaker::initControlParameter() const
{
    m_ui->formatCombo->addItems(QStringList() << "MP3" << "WAV");
    m_ui->kbpsCombo->addItems(QStringList() << "32" << "48" << "56" << "64" << "80"
                            << "96" << "112" << "128" << "192" << "224" << "256" << "320");
    m_ui->hzCombo->addItems(QStringList() << "8000" << "12050" << "16000" << "22050"
                            << "24000" << "32000" << "44100" << "48000");
    m_ui->msCombo->addItems(QStringList() << "Mono" << "Stereo");

    m_ui->kbpsCombo->setCurrentIndex(7);
    m_ui->hzCombo->setCurrentIndex(6);
}

void MusicSongRingtoneMaker::playButtonStateChanged()
{
    if(!m_mediaPlayer->isPlaying())
    {
        m_ui->playSongButton->setText(tr("Stop"));
    }
    else
    {
        m_ui->playSongButton->setText(tr("Play"));
    }
}
