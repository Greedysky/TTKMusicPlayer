#include "musicfileinformationwidget.h"
#include "ui_musicfileinformationwidget.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#include "musicnumberutils.h"
#include "musicsongtag.h"
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

QString MusicModifyLineEdit::getClassName()
{
    return staticMetaObject.className();
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
      m_ui(new Ui::MusicFileInformationWidget)
{
    m_ui->setupUi(this);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    setStyleSheet(MusicUIObject::MLineEditStyle01);

    m_ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->viewButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_ui->viewButton, SIGNAL(clicked()), SLOT(musicOpenFileDir()));
}

MusicFileInformationWidget::~MusicFileInformationWidget()
{
    saveModifyData();
    delete m_ui;
}

QString MusicFileInformationWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicFileInformationWidget::musicOpenFileDir()
{
    if(!MusicUtils::Core::openUrl(QFileInfo(m_path).absoluteFilePath()))
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
    m_ui->filePathEdit->setText( (check = name).isEmpty() ? "-" : check );
    m_ui->fileFormatEdit->setText( (check = fin.suffix() ).isEmpty() ? "-" : check );
    m_ui->fileSizeEdit->setText( (check = MusicUtils::Number::size2Label(fin.size()) )
                                .isEmpty() ? "-" : check );

    m_ui->fileAlbumEdit->setText( state ? ((check = tag.getAlbum()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileArtistEdit->setText( state ? ((check = tag.getArtist()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileGenreEdit->setText( state ? ((check = tag.getGenre()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileTitleEdit->setText( state ? ((check = tag.getTitle()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileYearEdit->setText( state ? ((check = tag.getYear()).isEmpty() ? "-" : check) : "-" );
    m_ui->fileTimeEdit->setText( state ? ((check = tag.getLengthString()).isEmpty() ? "-" : check) : "-" );

    m_ui->BitrateEdit->setText( state ? ((check = (tag.getBitrate()))
                              .isEmpty() ? "-" : check) : "-" );
    m_ui->ChannelEdit->setText( state ? ((check = tag.getChannel())
                                    .isEmpty() ? "-" : check) : "-" );
    m_ui->SamplingRateEdit->setText( state ? ((check = tag.getSamplingRate())
                                   .isEmpty() ? "-" : check) : "-" );
    m_ui->TrackNumEdit->setText( state ? ((check = tag.getTrackNum()).isEmpty() ? "-" : check) : "-" );
    m_ui->descriptionEdit->setText( state ? ((check = QString("%1 %2").arg(tag.getFormat())
                                   .arg(tag.getMode())).isEmpty() ? "-" : check) : "-" );
}

void MusicFileInformationWidget::saveModifyData()
{
    MusicSongTag tag;
    if(!tag.readFile(m_path))
    {
        return;
    }

    QString value = m_ui->fileAlbumEdit->text().trimmed();
    if(value != "-" && m_ui->fileAlbumEdit->getTextEdited()) tag.setAlbum(value);

    value = m_ui->fileArtistEdit->text().trimmed();
    if(value != "-" && m_ui->fileArtistEdit->getTextEdited()) tag.setArtist(value);

    value = m_ui->fileGenreEdit->text().trimmed();
    if(value != "-" && m_ui->fileGenreEdit->getTextEdited()) tag.setGenre(value);

    value = m_ui->fileTitleEdit->text().trimmed();
    if(value != "-" && m_ui->fileTitleEdit->getTextEdited()) tag.setTitle(value);

    value = m_ui->fileYearEdit->text().trimmed();
    if(value != "-" && m_ui->fileYearEdit->getTextEdited()) tag.setYear(value);
}

int MusicFileInformationWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
