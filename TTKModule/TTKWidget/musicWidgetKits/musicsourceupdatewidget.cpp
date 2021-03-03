#include "musicsourceupdatewidget.h"
#include "ui_musicsourceupdatewidget.h"
#include "musicsourceupdaterequest.h"
#include "musicdownloaddatarequest.h"
#include "musicmessagebox.h"
#include "musictoastlabel.h"
#include "musicuiobject.h"
#include "musiccoreutils.h"
#include "musicurlutils.h"
#include "musicsettingmanager.h"
#include "musicsourceupdatewidget.h"

#include "qsync/qsyncutils.h"

#include <QBoxLayout>

MusicSourceUpdateNotifyWidget::MusicSourceUpdateNotifyWidget(QWidget *parent)
    : MusicAbstractMoveSingleWidget(true, parent)
{
    hide();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, true);
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle17);
    blockMoveOption(true);

    const QSize &windowSize = G_SETTING_PTR->value(MusicSettingManager::ScreenSize).toSize();
    setGeometry(windowSize.width() - 240, windowSize.height() - 150, 220, 100);

    QVBoxLayout *vlayout = new QVBoxLayout(m_container);
    vlayout->setContentsMargins(5, 5, 5, 5);
    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(MusicUIObject::MQSSColorStyle03);
    m_textLabel->setAlignment(Qt::AlignCenter);

    QWidget *contain = new QWidget(this);
    QHBoxLayout *hlayout = new QHBoxLayout(contain);
    hlayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *updateButton = new QPushButton(tr("Update"), contain);
    QPushButton *nextTimeButton = new QPushButton(tr("Close"), contain);
    updateButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    nextTimeButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
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
    MusicSourceUpdateRequest *download = new MusicSourceUpdateRequest(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), SLOT(downLoadFinished(QVariant)));
    download->startToDownload();
}

void MusicSourceUpdateNotifyWidget::updateSourceClicked()
{
    close();
    MusicSourceUpdateWidget().exec();
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
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->downProgressBar->setStyleSheet(MusicUIObject::MQSSProgressBar01);
    m_ui->upgradeButton->setEnabled(false);
#ifdef Q_OS_UNIX
    m_ui->upgradeButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->upgradeButton, SIGNAL(clicked()), SLOT(upgradeButtonClicked()));
    connect(m_ui->cancelButton, SIGNAL(clicked()), SLOT(close()));
}

MusicSourceUpdateWidget::~MusicSourceUpdateWidget()
{
    delete m_ui;
}

void MusicSourceUpdateWidget::start()
{
    MusicSourceUpdateRequest *download = new MusicSourceUpdateRequest(this);
    connect(download, SIGNAL(downLoadDataChanged(QVariant)), SLOT(downLoadFinished(QVariant)));
    download->startToDownload();
}

void MusicSourceUpdateWidget::upgradeButtonClicked()
{
#ifdef Q_OS_WIN
    m_ui->stackedWidget->setCurrentIndex(SOURCE_UPDATE_INDEX_1);
    const QString &localDwonload = "v" + m_newVersionStr + EXE_FILE;
    MusicDownloadDataRequest *download = new MusicDownloadDataRequest(QString("%1%2").arg(QSyncUtils::generateDataBucketUrl()).arg(localDwonload),
                                                                    UPDATE_DIR_FULL + localDwonload, MusicObject::DownloadOther, this);
    connect(download, SIGNAL(downloadProgressChanged(float,QString,qint64)), SLOT(downloadProgressChanged(float,QString)));
    connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downloadProgressFinished()));
    connect(download, SIGNAL(downloadSpeedLabelChanged(QString,qint64)), SLOT(downloadSpeedLabelChanged(QString,qint64)));
    download->startToDownload();
#else
    MusicToastLabel::popup(tr("Current Platform Not Supported!"));
#endif
}

void MusicSourceUpdateWidget::downLoadFinished(const QVariant &data)
{
    const QVariantMap &value = data.toMap();
    m_newVersionStr = value["version"].toString();

    QString text;
    if(MusicUtils::Core::appVersionCheck(TTKMUSIC_VERSION_STR, m_newVersionStr))
    {
        text.append("v" + m_newVersionStr);
        text.append("\r\n");
        text.append(value["data"].toString());
        m_ui->upgradeButton->setEnabled(true);
        m_ui->titleLable->move(50, 0);
        m_ui->titleLable->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    }
    else
    {
        text.append(tr("Current version is updated!"));
        m_ui->titleLable->move(0, 0);
        m_ui->titleLable->setAlignment(Qt::AlignCenter);
    }
    m_ui->titleLable->setText(text);
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
}

void MusicSourceUpdateWidget::downloadProgressFinished()
{
    const QString &localDwonload = "v" + m_newVersionStr + EXE_FILE;

    MusicMessageBox message(this);
    message.setText(tr("Download Finish, Install Or Not"));
    if(message.exec())
    {
        MusicUtils::Url::openUrl("open", UPDATE_DIR_FULL+ localDwonload);
        TTKStatic_cast(QWidget*, parent())->close();
    }
}

int MusicSourceUpdateWidget::exec()
{
    start();

    return MusicAbstractMoveDialog::exec();
}
