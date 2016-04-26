#include "musicfileinformationwidget.h"
#include "ui_musicfileinformationwidget.h"
#include "musicuiobject.h"
#include "musicutils.h"
#include "musicsongtag.h"
#include "musicbackgroundmanager.h"
#include "musicmessagebox.h"

MusicModifyLineEdit::MusicModifyLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    m_isTextEdited = false;
    connect(this, SIGNAL(textEdited(QString)), SLOT(isTextEdited()));
}

MusicModifyLineEdit::~MusicModifyLineEdit()
{

}

void MusicModifyLineEdit::isTextEdited()
{
    m_isTextEdited = true;
}

void MusicModifyLineEdit::leaveEvent(QEvent *event)
{
    QLineEdit::leaveEvent(event);
    setReadOnly(true);
}

void MusicModifyLineEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    QLineEdit::mouseDoubleClickEvent(event);
    setReadOnly(false);
}


MusicFileInformationWidget::MusicFileInformationWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicFileInformationWidget)
{
    ui->setupUi(this);
    
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->viewButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(ui->viewButton, SIGNAL(clicked()), SLOT(musicOpenFileDir()));
}

MusicFileInformationWidget::~MusicFileInformationWidget()
{
    saveModifyData();
    delete ui;
}

void MusicFileInformationWidget::musicOpenFileDir()
{
    if(!MusicUtils::openUrl(QFileInfo(m_path).absoluteFilePath()))
    {
        MusicMessageBox message;
        message.setText(tr("The origin one does not exist!"));
        message.exec();
    }
}

void MusicFileInformationWidget::setFileInformation(const QString &name)
{
    MusicSongTag tag;
    bool state = tag.readFile(m_path = name);
    QFileInfo fin(name);
    QString check;
    ui->filePathEdit->setText( (check = name).isEmpty() ? "-" : check );
    ui->fileFormatEdit->setText( (check = fin.suffix() ).isEmpty() ? "-" : check );
    ui->fileSizeEdit->setText( (check = MusicUtils::size2Label(fin.size()) )
                                .isEmpty() ? "-" : check );

    ui->fileAlbumEdit->setText( state ? ((check = tag.getAlbum()).isEmpty() ? "-" : check) : "-" );
    ui->fileArtistEdit->setText( state ? ((check = tag.getArtist()).isEmpty() ? "-" : check) : "-" );
    ui->fileGenreEdit->setText( state ? ((check = tag.getGenre()).isEmpty() ? "-" : check) : "-" );
    ui->fileTitleEdit->setText( state ? ((check = tag.getTitle()).isEmpty() ? "-" : check) : "-" );
    ui->fileYearEdit->setText( state ? ((check = tag.getYear()).isEmpty() ? "-" : check) : "-" );
    ui->fileTimeEdit->setText( state ? ((check = tag.getLengthString()).isEmpty() ? "-" : check) : "-" );

    ui->BitrateEdit->setText( state ? ((check = (tag.getBitrate()))
                              .isEmpty() ? "-" : check) : "-" );
    ui->ChannelEdit->setText( state ? ((check = tag.getChannel())
                                    .isEmpty() ? "-" : check) : "-" );
    ui->SamplingRateEdit->setText( state ? ((check = tag.getSamplingRate())
                                   .isEmpty() ? "-" : check) : "-" );
    ui->TrackNumEdit->setText( state ? ((check = tag.getTrackNum()).isEmpty() ? "-" : check) : "-" );
    ui->descriptionEdit->setText( state ? ((check = QString("%1 %2").arg(tag.getFormat())
                                   .arg(tag.getMode())).isEmpty() ? "-" : check) : "-" );
}

void MusicFileInformationWidget::saveModifyData()
{
    MusicSongTag tag;
    if(!tag.readFile(m_path))
    {
        return;
    }

    QString value = ui->fileAlbumEdit->text().trimmed();
    if(value != "-" && ui->fileAlbumEdit->getTextEdited()) tag.setAlbum(value);

    value = ui->fileArtistEdit->text().trimmed();
    if(value != "-" && ui->fileArtistEdit->getTextEdited()) tag.setArtist(value);

    value = ui->fileGenreEdit->text().trimmed();
    if(value != "-" && ui->fileGenreEdit->getTextEdited()) tag.setGenre(value);

    value = ui->fileTitleEdit->text().trimmed();
    if(value != "-" && ui->fileTitleEdit->getTextEdited()) tag.setTitle(value);

    value = ui->fileYearEdit->text().trimmed();
    if(value != "-" && ui->fileYearEdit->getTextEdited()) tag.setYear(value);
}

int MusicFileInformationWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
