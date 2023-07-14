#include "musicfileinformationwidget.h"
#include "ui_musicfileinformationwidget.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"
#include "musicmessagebox.h"
#include "musicsong.h"

MusicFileInformationWidget::MusicFileInformationWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicFileInformationWidget),
      m_advanceOn(false),
      m_deleteOn(false)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);
    
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    setStyleSheet(TTK::UI::LineEditStyle01);
    setEditLineEnabled(false);

    advanceClicked();

    const QPixmap pix(":/image/lb_default_art");
    m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));

    m_ui->editButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->deletePixButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->savePixButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->saveButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->viewButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->openPixButton->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->editButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deletePixButton->setFocusPolicy(Qt::NoFocus);
    m_ui->savePixButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openPixButton->setFocusPolicy(Qt::NoFocus);
    m_ui->viewButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->editButton, SIGNAL(clicked()), SLOT(editTag()));
    connect(m_ui->deletePixButton, SIGNAL(clicked()), SLOT(deleteAlbumPicture()));
    connect(m_ui->savePixButton, SIGNAL(clicked()), SLOT(saveAlbumPicture()));
    connect(m_ui->saveButton, SIGNAL(clicked()), SLOT(saveTag()));
    connect(m_ui->viewButton, SIGNAL(clicked()), SLOT(openFileDir()));
    connect(m_ui->advanceLabel, SIGNAL(clicked()), SLOT(advanceClicked()));
    connect(m_ui->openPixButton, SIGNAL(clicked()), SLOT(openImageFileDir()));
}

MusicFileInformationWidget::~MusicFileInformationWidget()
{
    delete m_ui;
}

void MusicFileInformationWidget::openFileDir()
{
    if(!TTK::Url::openUrl(TTK::trackRelatedPath(m_path)))
    {
        MusicToastLabel::popup(tr("The file has been moved or does not exist"));
    }
}

void MusicFileInformationWidget::openImageFileDir()
{
    m_imagePath = TTK::File::getOpenFileName(this);
    if(m_imagePath.isEmpty())
    {
        return;
    }

    QPixmap pix;
    pix.load(m_imagePath);
    m_ui->pixmapSizeLabel->setText(QString("%1x%2").arg(pix.width()).arg(pix.height()));
    m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));
}

void MusicFileInformationWidget::advanceClicked()
{
    if(m_advanceOn)
    {
        setFixedHeight(420 + 150);
        m_ui->background->setFixedHeight(412 + 150);
        m_ui->backgroundMask->setFixedHeight(387 + 150);
        m_ui->advanceLabel->move(29, 380 + 150);
        m_ui->editButton->move(360, 345 + 150);
        m_ui->saveButton->move(440, 345 + 150);
        m_ui->pixmapLabel->setVisible(true);
        m_ui->label_17->setVisible(true);
        m_ui->decoderLabel->setVisible(true);

        QPixmap pix;
        MusicSongMeta meta;
        if(meta.read(m_path))
        {
            pix = meta.cover();
        }

        QString text = QString("%1x%2").arg(pix.width()).arg(pix.height());
        if(pix.isNull())
        {
            text = TTK_DEFAULT_STR;
            pix.load(":/image/lb_default_art");
        }

        m_ui->pixmapSizeLabel->setText(text);
        m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));
    }
    else
    {
        setFixedHeight(420);
        m_ui->background->setFixedHeight(412);
        m_ui->backgroundMask->setFixedHeight(387);
        m_ui->advanceLabel->move(29, 380);
        m_ui->editButton->move(360, 345);
        m_ui->saveButton->move(440, 345);
        m_ui->pixmapLabel->setVisible(false);
        m_ui->label_17->setVisible(false);
        m_ui->decoderLabel->setVisible(false);
    }

    m_advanceOn = !m_advanceOn;
    setBackgroundPixmap(size());
}

void MusicFileInformationWidget::deleteAlbumPicture()
{
    const QPixmap pix(":/image/lb_default_art");
    m_ui->pixmapSizeLabel->setText(TTK_DEFAULT_STR);
    m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));
    m_deleteOn = true;
}

void MusicFileInformationWidget::saveAlbumPicture()
{
    QPixmap pix;
    MusicSongMeta meta;
    if(meta.read(m_path))
    {
        pix = meta.cover();
    }

    if(!pix.isNull())
    {
        const QString &path = TTK::File::getSaveFileName(this);
        if(!path.isEmpty())
        {
            pix.save(path);
        }
    }
}

void MusicFileInformationWidget::editTag()
{
    setEditLineEnabled(!m_ui->fileAlbumEdit->isEnabled());
}

void MusicFileInformationWidget::saveTag()
{
    MusicMessageBox message;
    message.setText(tr("Are you sure to save?"));
    if(!message.exec())
    {
       return;
    }

    MusicSongMeta meta;
    if(!meta.read(m_path))
    {
        return;
    }

    QString value = m_ui->fileAlbumEdit->text().trimmed();
    if(value != TTK_DEFAULT_STR)
    {
        meta.setAlbum(value);
    }

    value = m_ui->fileArtistEdit->text().trimmed();
    if(value != TTK_DEFAULT_STR)
    {
        meta.setArtist(value);
    }

    value = m_ui->fileGenreEdit->text().trimmed();
    if(value != TTK_DEFAULT_STR)
    {
        meta.setGenre(value);
    }

    value = m_ui->fileTitleEdit->text().trimmed();
    if(value != TTK_DEFAULT_STR)
    {
        meta.setTitle(value);
    }

    value = m_ui->fileYearEdit->text().trimmed();
    if(value != TTK_DEFAULT_STR)
    {
        meta.setYear(value);
    }

    if(m_deleteOn)
    {
        meta.setCover(QPixmap());
    }
    else if(!m_imagePath.isEmpty())
    {
        meta.setCover(QPixmap(m_imagePath));
    }

    meta.save();
    MusicToastLabel::popup(tr("Save successfully"));
}

void MusicFileInformationWidget::setFileInformation(const QString &name)
{
    if(name.contains(CACHE_DIR_FULL)) //cache song should not allow open url
    {
        m_ui->viewButton->setEnabled(false);
    }

    MusicSongMeta meta;
    const bool state = meta.read(m_path = name);
    const QFileInfo fin(meta.fileRelatedPath());

    QString check;
    m_ui->filePathEdit->setText((check = fin.filePath()).isEmpty() ? TTK_DEFAULT_STR : check);
    m_ui->fileFormatEdit->setText((check = TTK_FILE_SUFFIX(fin)).isEmpty() ? TTK_DEFAULT_STR : check);
    m_ui->fileSizeEdit->setText((check = TTK::Number::sizeByteToLabel(fin.size())).isEmpty() ? TTK_DEFAULT_STR : check);

    m_ui->fileAlbumEdit->setText(state ? ((check = meta.album()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->fileArtistEdit->setText(state ? ((check = meta.artist()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->fileGenreEdit->setText(state ? ((check = meta.genre()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->fileTitleEdit->setText(state ? ((check = meta.title()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->fileYearEdit->setText(state ? ((check = meta.year()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->fileTimeEdit->setText(state ? ((check = meta.duration()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);

    const int rating = meta.rating().toInt();
    if(rating == 0) m_ui->ratingLabel->setValue(0);
    else if(rating < 64) m_ui->ratingLabel->setValue(1);
    else if(rating < 128) m_ui->ratingLabel->setValue(2);
    else if(rating < 196) m_ui->ratingLabel->setValue(3);
    else if(rating < 255) m_ui->ratingLabel->setValue(4);
    else m_ui->ratingLabel->setValue(5);

    m_ui->bitrateEdit->setText(state ? ((check = (meta.bitrate())).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->channelEdit->setText(state ? ((check = meta.channel()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->sampleRateEdit->setText(state ? ((check = meta.sampleRate()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->trackNumEdit->setText(state ? ((check = meta.trackNum()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);
    m_ui->decoderLabel->setText(state ? ((check = meta.decoder()).isEmpty() ? TTK_DEFAULT_STR : check) : TTK_DEFAULT_STR);

    QColor color;
    QString bitrate;
    TTK::Number::bitrateToQuality(TTK::Number::bitrateToLevel(m_ui->bitrateEdit->text()), bitrate, color);
    m_ui->qualityEdit->setText(bitrate);
}

void MusicFileInformationWidget::setEditLineEnabled(bool enabled)
{
    m_ui->fileAlbumEdit->setEnabled(enabled);
    m_ui->fileArtistEdit->setEnabled(enabled);
    m_ui->fileGenreEdit->setEnabled(enabled);
    m_ui->fileTitleEdit->setEnabled(enabled);
    m_ui->fileYearEdit->setEnabled(enabled);

    m_ui->saveButton->setEnabled(enabled);
    m_ui->deletePixButton->setEnabled(enabled);

    m_ui->openPixButton->setEnabled(enabled);
}
