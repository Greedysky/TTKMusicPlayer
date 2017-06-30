#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicsourceupdatethread.h"
#include "musicdatadownloadthread.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#///QJson import
#include "qjson/parser.h"

#include <QProcess>

MusicSourceUpdateWidget::MusicSourceUpdateWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSourceUpdateWidget)
{
    m_ui->setupUi(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    m_ui->upgradeButton->setEnabled(false);

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->upgradeFailedLabel, SIGNAL(clicked()), SLOT(upgradeFailedClicked()));
    connect(m_ui->upgradeButton, SIGNAL(clicked()), SLOT(upgradeButtonClicked()));
}

MusicSourceUpdateWidget::~MusicSourceUpdateWidget()
{
    delete m_ui;
}

QString MusicSourceUpdateWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
    m_ui->stackedWidget->setCurrentIndex(1);
    QString localDwonload = "v" + m_newVersionStr + DD_TYPE_EXE;
    MusicDataDownloadThread *download = new MusicDataDownloadThread(QString("%1%2").arg(MusicUtils::Algorithm::mdII(DOWNLOAD_URL, false)).arg(localDwonload),
                                                                    MusicObject::getAppDir() + localDwonload, MusicDownLoadThreadAbstract::Download_Other, this);
    connect(download, SIGNAL(downloadProgressChanged(float,QString,qint64)), SLOT(downloadProgressChanged(float,QString)));
    connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downloadProgressFinished()));
    download->startToDownload();
}

void MusicSourceUpdateWidget::upgradeFailedClicked()
{
    MusicUtils::Core::openUrl(MusicUtils::Algorithm::mdII(CSDN_URL, false), false);
}

void MusicSourceUpdateWidget::downLoadFinished(const QVariant &data)
{
    QVariantMap value = data.toMap();
    m_newVersionStr = value["version"].toString();

    QString text;
    if(m_newVersionStr != TTKMUSIC_VERSION_STR)
    {
        text.append(m_newVersionStr);
        text.append("\r\n");
        text.append(value["data"].toString());
        m_ui->upgradeButton->setEnabled(true);
        m_ui->titleLable_F->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    }
    else
    {
        text.append(tr("Current version is updated!"));
        m_ui->titleLable_F->setAlignment(Qt::AlignCenter);
    }
    m_ui->titleLable_F->setText( text );
}

void MusicSourceUpdateWidget::downloadProgressChanged(float percent, const QString &total)
{
    m_ui->fileSizeLabel->setText(tr("FileSize: %1").arg(total));
    m_ui->downProgressBar->setValue(percent);
    m_ui->downProgressBarAL->setValue(percent);
}

void MusicSourceUpdateWidget::downloadProgressFinished()
{
    QString localDwonload = "v" + m_newVersionStr + DD_TYPE_EXE;
    MusicMessageBox message(this);
    message.setText(tr("Download Finish, Install Or Not"));
    if(message.exec() == 0)
    {
        QProcess::startDetached(MusicObject::getAppDir() + localDwonload, QStringList());
        MStatic_cast(QWidget*, parent())->close();
    }
}

int MusicSourceUpdateWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());

    MusicSourceUpdateThread *download = new MusicSourceUpdateThread(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), SLOT(downLoadFinished(QVariant)));
    download->startToDownload();

    return MusicAbstractMoveDialog::exec();
}
