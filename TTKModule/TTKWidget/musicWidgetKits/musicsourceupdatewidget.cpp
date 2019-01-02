#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicsourceupdatethread.h"
#include "musicdatadownloadthread.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#include "musicurlutils.h"
#include "musictime.h"
#include "musicsettingmanager.h"
#include "musicapplicationobject.h"
#include "musicotherdefine.h"
#///QJson import
#include "qjson/parser.h"

#include <QBoxLayout>

MusicSourceUpdateNotifyWidget::MusicSourceUpdateNotifyWidget(QWidget *parent)
    : MusicAbstractMoveSingleWidget(true, parent)
{
    hide();

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);
    setStyleSheet(MusicUIObject::MBackgroundStyle17);
    blockMoveOption(true);

    const QSize &windowSize = M_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    setGeometry(windowSize.width() - 240, windowSize.height() - 150, 220, 100);

    QVBoxLayout *vlayout = new QVBoxLayout(m_container);
    vlayout->setContentsMargins(5, 5, 5, 5);
    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    m_textLabel->setAlignment(Qt::AlignCenter);

    QWidget *contain = new QWidget(this);
    QHBoxLayout *hlayout = new QHBoxLayout(contain);
    hlayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *updateButton = new QPushButton(tr("Update"), contain);
    QPushButton *nextTimeButton = new QPushButton(tr("Close"), contain);
    updateButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    nextTimeButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    updateButton->setFixedSize(85, 27);
    nextTimeButton->setFixedSize(85, 27);
    hlayout->addWidget(updateButton);
    hlayout->addWidget(nextTimeButton);
    contain->setLayout(hlayout);

#ifdef Q_OS_UNIX
    updateButton->setFocusPolicy(Qt::NoFocus);
    nextTimeButton->setFocusPolicy(Qt::NoFocus);
#endif

    vlayout->addWidget(m_textLabel);
    vlayout->addWidget(contain);
    m_container->setLayout(vlayout);

    connect(updateButton, SIGNAL(clicked()), SLOT(updateSourceClicked()));
    connect(nextTimeButton, SIGNAL(clicked()), SLOT(close()));
}

MusicSourceUpdateNotifyWidget::~MusicSourceUpdateNotifyWidget()
{
    delete m_textLabel;
}

void MusicSourceUpdateNotifyWidget::start()
{
    MusicSourceUpdateThread *download = new MusicSourceUpdateThread(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), SLOT(downLoadFinished(QVariant)));
    download->startToDownload();
}

void MusicSourceUpdateNotifyWidget::updateSourceClicked()
{
    close();
    MusicApplicationObject::instance()->musicVersionUpdate();
}

void MusicSourceUpdateNotifyWidget::downLoadFinished(const QVariant &data)
{
    const QVariantMap &value = data.toMap();
    const QString &versionStr = value["version"].toString();

    if(MusicUtils::Core::appVersionCheck(TTKMUSIC_VERSION_STR, versionStr))
    {
        show();
        m_textLabel->setText(tr("New Version Found") + "\r\n" + versionStr);
    }
    else
    {
        close();
    }
}



MusicSourceUpdateWidget::MusicSourceUpdateWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicSourceUpdateWidget)
{
    m_ui->setupUi(this);
    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    m_ui->upgradeButton->setEnabled(false);
#ifdef Q_OS_UNIX
    m_ui->upgradeButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->upgradeFailedLabel, SIGNAL(clicked()), SLOT(upgradeFailedClicked()));
    connect(m_ui->upgradeButton, SIGNAL(clicked()), SLOT(upgradeButtonClicked()));
}

MusicSourceUpdateWidget::~MusicSourceUpdateWidget()
{
    delete m_ui;
}

void MusicSourceUpdateWidget::start()
{
    MusicSourceUpdateThread *download = new MusicSourceUpdateThread(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), SLOT(downLoadFinished(QVariant)));
    download->startToDownload();
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
#ifdef Q_OS_WIN
    m_ui->stackedWidget->setCurrentIndex(SOURCE_UPDATE_INDEX_1);
    const QString &localDwonload = "v" + m_newVersionStr + EXE_FILE;
    const QString &buketUrl = M_SETTING_PTR->value(MusicSettingManager::QiNiuDataConfigChoiced).toString();
    MusicDataDownloadThread *download = new MusicDataDownloadThread(QString("%1%2").arg(MusicUtils::Algorithm::mdII(buketUrl, false)).arg(localDwonload),
                                                                    UPDATE_DIR_FULL + localDwonload, MusicObject::DownloadOther, this);
    connect(download, SIGNAL(downloadProgressChanged(float,QString,qint64)), SLOT(downloadProgressChanged(float,QString)));
    connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downloadProgressFinished()));
    connect(download, SIGNAL(downloadSpeedLabelChanged(QString,qint64)), SLOT(downloadSpeedLabelChanged(QString,qint64)));
    download->startToDownload();
#else
    MusicMessageBox message(this);
    message.setText(tr("Current Platform Not Supported!"));
    message.exec();
#endif
}

void MusicSourceUpdateWidget::upgradeFailedClicked()
{
    MusicUtils::Url::openUrl(MusicUtils::Algorithm::mdII(CSDN_URL, false), false);
}

void MusicSourceUpdateWidget::downLoadFinished(const QVariant &data)
{
    const QVariantMap &value = data.toMap();
    m_newVersionStr = value["version"].toString();

    QString text;
    if(MusicUtils::Core::appVersionCheck(TTKMUSIC_VERSION_STR, m_newVersionStr))
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

void MusicSourceUpdateWidget::downloadSpeedLabelChanged(const QString &speed, qint64 timeLeft)
{
    m_ui->speedLabel->setText(tr("Speed: %1").arg(speed));
    m_ui->timeLabel->setText(tr("Left: %1").arg(MusicTime::normalTime2Label(timeLeft)));
}

void MusicSourceUpdateWidget::downloadProgressChanged(float percent, const QString &total)
{
    m_ui->fileSizeLabel->setText(tr("FileSize: %1").arg(total));
    m_ui->downProgressBar->setValue(percent);
    m_ui->downProgressBarAL->setValue(percent);
}

void MusicSourceUpdateWidget::downloadProgressFinished()
{
    const QString &localDwonload = "v" + m_newVersionStr + EXE_FILE;

    MusicMessageBox message(this);
    message.setText(tr("Download Finish, Install Or Not"));
    if(message.exec())
    {
        MusicUtils::Url::openUrl("open", UPDATE_DIR_FULL+ localDwonload);
        MStatic_cast(QWidget*, parent())->close();
    }
}

int MusicSourceUpdateWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());

    start();

    return MusicAbstractMoveDialog::exec();
}
