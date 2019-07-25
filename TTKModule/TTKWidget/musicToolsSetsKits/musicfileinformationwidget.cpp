#include "musicfileinformationwidget.h"
#include "ui_musicfileinformationwidget.h"
#include "musicuiobject.h"
#include "musicurlutils.h"
#include "musicnumberutils.h"
#include "musicsongtag.h"
#include "musicmessagebox.h"
#include "musicfileutils.h"

#define ADVANCE_OFFSET  150

MusicFileInformationWidget::MusicFileInformationWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicFileInformationWidget)
{
    m_ui->setupUi(this);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    setStyleSheet(MusicUIObject::MLineEditStyle01);
    setEditLineEnable(false);
    m_advanceOn = false;
    musicAdvanceClicked();

    QPixmap pix;
    pix.load(":/image/lb_defaultArt");
    m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));

    m_ui->editButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->saveButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->openPixButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->editButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openPixButton->setFocusPolicy(Qt::NoFocus);
    m_ui->viewButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->editButton, SIGNAL(clicked()), SLOT(musicEditTag()));
    connect(m_ui->saveButton, SIGNAL(clicked()), SLOT(musicSaveTag()));
    connect(m_ui->viewButton, SIGNAL(clicked()), SLOT(musicOpenFileDir()));
    connect(m_ui->advanceLabel, SIGNAL(clicked()), SLOT(musicAdvanceClicked()));
    connect(m_ui->openPixButton, SIGNAL(clicked()), SLOT(musicOpenImageFileDir()));
}

MusicFileInformationWidget::~MusicFileInformationWidget()
{
    delete m_ui;
}

void MusicFileInformationWidget::musicOpenFileDir()
{
    if(!MusicUtils::Url::openUrl(QFileInfo(m_path).absoluteFilePath()))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
    }
}

void MusicFileInformationWidget::musicOpenImageFileDir()
{
    m_imagePath = MusicUtils::File::getOpenFileDialog(this);
    if(m_imagePath.isEmpty())
    {
        return;
    }

    QPixmap pix;
    pix.load(m_imagePath);
    m_ui->pixmapSizeLabel->setText(QString("%1x%2").arg(pix.width()).arg(pix.height()));
    m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));
}

void MusicFileInformationWidget::musicAdvanceClicked()
{
    if(m_advanceOn)
    {
        setFixedHeight(420 + ADVANCE_OFFSET);
        m_ui->background->setFixedHeight(412 + ADVANCE_OFFSET);
        m_ui->backgroundMask->setFixedHeight(387 + ADVANCE_OFFSET);
        m_ui->advanceLabel->move(29, 385 + ADVANCE_OFFSET);
        m_ui->editButton->move(310, 355 + ADVANCE_OFFSET);
        m_ui->saveButton->move(390, 355 + ADVANCE_OFFSET);
        m_ui->pixmapLabel->setVisible(true);
        m_ui->label_17->setVisible(true);
        m_ui->decoderLabel->setVisible(true);

        QPixmap pix;
        MusicSongTag tag;
        if(tag.read(m_path))
        {
            pix = tag.getCover();
        }

        QString text = QString("%1x%2").arg(pix.width()).arg(pix.height());
        if(pix.isNull())
        {
            text = "-";
            pix.load(":/image/lb_defaultArt");
        }
        m_ui->pixmapSizeLabel->setText(text);
        m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));
    }
    else
    {
        setFixedHeight(420);
        m_ui->background->setFixedHeight(412);
        m_ui->backgroundMask->setFixedHeight(387);
        m_ui->advanceLabel->move(29, 385);
        m_ui->editButton->move(310, 355);
        m_ui->saveButton->move(390, 355);
        m_ui->pixmapLabel->setVisible(false);
        m_ui->label_17->setVisible(false);
        m_ui->decoderLabel->setVisible(false);
    }

    m_advanceOn = !m_advanceOn;
    setBackgroundPixmap(m_ui->background, size());
}

void MusicFileInformationWidget::musicEditTag()
{
    setEditLineEnable(!m_ui->fileAlbumEdit->isEnabled());
}

void MusicFileInformationWidget::musicSaveTag()
{
    MusicSongTag tag;
    if(!tag.read(m_path))
    {
        return;
    }

    QString value = m_ui->fileAlbumEdit->text().trimmed();
    if(value != "-")
    {
        tag.setAlbum(value);
    }

    value = m_ui->fileArtistEdit->text().trimmed();
    if(value != "-")
    {
        tag.setArtist(value);
    }

    value = m_ui->fileGenreEdit->text().trimmed();
    if(value != "-")
    {
        tag.setGenre(value);
    }

    value = m_ui->fileTitleEdit->text().trimmed();
    if(value != "-")
    {
        tag.setTitle(value);
    }

    value = m_ui->fileYearEdit->text().trimmed();
    if(value != "-")
    {
        tag.setYear(value);
    }

    if(!m_imagePath.isEmpty())
    {
        tag.setCover(QPixmap(m_imagePath));
    }

    tag.save();

    MusicMessageBox message;
    message.setText(tr("Save Successfully!"));
    message.exec();
}

void MusicFileInformationWidget::setFileInformation(const QString &name)
{
    if(name.contains(CACHE_DIR_FULL))
    {
        m_ui->viewButton->setEnabled(false);
    }
    //cache song should not allow open url

    MusicSongTag tag;
    const bool state = tag.read(m_path = name);
    const QFileInfo fin(name);
    QString check;
    m_ui->filePathEdit->setText( (check = name).isEmpty() ? "-" : check );
    m_ui->fileFormatEdit->setText( (check = fin.suffix() ).isEmpty() ? "-" : check );
    m_ui->fileSizeEdit->setText( (check = MusicUtils::Number::size2Label(fin.size()) ).isEmpty() ? "-" : check );

    m_ui->fileAlbumEdit->setText( state ? ((check = tag.getAlbum()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileArtistEdit->setText( state ? ((check = tag.getArtist()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileGenreEdit->setText( state ? ((check = tag.getGenre()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileTitleEdit->setText( state ? ((check = tag.getTitle()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileYearEdit->setText( state ? ((check = tag.getYear()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileTimeEdit->setText( state ? ((check = tag.getLengthString()).isEmpty() ? "-" : check) : "-" );

    m_ui->BitrateEdit->setText( state ? ((check = (tag.getBitrate())).isEmpty() ? "-" : check) : "-" );
    m_ui->ChannelEdit->setText( state ? ((check = tag.getChannel()).isEmpty() ? "-" : check) : "-" );
    m_ui->SamplingRateEdit->setText( state ? ((check = tag.getSampleRate()).isEmpty() ? "-" : check) : "-" );
    m_ui->TrackNumEdit->setText( state ? ((check = tag.getTrackNum()).isEmpty() ? "-" : check) : "-" );
    m_ui->decoderLabel->setText( state ? ((check = tag.getDecoder()).isEmpty() ? "-" : check.toUpper()) : "-" );
    m_ui->qualityEdit->setText( MusicUtils::Number::transfromBitrateToQuality(MusicUtils::Number::transfromBitrateToLevel(m_ui->BitrateEdit->text())) );
}

void MusicFileInformationWidget::setEditLineEnable(bool enable)
{
    m_ui->fileAlbumEdit->setEnabled(enable);
    m_ui->fileArtistEdit->setEnabled(enable);
    m_ui->fileGenreEdit->setEnabled(enable);
    m_ui->fileTitleEdit->setEnabled(enable);
    m_ui->fileYearEdit->setEnabled(enable);

    m_ui->saveButton->setEnabled(enable);
    m_ui->openPixButton->setEnabled(enable);
}

int MusicFileInformationWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
