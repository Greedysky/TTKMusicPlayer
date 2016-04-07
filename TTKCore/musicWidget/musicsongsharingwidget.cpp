#include "musicsongsharingwidget.h"
#include "ui_musicsongsharingwidget.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicbackgroundmanager.h"
#include "musicdata2downloadthread.h"
#include "musicmessagebox.h"

#include <QTimer>
#include <QDesktopServices>

MusicSongSharingWidget::MusicSongSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSongSharingWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->qqButton->setChecked(true);
    ui->textEdit->setStyleSheet(MusicUIObject::MTextEditStyle01);

    connect(ui->textEdit, SIGNAL(textChanged()), SLOT(textAreaChanged()));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->shareButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
}

MusicSongSharingWidget::~MusicSongSharingWidget()
{
    delete ui;
}

void MusicSongSharingWidget::setSongName(const QString &name)
{
    ui->sharedName->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 200));

    QString path = ART_DOWNLOAD_AL + name.split('-').front().trimmed() + SKN_FILE;
    ui->sharedNameIcon->setPixmap(QPixmap(QFile::exists(path)
                                  ? path : ":/share/defaultArt").scaled(50, 50));

    ui->textEdit->setText(tr("I used to listen music #%1# by TTKMusicPlayer,").arg(name) +
                          tr("and recommend it to you! (From #TTKMusicPlayer#)"));
}

int MusicSongSharingWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}

void MusicSongSharingWidget::confirmButtonClicked()
{
    QStringList infos = ui->sharedName->text().split('-');
    if(infos.count() != 0)
    {
        ///download art picture
        MusicData2DownloadThread *down = new MusicData2DownloadThread(
                    SML_BG_ART_URL.arg(infos.front().trimmed()),
                    ART_DOWNLOAD_AL + TMP_DOWNLOAD,
                    MusicDownLoadThreadAbstract::Download_SmlBG, this);
        connect(down, SIGNAL(data2urlHasChanged(QString)),
                      SLOT(data2urlHasChanged(QString)), Qt::QueuedConnection);
        down->startToDownload();
    }
    QTimer::singleShot(5000, this, SLOT(queryUrlTimeout()));
}

void MusicSongSharingWidget::queryUrlTimeout()
{
    MusicMessageBox message;
    message.setText(tr("Song does not support sharing!"));
    message.exec();
}

void MusicSongSharingWidget::data2urlHasChanged(const QString &imageUrl)
{
    QString url;
    if(ui->qqButton->isChecked())
    {
        url = QString(QQ_SHARE).arg(ui->textEdit->toPlainText()).arg(imageUrl)
                               .arg(ui->sharedName->text()).arg(tr("TTKMusicPlayer"));
    }
    else if(ui->renrenButton->isChecked())
    {
        url = QString(RENREN_SHARE).arg(ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(ui->qqspaceButton->isChecked())
    {
        url = QString(QQ_SPACE_SHARE).arg(tr("TTKMusicPlayer")).arg(imageUrl)
                                     .arg(ui->textEdit->toPlainText());
    }
    else if(ui->qqblogButton->isChecked())
    {
        url = QString(QQ_MICBG_SHARE).arg(ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(ui->sinaButton->isChecked())
    {
        url = QString(SINA_SHARE).arg(imageUrl).arg(ui->textEdit->toPlainText());
    }

    url.replace('#', "%23");
    QDesktopServices::openUrl(QUrl(url));
    QTimer::singleShot(1000, this, SLOT(close()));
}

void MusicSongSharingWidget::close()
{
    ///remove temp file path
    QFile::remove(ART_DOWNLOAD_AL + TMP_DOWNLOAD);
    MusicAbstractMoveDialog::close();
}

void MusicSongSharingWidget::textAreaChanged()
{
    int count = ui->textEdit->toPlainText().length();
    if((count = 150 - count) > -1)
    {
        ui->textEditArea->setText(tr("You can enter %1 characters").arg(count));
    }
    ui->textEdit->setEnabled( count > -1 );
}
