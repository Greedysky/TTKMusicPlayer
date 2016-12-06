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
      m_ui(new Ui::MusicSongSharingWidget)
{
    m_ui->setupUi(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->qqButton->setChecked(true);
    m_ui->textEdit->setStyleSheet(MusicUIObject::MTextEditStyle01);

    m_qrCodeWidget = new QRCodeQWidget(QByteArray(), QSize(90, 90), this);
    m_qrCodeWidget->setMargin(2);
    m_qrCodeWidget->setIcon(":/image/lb_player_logo", 0.23);
    m_ui->QRCodeIconWidgetLayout->addWidget(m_qrCodeWidget);

    connect(m_ui->textEdit, SIGNAL(textChanged()), SLOT(textAreaChanged()));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->shareButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
}

MusicSongSharingWidget::~MusicSongSharingWidget()
{
    delete m_qrCodeWidget;
    delete m_ui;
}

QString MusicSongSharingWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongSharingWidget::setSongName(const QString &name)
{
    m_ui->sharedName->setToolTip(name);
    m_ui->sharedName->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 200));

    QString path = ART_DIR_FULL + MusicUtils::Core::artistName(name) + SKN_FILE;
    m_ui->sharedNameIcon->setPixmap(QPixmap(QFile::exists(path)
                                  ? path : ":/image/lb_defaultArt").scaled(50, 50));

    m_ui->textEdit->setText(tr("I used to listen music #%1# by TTKMusicPlayer,").arg(name) +
                          tr("and recommend it to you! (From #TTKMusicPlayer#)"));
}

int MusicSongSharingWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSongSharingWidget::confirmButtonClicked()
{
    MusicDownLoadQueryWYThread *down = new MusicDownLoadQueryWYThread(this);
    down->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, m_ui->sharedName->text().trimmed());

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
    if(m_ui->qqButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(QQ_SHARE, URL_KEY)).arg(playUrl).arg(m_ui->textEdit->toPlainText()).arg(imageUrl)
                               .arg(m_ui->sharedName->text()).arg(tr("TTKMusicPlayer"));
    }
    else if(m_ui->renrenButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(RENREN_SHARE, URL_KEY)).arg(playUrl).arg(m_ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(m_ui->qqspaceButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(QQ_SPACE_SHARE, URL_KEY)).arg(playUrl).arg(tr("TTKMusicPlayer")).arg(imageUrl)
                                     .arg(m_ui->textEdit->toPlainText());
    }
    else if(m_ui->qqblogButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(QQ_MICBG_SHARE, URL_KEY)).arg(playUrl).arg(m_ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(m_ui->sinaButton->isChecked())
    {
        url = QString(MusicCryptographicHash::decryptData(SINA_SHARE, URL_KEY)).arg(playUrl).arg(imageUrl).arg(m_ui->textEdit->toPlainText());
    }
    else if(m_ui->weixingButton->isChecked())
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
    QString text = m_ui->textEdit->toPlainText();
    int length = text.count();
    if(length > max)
    {
        QTextCursor textCursor = m_ui->textEdit->textCursor();
        int position = textCursor.position();
        text.remove(position - (length - max), length - max);
        m_ui->textEdit->setText(text);
        textCursor.setPosition(position - (length - max));
        m_ui->textEdit->setTextCursor(textCursor);
    }
    else
    {
        m_ui->textEditArea->setText(tr("You can enter %1 characters").arg(max - length));
    }
}
