#include "musicsongsharingwidget.h"
#include "ui_musicsongsharingwidget.h"
#include "musicdownloadquerythreadabstract.h"
#include "musicdownloadqueryfactory.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicobject.h"
#include "musicurlutils.h"
#include "musicstringutils.h"
#include "musicwidgetutils.h"
#include "musicsemaphoreloop.h"
#include "musicdownloadsourcethread.h"
#include "musicotherdefine.h"

#include "qrencode/qrcodewidget.h"

MusicSongSharingWidget::MusicSongSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongSharingWidget)
{
    m_ui->setupUi(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_type = Null;

    m_ui->qqButton->setChecked(true);
    m_ui->textEdit->setStyleSheet(MusicUIObject::MTextEditStyle01);

    m_qrCodeWidget = new QRCodeQWidget(QByteArray(), QSize(90, 90), this);
    m_qrCodeWidget->setMargin(2);
    m_qrCodeWidget->setIcon(":/image/lb_app_logo", 0.23);
    m_ui->QRCodeIconWidgetLayout->addWidget(m_qrCodeWidget);

#ifdef Q_OS_UNIX
    m_ui->qqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->renrenButton->setFocusPolicy(Qt::NoFocus);
    m_ui->qqspaceButton->setFocusPolicy(Qt::NoFocus);
    m_ui->qqblogButton->setFocusPolicy(Qt::NoFocus);
    m_ui->sinaButton->setFocusPolicy(Qt::NoFocus);
    m_ui->weixingButton->setFocusPolicy(Qt::NoFocus);
    m_ui->shareButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif

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

void MusicSongSharingWidget::setData(Type type, const QVariantMap &data)
{
    m_type = type;
    m_data = data;

    const QString &name = data["songName"].toString();

    switch(m_type)
    {
        case Song: break;
        case Movie: break;
        case Artist:
        case Album:
        case Playlist:
            {
                const QString &smallUrl = data["smallUrl"].toString();
                MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
                connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
                if(!smallUrl.isEmpty() && smallUrl != COVER_URL_NULL)
                {
                    download->startToDownload(smallUrl);
                }
                break;
            }
        default: break;
    }

    m_ui->sharedName->setToolTip(name);
    m_ui->sharedName->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 200));

    QString path = ART_DIR_FULL + MusicUtils::String::artistName(name) + SKN_FILE;
    m_ui->sharedNameIcon->setPixmap(QPixmap(QFile::exists(path) ? path : ":/image/lb_defaultArt").scaled(50, 50));
    m_ui->textEdit->setText(tr("I used to listen music #%1# by TTKMusicPlayer,").arg(name) + tr("and recommend it to you! (From #TTKMusicPlayer#)"));
}

int MusicSongSharingWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicSongSharingWidget::confirmButtonClicked()
{
    switch(m_type)
    {
        case Song:
            {
                MusicDownLoadQueryThreadAbstract *d = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
                d->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, m_ui->sharedName->text().trimmed());

                MusicSemaphoreLoop loop;
                connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
                loop.exec();

                if(!d->isEmpty())
                {
                    const MusicObject::MusicSongInformation info(d->getMusicSongInfos().first());
                    QString server = d->getQueryServer();
                    if(server == QUERY_WY_INTERFACE)
                        server = MusicUtils::Algorithm::mdII(WY_SG_SHARE, ALG_LOW_KEY, false).arg(info.m_songId);
                    else if(server == QUERY_QQ_INTERFACE)
                        server = MusicUtils::Algorithm::mdII(QQ_SG_SHARE, ALG_LOW_KEY, false).arg(info.m_songId);
                    else if(server == QUERY_KG_INTERFACE)
                        server = MusicUtils::Algorithm::mdII(KG_SG_SHARE, ALG_LOW_KEY, false).arg(info.m_songId);
                    else if(server == QUERY_BD_INTERFACE)
                        server = MusicUtils::Algorithm::mdII(BD_SG_SHARE, ALG_LOW_KEY, false).arg(info.m_songId);
                    else if(server == QUERY_KW_INTERFACE)
                        server = MusicUtils::Algorithm::mdII(KW_SG_SHARE, ALG_LOW_KEY, false).arg(info.m_songId);
                    else if(server == QUERY_XM_INTERFACE)
                        server = MusicUtils::Algorithm::mdII(XM_SG_SHARE, ALG_LOW_KEY, false).arg(info.m_songId);
                    else
                    {
                        QTimer::singleShot(2*MT_S2MS, this, SLOT(queryUrlTimeout()));
                        break;
                    }

                    downLoadDataChanged(server, info.m_smallPicUrl);
                }
                else
                {
                    QTimer::singleShot(2*MT_S2MS, this, SLOT(queryUrlTimeout()));
                }
                break;
            }
        case Movie:
            {
                QString server = m_data["queryServer"].toString(), id = m_data["id"].toString();
                if(id.contains(MUSIC_YYT_PREFIX))
                {
                    id.remove(MUSIC_YYT_PREFIX);
                    server = MusicUtils::Algorithm::mdII(YT_MV_SHARE, ALG_LOW_KEY, false).arg(id);
                }
                else if(server == QUERY_WY_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(WY_MV_SHARE, ALG_LOW_KEY, false).arg(id);
                else if(server == QUERY_QQ_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(QQ_MV_SHARE, ALG_LOW_KEY, false).arg(id);
                else if(server == QUERY_KG_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KG_MV_SHARE, ALG_LOW_KEY, false).arg(id);
                else if(server == QUERY_BD_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(BD_MV_SHARE, ALG_LOW_KEY, false).arg(id);
                else if(server == QUERY_KW_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KW_MV_SHARE, ALG_LOW_KEY, false).arg(id);
                else if(server == QUERY_XM_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(XM_MV_SHARE, ALG_LOW_KEY, false).arg(id);
                else
                {
                    QTimer::singleShot(2*MT_S2MS, this, SLOT(queryUrlTimeout()));
                    break;
                }

                downLoadDataChanged(server, QString());
                break;
            }
        case Artist:
            {
                QString server = m_data["queryServer"].toString();
                if(server == QUERY_WY_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(WY_AR_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_QQ_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(QQ_AR_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_KG_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KG_AR_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_BD_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(BD_AR_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_KW_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KW_AR_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_XM_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(XM_AR_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else
                {
                    QTimer::singleShot(2*MT_S2MS, this, SLOT(queryUrlTimeout()));
                    break;
                }

                downLoadDataChanged(server, m_data["smallUrl"].toString());
                break;
            }
        case Album:
            {
                QString server = m_data["queryServer"].toString();
                if(server == QUERY_WY_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(WY_AL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_QQ_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(QQ_AL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_KG_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KG_AL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_BD_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(BD_AL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_KW_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KW_AL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_XM_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(XM_AL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else
                {
                    QTimer::singleShot(2*MT_S2MS, this, SLOT(queryUrlTimeout()));
                    break;
                }

                downLoadDataChanged(server, m_data["smallUrl"].toString());
                break;
            }
        case Playlist:
            {
                QString server = m_data["queryServer"].toString();
                if(server == QUERY_WY_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(WY_PL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_QQ_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(QQ_PL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_KG_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KG_PL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_BD_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(BD_PL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_KW_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(KW_PL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else if(server == QUERY_XM_INTERFACE)
                    server = MusicUtils::Algorithm::mdII(XM_PL_SHARE, ALG_LOW_KEY, false).arg(m_data["id"].toString());
                else
                {
                    QTimer::singleShot(2*MT_S2MS, this, SLOT(queryUrlTimeout()));
                    break;
                }

                downLoadDataChanged(server, m_data["smallUrl"].toString());
                break;
            }
        default: break;
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
        url = QString(MusicUtils::Algorithm::mdII(QQ_SHARE, ALG_LOW_KEY, false)).arg(playUrl).arg(m_ui->textEdit->toPlainText()).arg(imageUrl).arg(m_ui->sharedName->text()).arg(tr("TTKMusicPlayer"));
    }
    else if(m_ui->renrenButton->isChecked())
    {
        url = QString(MusicUtils::Algorithm::mdII(RENREN_SHARE, ALG_LOW_KEY, false)).arg(playUrl).arg(m_ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(m_ui->qqspaceButton->isChecked())
    {
        url = QString(MusicUtils::Algorithm::mdII(QQ_SPACE_SHARE, ALG_LOW_KEY, false)).arg(playUrl).arg(tr("TTKMusicPlayer")).arg(imageUrl).arg(m_ui->textEdit->toPlainText());
    }
    else if(m_ui->qqblogButton->isChecked())
    {
        url = QString(MusicUtils::Algorithm::mdII(QQ_MICBG_SHARE, ALG_LOW_KEY, false)).arg(playUrl).arg(m_ui->textEdit->toPlainText()).arg(imageUrl);
    }
    else if(m_ui->sinaButton->isChecked())
    {
        url = QString(MusicUtils::Algorithm::mdII(SINA_SHARE, ALG_LOW_KEY, false)).arg(playUrl).arg(imageUrl).arg(m_ui->textEdit->toPlainText());
    }
    else if(m_ui->weixingButton->isChecked())
    {
        m_qrCodeWidget->setText(playUrl.toUtf8());
        m_qrCodeWidget->update();
        return;
    }

    url.replace("?name=", "%3Fname%3D");
    url.replace("?id=", "%3Fid%3D");
    url.replace("?pid=", "%3Fpid%3D");
    url.replace("#hash=", "%23hash%3D");
    url.replace('#', "%23");

    MusicUtils::Url::openUrl(url, false);
    QTimer::singleShot(MT_S2MS, this, SLOT(close()));
}

void MusicSongSharingWidget::close()
{
    ///remove temp file path
    QFile::remove(ART_DIR_FULL + TEMPPATH);
    MusicAbstractMoveDialog::close();
}

void MusicSongSharingWidget::textAreaChanged()
{
    const int max = 150;
    QString text = m_ui->textEdit->toPlainText();
    const int length = text.count();

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

void MusicSongSharingWidget::downLoadFinished(const QByteArray &data)
{
    if(m_ui->sharedNameIcon)
    {
        QPixmap pix;
        pix.loadFromData(data);
        m_ui->sharedNameIcon->setPixmap(pix.scaled(m_ui->sharedNameIcon->size()));
    }
}
