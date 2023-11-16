#include "musicsongsharingwidget.h"
#include "ui_musicsongsharingwidget.h"
#include "musicabstractqueryrequest.h"
#include "musicdownloadqueryfactory.h"
#include "musictoastlabel.h"
#include "musicurlutils.h"
#include "musiccoversourcerequest.h"

#include "qrencode/qrcodewidget.h"

MusicSongSharingWidget::MusicSongSharingWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSongSharingWidget),
      m_type(Module::Null)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->qqButton->setChecked(true);
    m_ui->textEdit->setStyleSheet(TTK::UI::TextEditStyle01);

    m_qrCodeWidget = new QRCodeQWidget({}, QSize(90, 90), this);
    m_qrCodeWidget->setMargin(2);
    m_qrCodeWidget->setIcon(":/image/lb_app_logo", 0.23);
    m_ui->QRCodeIconWidgetLayout->addWidget(m_qrCodeWidget);

#ifdef Q_OS_UNIX
    m_ui->qqButton->setFocusPolicy(Qt::NoFocus);
    m_ui->qqspaceButton->setFocusPolicy(Qt::NoFocus);
    m_ui->sinaButton->setFocusPolicy(Qt::NoFocus);
    m_ui->weixingButton->setFocusPolicy(Qt::NoFocus);
    m_ui->shareButton->setFocusPolicy(Qt::NoFocus);
    m_ui->cancelButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->textEdit, SIGNAL(textChanged()), SLOT(textAreaChanged()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->shareButton, SIGNAL(clicked()), SLOT(confirmButtonClicked()));
}

MusicSongSharingWidget::~MusicSongSharingWidget()
{
    delete m_qrCodeWidget;
    delete m_ui;
}

void MusicSongSharingWidget::setData(Module type, const QVariantMap &data)
{
    m_type = type;
    m_data = data;

    const QString &name = data["songName"].toString();

    switch(m_type)
    {
        case Module::Song: break;
        case Module::Movie: break;
        case Module::Artist:
        case Module::Album:
        case Module::Playlist:
        {
            const QString &cover = data["smallUrl"].toString();
            if(!cover.isEmpty() && cover != TTK_NULL_STR)
            {
                MusicCoverSourceRequest *d = new MusicCoverSourceRequest(this);
                connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
                d->startRequest(cover);
            }
            break;
        }
        default: break;
    }

    m_ui->sharedName->setToolTip(name);
    m_ui->sharedName->setText(TTK::Widget::elidedText(font(), name, Qt::ElideRight, 200));

    QString path = ART_DIR_FULL + TTK::String::artistName(name) + SKN_FILE;
    m_ui->sharedNameIcon->setPixmap(QPixmap(QFile::exists(path) ? path : ":/image/lb_default_art").scaled(50, 50));
    m_ui->textEdit->setText(tr("I used to listen music #%1# by TTKMusicPlayer,").arg(name) + tr("and recommend it to you! (From #TTKMusicPlayer#)"));
}

void MusicSongSharingWidget::confirmButtonClicked()
{
    switch(m_type)
    {
        case Module::Song:
        {
            TTKSemaphoreLoop loop;
            MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
            d->startToSearch(MusicAbstractQueryRequest::QueryType::Music, m_ui->sharedName->text().trimmed());
            loop.exec();

            if(!d->isEmpty())
            {
                const TTK::MusicSongInformation info(d->songInfoList().front());
                QString server = d->queryServer();
                if(server == QUERY_WY_INTERFACE)
                {
                    server = TTK::Algorithm::mdII(WY_SG_SHARE, ALG_SHR_KEY, false).arg(info.m_songId);
                }
                else if(server == QUERY_KG_INTERFACE)
                {
                    server = TTK::Algorithm::mdII(KG_SG_SHARE, ALG_SHR_KEY, false).arg(info.m_songId);
                }
                else if(server == QUERY_KW_INTERFACE)
                {
                    server = TTK::Algorithm::mdII(KW_SG_SHARE, ALG_SHR_KEY, false).arg(info.m_songId);
                }
                else
                {
                    QTimer::singleShot(2 * TTK_DN_S2MS, this, SLOT(queryUrlTimeout()));
                    break;
                }

                downLoadFinished(server, info.m_coverUrl);
            }
            else
            {
                QTimer::singleShot(2 * TTK_DN_S2MS, this, SLOT(queryUrlTimeout()));
            }
            break;
        }
        case Module::Movie:
        {
            QString server = m_data["queryServer"].toString(), id = m_data["id"].toString();
            if(server == QUERY_WY_INTERFACE)
            {
                server = TTK::Algorithm::mdII(WY_MV_SHARE, ALG_SHR_KEY, false).arg(id);
            }
            else if(server == QUERY_KG_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KG_MV_SHARE, ALG_SHR_KEY, false).arg(id);
            }
            else if(server == QUERY_KW_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KW_MV_SHARE, ALG_SHR_KEY, false).arg(id);
            }
            else
            {
                QTimer::singleShot(2 * TTK_DN_S2MS, this, SLOT(queryUrlTimeout()));
                break;
            }

            downLoadFinished(server, {});
            break;
        }
        case Module::Artist:
        {
            QString server = m_data["queryServer"].toString();
            if(server == QUERY_WY_INTERFACE)
            {
                server = TTK::Algorithm::mdII(WY_AR_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else if(server == QUERY_KG_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KG_AR_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else if(server == QUERY_KW_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KW_AR_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else
            {
                QTimer::singleShot(2 * TTK_DN_S2MS, this, SLOT(queryUrlTimeout()));
                break;
            }

            downLoadFinished(server, m_data["smallUrl"].toString());
            break;
        }
        case Module::Album:
        {
            QString server = m_data["queryServer"].toString();
            if(server == QUERY_WY_INTERFACE)
            {
                server = TTK::Algorithm::mdII(WY_AL_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else if(server == QUERY_KG_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KG_AL_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else if(server == QUERY_KW_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KW_AL_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else
            {
                QTimer::singleShot(2 * TTK_DN_S2MS, this, SLOT(queryUrlTimeout()));
                break;
            }

            downLoadFinished(server, m_data["smallUrl"].toString());
            break;
        }
        case Module::Playlist:
        {
            QString server = m_data["queryServer"].toString();
            if(server == QUERY_WY_INTERFACE)
            {
                server = TTK::Algorithm::mdII(WY_PL_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else if(server == QUERY_KG_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KG_PL_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else if(server == QUERY_KW_INTERFACE)
            {
                server = TTK::Algorithm::mdII(KW_PL_SHARE, ALG_SHR_KEY, false).arg(m_data["id"].toString());
            }
            else
            {
                QTimer::singleShot(2 * TTK_DN_S2MS, this, SLOT(queryUrlTimeout()));
                break;
            }

            downLoadFinished(server, m_data["smallUrl"].toString());
            break;
        }
        default: break;
    }
}

void MusicSongSharingWidget::queryUrlTimeout()
{
    MusicToastLabel::popup(tr("Song does not support sharing"));
}

void MusicSongSharingWidget::downLoadFinished(const QString &playUrl, const QString &imageUrl)
{
    QString url;
    if(m_ui->qqButton->isChecked())
    {
        url = QString(TTK::Algorithm::mdII(QQ_SHARE, ALG_SHR_KEY, false)).arg(playUrl, m_ui->textEdit->toPlainText(), imageUrl, m_ui->sharedName->text(), tr("TTKMusicPlayer"));
    }
    else if(m_ui->qqspaceButton->isChecked())
    {
        url = QString(TTK::Algorithm::mdII(QZONE_SHARE, ALG_SHR_KEY, false)).arg(playUrl, tr("TTKMusicPlayer"), imageUrl, m_ui->textEdit->toPlainText());
    }
    else if(m_ui->sinaButton->isChecked())
    {
        url = QString(TTK::Algorithm::mdII(SINA_SHARE, ALG_SHR_KEY, false)).arg(playUrl, imageUrl, m_ui->textEdit->toPlainText());
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

    TTK::Url::openUrl(url, false);
    QTimer::singleShot(TTK_DN_S2MS, this, SLOT(close()));
}

void MusicSongSharingWidget::textAreaChanged()
{
    constexpr int max = 150;
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

void MusicSongSharingWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    if(m_ui->sharedNameIcon)
    {
        QPixmap pix;
        pix.loadFromData(bytes);
        m_ui->sharedNameIcon->setPixmap(pix.scaled(m_ui->sharedNameIcon->size()));
    }
}
