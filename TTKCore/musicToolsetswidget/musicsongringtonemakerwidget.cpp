#include "musicsongringtonemakerwidget.h"
#include "ui_musicsongringtonemakerwidget.h"
#include "musiccutsliderwidget.h"
#include "musiccoremplayer.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicsongtag.h"
#include "musicwidgetutils.h"
#include "musictime.h"

#include <QProcess>
#include <QFileDialog>
#include <QStyledItemDelegate>

MusicSongRingtoneMaker::MusicSongRingtoneMaker(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSongRingtoneMaker)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->addSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->playSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->playRingButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->saveSongButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->formatCombo->setItemDelegate(new QStyledItemDelegate(ui->formatCombo));
    ui->formatCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->formatCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->kbpsCombo->setItemDelegate(new QStyledItemDelegate(ui->kbpsCombo));
    ui->kbpsCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->kbpsCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->hzCombo->setItemDelegate(new QStyledItemDelegate(ui->hzCombo));
    ui->hzCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->hzCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->msCombo->setItemDelegate(new QStyledItemDelegate(ui->msCombo));
    ui->msCombo->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->msCombo->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    initControlParameter();

    m_startPos = 0;
    m_stopPos = 9999;
    m_playRingtone = false;

    ui->playSongButton->setEnabled(false);
    ui->playRingButton->setEnabled(false);
    ui->saveSongButton->setEnabled(false);
    ui->cutSliderWidget->resizeWindow(440, 55);
    m_player = new MusicCoreMPlayer(this);

    connect(ui->addSongButton, SIGNAL(clicked()), SLOT(initInputPath()));
    connect(ui->playSongButton, SIGNAL(clicked()), SLOT(playInputSong()));
    connect(ui->playRingButton, SIGNAL(clicked()), SLOT(playRingtone()));
    connect(ui->saveSongButton, SIGNAL(clicked()), SLOT(initOutputPath()));
    connect(ui->cutSliderWidget, SIGNAL(posChanged(qint64,qint64)), SLOT(posChanged(qint64,qint64)));
    connect(ui->cutSliderWidget, SIGNAL(buttonReleaseChanged(qint64)), SLOT(buttonReleaseChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
}

MusicSongRingtoneMaker::~MusicSongRingtoneMaker()
{
    delete m_player;
    delete ui;
}

QString MusicSongRingtoneMaker::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongRingtoneMaker::initInputPath()
{
    QStringList supportedFormat;
    supportedFormat << "mp3" <<"wav" <<"wma" << "ogg" << "flac" << "ac3" << "aac";

    QString filter = "Files (";
    for(int i=0; i<supportedFormat.count(); ++i)
    {
        filter += QString("*.%1 ").arg(supportedFormat[i]);
    }
    filter = filter.trimmed() + ")";

    QString path = QFileDialog::getOpenFileName(this, QString(), "./", filter);
    if(path.isEmpty())
    {
        return;
    }

    m_inputFilePath = path;
    MusicSongTag tag;
    if(tag.readFile(m_inputFilePath))
    {
        QString name = QFileInfo(m_inputFilePath).fileName();
        ui->songLabelValue->setToolTip( name );
        name = MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 220);
        ui->songLabelValue->setText(tr("SongName: %1 ( %2, %3, %4)").arg(name)
                .arg(tag.getLengthString()).arg(tag.getSamplingRate()).arg(tag.getBitrate()));
    }
    else
    {
        ui->songLabelValue->setText(tr("Open File Error!"));
        return;
    }

    ui->playSongButton->setEnabled(true);
    ui->playRingButton->setEnabled(true);
    ui->saveSongButton->setEnabled(true);

    m_player->setMedia(MusicCoreMPlayer::MusicCategory, m_inputFilePath);
    playInputSong();

}

void MusicSongRingtoneMaker::initOutputPath()
{
    QString value = QString("Files (*.%1 )").arg(ui->formatCombo->currentText().toLower());
    value = QFileDialog::getSaveFileName(this, QString(), "./", value);
    if(value.isEmpty())
    {
        return;
    }

    QProcess::execute(MAKE_TRANSFORM_FULL, QStringList()
        << "-i" << m_inputFilePath << "-ss" << QString::number(m_startPos)
        << "-t" << QString::number(m_stopPos) << "-acodec" << "copy"
        << "-ab" << ui->kbpsCombo->currentText() + "k"
        << "-ar" << ui->hzCombo->currentText()
        << "-ac" << QString::number(ui->msCombo->currentIndex() + 1) << value );
}

void MusicSongRingtoneMaker::playInputSong()
{
    m_playRingtone = false;
    playButtonStateChanged();
    m_player->play();
}

void MusicSongRingtoneMaker::playRingtone()
{
    if(m_player->state() == MusicCoreMPlayer::StoppedState ||
       m_player->state() == MusicCoreMPlayer::PausedState)
    {
        ui->playSongButton->setText(tr("Stop"));
    }
    m_playRingtone = true;
    m_player->setPosition(m_startPos);
}

void MusicSongRingtoneMaker::positionChanged(qint64 position)
{
    ui->cutSliderWidget->setPosition(position);
    if(m_playRingtone && m_stopPos < position)
    {
        m_player->play();
        ui->playSongButton->setText(tr("Play"));
    }
}

void MusicSongRingtoneMaker::durationChanged(qint64 duration)
{
    ui->cutSliderWidget->setDuration(duration);
}

void MusicSongRingtoneMaker::posChanged(qint64 start, qint64 end)
{
    m_startPos = start;
    m_stopPos = end;
    ui->startTimeLabel->setText( tr("Begin: ") + MusicTime::toString(start, MusicTime::All_Sec, "mm:ss:zzz") );
    ui->stopTimeLabel->setText( tr("End: ") + MusicTime::toString(end, MusicTime::All_Sec, "mm:ss:zzz") );
    ui->ringLabelValue->setText( tr("Ring Info.\tCut Length: %1")
                        .arg(MusicTime::toString(end - start, MusicTime::All_Sec, "mm:ss")));
}

void MusicSongRingtoneMaker::buttonReleaseChanged(qint64 pos)
{
    if(m_player->state() == MusicCoreMPlayer::StoppedState ||
       m_player->state() == MusicCoreMPlayer::PausedState)
    {
        ui->playSongButton->setText(tr("Stop"));
    }
    m_player->setPosition(pos);
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

    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSongRingtoneMaker::initControlParameter() const
{
    ui->formatCombo->addItems(QStringList() << "MP3" << "WAV");
    ui->kbpsCombo->addItems(QStringList() << "32" << "48" << "56" << "64" << "80"
                     << "96" << "112" << "128" << "192" << "224" << "256" << "320");
    ui->hzCombo->addItems(QStringList() << "8000" << "12050" << "16000" << "22050"
                            << "24000" << "32000" << "44100" << "48000");
    ui->msCombo->addItems(QStringList() << "Mono" << "Stereo");

    ui->kbpsCombo->setCurrentIndex(7);
    ui->hzCombo->setCurrentIndex(6);
}

void MusicSongRingtoneMaker::playButtonStateChanged()
{
    if(m_player->state() == MusicCoreMPlayer::StoppedState ||
       m_player->state() == MusicCoreMPlayer::PausedState)
    {
        ui->playSongButton->setText(tr("Stop"));
    }
    else
    {
        ui->playSongButton->setText(tr("Play"));
    }
}
