#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicbackgroundmanager.h"
#include "musicsourcedownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicuiobject.h"
#include "musicutils.h"
#include "musicversion.h"

#include <QProcess>

#ifdef MUSIC_GREATER_NEW
#   include <QJsonParseError>
#   include <QJsonObject>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#endif

MusicSourceUpdateWidget::MusicSourceUpdateWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSourceUpdateWidget)
{
    ui->setupUi(this);
    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
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

QString MusicSourceUpdateWidget::getClassName()
{
    return staticMetaObject.className();
}

int MusicSourceUpdateWidget::exec()
{
    QPixmap pix(M_BACKGROUND_PTR->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));

    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(VERSION_URL);

    return MusicAbstractMoveDialog::exec();
}

void MusicSourceUpdateWidget::downLoadFinished(const QByteArray &data)
{
#ifdef MUSIC_GREATER_NEW
    QJsonParseError jsonError;
    QJsonDocument parseDoucment = QJsonDocument::fromJson(data, &jsonError);
    ///Put the data into Json
    if( jsonError.error != QJsonParseError::NoError ||
        !parseDoucment.isObject())
    {
        return;
    }

    QJsonObject jsonObject = parseDoucment.object();
    m_newVersionStr = jsonObject.value("version").toString();
#else
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("value=" + QString(data));
    m_newVersionStr = sc.property("version").toString();
#endif
    QString text;
    if(m_newVersionStr != TTKMUSIC_VERSION_STR)
    {
        text.append(m_newVersionStr);
        text.append("\r\n");
#ifdef MUSIC_GREATER_NEW
        text.append(jsonObject.value("data").toString());
#else
        text.append(sc.property("data").toString());
#endif
        ui->upgradeButton->setEnabled(true);
        ui->titleLable_F->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    }
    else
    {
        text.append(tr("Current version is updated!"));
        ui->titleLable_F->setAlignment(Qt::AlignCenter);
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
        QString localDwonload = "v" + m_newVersionStr + DD_TYPE_ZIP;
        QProcess::startDetached(MusicObject::getAppDir() + NEW_UPDATE_URL, QStringList(localDwonload));
        MStatic_cast(QWidget*, parent())->close();
    }
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QString localDwonload = "v" + m_newVersionStr + DD_TYPE_ZIP;
    MusicDataDownloadThread *download = new MusicDataDownloadThread(QString("%1%2").arg(DOWNLOAD_URL).arg(localDwonload),
                                                                    MusicObject::getAppDir() + localDwonload, MusicDownLoadThreadAbstract::Download_Other, this);
    connect(download, SIGNAL(downloadProgressChanged(float,QString,qint64)), SLOT(downloadProgressChanged(float,QString)));
    download->startToDownload();
}

void MusicSourceUpdateWidget::upgradeFailedClicked()
{
    MusicUtils::UCore::openUrl(CSDN_URL, false);
}
