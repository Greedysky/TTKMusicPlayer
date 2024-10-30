#include "musicfileinformationwidget.h"
#include "ui_musicfileinformationwidget.h"
#include "musicwycoverrequest.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musictoastlabel.h"
#include "musicfileutils.h"
#include "musicmessagebox.h"
#include "musiccoremplayer.h"
#include "musicsong.h"

MusicFileInformationWidget::MusicFileInformationWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicFileInformationWidget),
      m_deleteImage(false),
      m_player(nullptr)
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

    const QPixmap pix(":/image/lb_default_art");
    m_ui->pixmapLabel->setPixmap(pix.scaled(m_ui->pixmapLabel->size()));
    m_ui->mainViewWidget->setStyleSheet(TTK::UI::TabWidgetStyle01);
    m_ui->descriptionPlainEdit->setReadOnly(true);
    m_ui->descriptionPlainEdit->verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle03);

    m_ui->editButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->deletePixButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->savePixButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->saveButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->viewButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->openPixButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->dynamicPixButton->setStyleSheet(TTK::UI::PushButtonStyle04);

#ifdef Q_OS_UNIX
    m_ui->editButton->setFocusPolicy(Qt::NoFocus);
    m_ui->deletePixButton->setFocusPolicy(Qt::NoFocus);
    m_ui->savePixButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveButton->setFocusPolicy(Qt::NoFocus);
    m_ui->viewButton->setFocusPolicy(Qt::NoFocus);
    m_ui->openPixButton->setFocusPolicy(Qt::NoFocus);
    m_ui->dynamicPixButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->editButton, SIGNAL(clicked()), SLOT(editTag()));
    connect(m_ui->deletePixButton, SIGNAL(clicked()), SLOT(deleteAlbumImage()));
    connect(m_ui->savePixButton, SIGNAL(clicked()), SLOT(saveAlbumImage()));
    connect(m_ui->saveButton, SIGNAL(clicked()), SLOT(saveTag()));
    connect(m_ui->viewButton, SIGNAL(clicked()), SLOT(openFileDir()));
    connect(m_ui->openPixButton, SIGNAL(clicked()), SLOT(openImageFileDir()));
    connect(m_ui->dynamicPixButton, SIGNAL(clicked()), SLOT(openDynamicImage()));
}

MusicFileInformationWidget::~MusicFileInformationWidget()
{
    delete m_ui;
    delete m_player;
}

void MusicFileInformationWidget::openFileDir()
{
    if(!TTK::Url::openUrl(TTK::trackRelatedPath(m_path)))
    {
        MusicToastLabel::popup(tr("The file has been moved or does not exist"));
    }
}

static void rendererPixmap(Ui::MusicFileInformationWidget *ui, const QPixmap &pixmap)
{
    if(pixmap.isNull())
    {
        ui->pixmapWidthLabel->setText(TTK_DEFAULT_STR);
        ui->pixmapHeightLabel->setText(TTK_DEFAULT_STR);
    }
    else
    {
        ui->pixmapWidthLabel->setText(QString("%1px").arg(pixmap.width()));
        ui->pixmapHeightLabel->setText(QString("%1px").arg(pixmap.height()));
        ui->pixmapLabel->setPixmap(pixmap.scaled(ui->pixmapLabel->size()));
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
    rendererPixmap(m_ui, pix);
}

void MusicFileInformationWidget::deleteAlbumImage()
{
    m_deleteImage = true;
    rendererPixmap(m_ui, QPixmap(":/image/lb_default_art"));
}

void MusicFileInformationWidget::saveAlbumImage()
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

void MusicFileInformationWidget::openDynamicImage()
{
    if(m_ui->dynamicPixButton->text() == tr("Dynamic"))
    {
        m_ui->dynamicPixButton->setText(tr("Static"));

        MusicWYCoverSourceRequest *d = new MusicWYCoverSourceRequest(this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
        d->startToRequest(QFileInfo(m_path).baseName());
    }
    else
    {
        m_ui->dynamicPixButton->setText(tr("Dynamic"));

        if(m_player && m_player->isPlaying())
        {
            m_player->stop();
            delete m_player;
            m_player = nullptr;
        }

        MusicSongMeta meta;
        meta.read(m_path);
        rendererPixmap(m_ui, meta.cover());
    }
}

void MusicFileInformationWidget::downLoadFinished(const QString &bytes)
{
    if(bytes.isEmpty())
    {
        MusicToastLabel::popup(tr("No dynamic cover data"));
        m_ui->dynamicPixButton->setText(tr("Dynamic"));
        return;
    }

    if(!m_player)
    {
        m_player = new MusicCoreMPlayer(this);
    }

    m_player->setMedia(MusicCoreMPlayer::Module::Movie, bytes, m_ui->pixmapLabel->winId());
    m_player->play();
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

    if(m_deleteImage)
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

void MusicFileInformationWidget::initialize(const QString &name)
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

    QColor color;
    QString bitrate;
    TTK::Number::bitrateToQuality(TTK::Number::bitrateToLevel(m_ui->bitrateEdit->text()), bitrate, color);
    m_ui->qualityEdit->setText(bitrate);

    m_ui->descriptionPlainEdit->setPlainText(meta.description());

    rendererPixmap(m_ui, meta.cover());
}

void MusicFileInformationWidget::setEditLineEnabled(bool enabled)
{
    m_ui->fileAlbumEdit->setEnabled(enabled);
    m_ui->fileArtistEdit->setEnabled(enabled);
    m_ui->fileGenreEdit->setEnabled(enabled);
    m_ui->fileTitleEdit->setEnabled(enabled);
    m_ui->fileYearEdit->setEnabled(enabled);

    m_ui->openPixButton->setEnabled(enabled);
    m_ui->deletePixButton->setEnabled(enabled);
    m_ui->saveButton->setEnabled(enabled);
}
