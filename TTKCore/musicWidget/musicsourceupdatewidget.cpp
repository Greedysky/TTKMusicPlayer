#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicbackgroundmanager.h"
#include "musicsourcedownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicuiobject.h"
#include "musicutils.h"
#include "musicversion.h"
#include "musicmessagebox.h"

#ifdef MUSIC_QT_5
#   include <QJsonParseError>
#   include <QJsonObject>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#endif

#define VERSION_URL     "http://7xpa0g.com1.z0.glb.clouddn.com/version"
#define DOWNLOAD_URL    "http://7xpa0g.com1.z0.glb.clouddn.com/v2.2.11.0.7z"
#define CSDN_URL        "http://download.csdn.net/album/detail/3094"

MusicSourceUpdateWidget::MusicSourceUpdateWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSourceUpdateWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    ui->upgradeButton->setEnabled(false);

    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->upgradeFailedLabel, SIGNAL(clicked()), SLOT(upgradeFailedClicked()));
    connect(ui->upgradeButton, SIGNAL(clicked()), SLOT(upgradeButtonClicked()));
}

MusicSourceUpdateWidget::~MusicSourceUpdateWidget()
{
    delete ui;
}

int MusicSourceUpdateWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));

    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    connect(download, SIGNAL(recievedData(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(VERSION_URL);

    return MusicAbstractMoveDialog::exec();
}

void MusicSourceUpdateWidget::downLoadFinished(const QByteArray &data)
{
    QJsonParseError jsonError;
    QJsonDocument parseDoucment = QJsonDocument::fromJson(data, &jsonError);
    ///Put the data into Json
    if( jsonError.error != QJsonParseError::NoError ||
        !parseDoucment.isObject())
    {
        return;
    }

    QJsonObject jsonObject = parseDoucment.object();
    QString text, version = jsonObject.value("version").toString();
    if(version == TTKMUSIC_VERSION_STR)
    {
        text.append(version);
        text.append("\r\n");
        text.append(jsonObject.value("data").toString());
        ui->upgradeButton->setEnabled(true);
    }
    else
    {
        text.append(tr("Current version is updated!"));
    }
    ui->titleLable_F->setText( text );
}

void MusicSourceUpdateWidget::downloadProgressChanged(float percent, const QString &total)
{
    ui->fileSizeLabel->setText(tr("FileSize: %1").arg(total));
    ui->downProgressBar->setValue(percent);
    ui->downProgressBarAL->setValue(percent);

    if(percent == 100.0f)
    {
        MusicMessageBox message;
        message.setText(tr("Download finished, please reunzip!"));
        message.exec();

        close();
    }
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    MusicDataDownloadThread *download = new MusicDataDownloadThread(DOWNLOAD_URL, TMP_DOWNLOAD + QString(".7z"),
                                                                    MusicDownLoadThreadAbstract::Download_Other, this);
    connect(download, SIGNAL(downloadProgressChanged(float,QString,qint64)), SLOT(downloadProgressChanged(float,QString)));
    download->startToDownload();
}

void MusicSourceUpdateWidget::upgradeFailedClicked()
{
    MusicUtils::openUrl(CSDN_URL, false);
}
