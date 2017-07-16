#include "musicfileinformationwidget.h"
#include "ui_musicfileinformationwidget.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#include "musicnumberutils.h"
#include "musicsongtag.h"
#include "musicmessagebox.h"

#include <QBuffer>
#include <QFileDialog>
#include <QStyledItemDelegate>

#define ADVANCE_OFFSET  150

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
    setEditLineEnable(false);
    m_advanceOn = false;
    musicAdvanceClicked();

    QPixmap pix;
    pix.load(":/image/lb_defaultArt");
    m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));
    m_ui->idv3ComboBox->addItems(QStringList() << "ID3v2.3" << "ID3v2.4");

    m_ui->editButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->saveButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->viewButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->openPixButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);

    m_ui->idv3ComboBox->setItemDelegate(new QStyledItemDelegate(m_ui->idv3ComboBox));
    m_ui->idv3ComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    m_ui->idv3ComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);

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

void MusicFileInformationWidget::musicOpenImageFileDir()
{
    m_imagePath = QFileDialog::getOpenFileName(
                  this, QString(), "./", "Images (*.png *.bmp *.jpg)");
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
        setFixedHeight(385 + ADVANCE_OFFSET);
        m_ui->background->setFixedHeight(377 + ADVANCE_OFFSET);
        m_ui->backgroundMask->setFixedHeight(352 + ADVANCE_OFFSET);
        m_ui->advanceLabel->move(29, 350 + ADVANCE_OFFSET);
        m_ui->editButton->move(310, 320 + ADVANCE_OFFSET);
        m_ui->saveButton->move(390, 320 + ADVANCE_OFFSET);
        m_ui->pixmapLabel->setVisible(true);
        m_ui->label_17->setVisible(true);
        m_ui->idv3ComboBox->setVisible(true);

        MusicSongTag tag;
        if(!tag.readFile(m_path))
        {
            return;
        }
        QPixmap pix;
        pix.loadFromData(tag.getCover());
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
        setFixedHeight(385);
        m_ui->background->setFixedHeight(377);
        m_ui->backgroundMask->setFixedHeight(352);
        m_ui->advanceLabel->move(29, 350);
        m_ui->editButton->move(310, 320);
        m_ui->saveButton->move(390, 320);
        m_ui->pixmapLabel->setVisible(false);
        m_ui->label_17->setVisible(false);
        m_ui->idv3ComboBox->setVisible(false);
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
    if(!tag.readFile(m_path))
    {
        return;
    }
    tag.setTagVersion(m_ui->idv3ComboBox->currentIndex() == 0 ? 3 : 4);

    QString value = m_ui->fileAlbumEdit->text().trimmed();
    if(value != "-" && m_ui->fileAlbumEdit->getTextEdited())
    {
        tag.setAlbum(value);
    }

    value = m_ui->fileArtistEdit->text().trimmed();
    if(value != "-" && m_ui->fileArtistEdit->getTextEdited())
    {
        tag.setArtist(value);
    }

    value = m_ui->fileGenreEdit->text().trimmed();
    if(value != "-" && m_ui->fileGenreEdit->getTextEdited())
    {
        tag.setGenre(value);
    }

    value = m_ui->fileTitleEdit->text().trimmed();
    if(value != "-" && m_ui->fileTitleEdit->getTextEdited())
    {
        tag.setTitle(value);
    }

    value = m_ui->fileYearEdit->text().trimmed();
    if(value != "-" && m_ui->fileYearEdit->getTextEdited())
    {
        tag.setYear(value);
    }

    if(!m_imagePath.isEmpty())
    {
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        QPixmap(m_imagePath).save(&buffer, "jpg");
        tag.setCover(data);
    }

    MusicMessageBox message;
    message.setText(tr("Save Successfully!"));
    message.exec();
}

void MusicFileInformationWidget::setFileInformation(const QString &name)
{
    if(name.contains('='))
    {
        m_ui->viewButton->setEnabled(false);
    }
    //cache song should not allow open url

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
