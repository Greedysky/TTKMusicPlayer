#include "musicsongsharingwidget.h"
#include "ui_musicsongsharingwidget.h"
#include "musicdownloadquerywythread.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musiccoreutils.h"
#include "musicwidgetutils.h"

#include "qrcodewidget.h"

#include <QTimer>

MusicSongSharingWidget::MusicSongSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSongSharingWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));

    ui->qqButton->setChecked(true);
    ui->textEdit->setStyleSheet(MusicUIObject::MTextEditStyle01);

    m_qrCodeWidget = new QRCodeQWidget(QByteArray(), QSize(90, 90), this);
    m_qrCodeWidget->setMargin(2);
    m_qrCodeWidget->setIcon(":/image/lb_player_logo", 0.23);
    ui->QRCodeIconWidgetLayout->addWidget(m_qrCodeWidget);

    connect(ui->textEdit, SIGNAL(textChanged()), SLOT(textAreaChanged()));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->shareButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
}

MusicSongSharingWidget::~MusicSongSharingWidget()
{
    delete m_qrCodeWidget;
    delete ui;
}

QString MusicSongSharingWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongSharingWidget::setSongName(const QString &name)
{
    ui->sharedName->setToolTip(name);
    ui->sharedName->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 200));

    QString path = ART_DIR_FULL + MusicUtils::Core::artistName(name) + SKN_FILE;
    ui->sharedNameIcon->setPixmap(QPixmap(QFile::exists(path)
                                  ? path : ":/image/lb_defaultArt").scaled(50, 50));

    ui->textEdit->setText(tr("I used to listen music #%1# by TTKMusicPlayer,").arg(name) +
                          tr("and recommend it to you! (From #TTKMusicPlayer#)"));
}

int MusicSongSharingWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSongSharingWidget::confirmButtonClicked()
{
    MusicDownLoadQueryWYThread *down = new MusicDownLoadQueryWYThread(this);
    down->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, ui->sharedName->text().trimmed());

    QEventLoop loop;
    connect(down, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    if(!down->getMusicSongInfos().isEmpty())
    {
        MusicObject::MusicSongInfomation info(down->getMusicSongInfos().first());
        downLoadDataChanged(MusicCryptographicHash::decryptData(WEB_PLAYER, URL_KEY) + info.m_songId, info.m_smallPicUrl);
    }
    else
    {
        QTimer::singleShot(2*MT_S2MS, this, SLOT(queryUrlTimeout()));
    }
}

void MusicSongSharingWidget::queryUrlTimeout()
{
    MusicMessageBox message;
    message.setText(tr("Song does not support sharing!"));
    message.exec();
}

void MusicSongSharingWidget::downLoadDataChanged(const QString &playUrl, const QString &imageUrl)
{
    QString url;
    if(ui->qqButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(QQ_SHARE, URL_KEY)).arg(playUrl).arg(ui->textEdit->toPlainText()).arg(imageUrl)
                               .arg(ui->sharedName->text()).arg(tr("TTKMusicPlayer"));
    }
    else if(ui->renrenButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(RENREN_SHARE, URL_KEY)).arg(playUrl).arg(ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(ui->qqspaceButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(QQ_SPACE_SHARE, URL_KEY)).arg(playUrl).arg(tr("TTKMusicPlayer")).arg(imageUrl)
                                     .arg(ui->textEdit->toPlainText());
    }
    else if(ui->qqblogButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(QQ_MICBG_SHARE, URL_KEY)).arg(playUrl).arg(ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(ui->sinaButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(SINA_SHARE, URL_KEY)).arg(playUrl).arg(imageUrl).arg(ui->textEdit->toPlainText());
    }
    else if(ui->weixingButton->isChecked())
    {
        m_qrCodeWidget->setText(playUrl.toUtf8());
        m_qrCodeWidget->update();
        return;
    }

    url.replace("player?song=", "player%3Fsong%3D");
    url.replace('#', "%23");

    MusicUtils::Core::openUrl(url, false);
    QTimer::singleShot(MT_S2MS, this, SLOT(close()));
}

void MusicSongSharingWidget::close()
{
    ///remove temp file path
    QFile::remove(ART_DIR_FULL + TEMPORARY_DIR);
    MusicAbstractMoveDialog::close();
}

void MusicSongSharingWidget::textAreaChanged()
{
    const int max = 150;
    QString text = ui->textEdit->toPlainText();
    int length = text.count();
    if(length > max)
    {
        QTextCursor textCursor = ui->textEdit->textCursor();
        int position = textCursor.position();
        text.remove(position - (length - max), length - max);
        ui->textEdit->setText(text);
        textCursor.setPosition(position - (length - max));
        ui->textEdit->setTextCursor(textCursor);
    }
    else
    {
        ui->textEditArea->setText(tr("You can enter %1 characters").arg(max - length));
    }
}
