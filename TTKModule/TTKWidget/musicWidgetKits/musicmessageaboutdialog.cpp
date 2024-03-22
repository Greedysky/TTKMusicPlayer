#include "musicmessageaboutdialog.h"
#include "ui_musicmessageaboutdialog.h"
#include "musicpvcounterrequest.h"
#include "ttkversion.h"

MusicMessageAboutDialog::MusicMessageAboutDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicMessageAboutDialog)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->confirmButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(close()));

    m_networkRequest = new MusicPVCounterRequest(this);
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(downloadFinished(QString)));
    m_networkRequest->startToRequest();

    const QString &buildTime = QString("%1 %2").arg(__DATE__, __TIME__);
    const QByteArray &md5 = TTK::Algorithm::md5(buildTime.toUtf8()).mid(12, 8);

    m_ui->iconLabel->setPixmap(QPixmap(":/image/lb_logo"));
    m_ui->versionLabel->setText(QString("Version: %1\n").arg(TTK_VERSION_STR) +
                                QString("Hash: %1\n").arg(TTK::Algorithm::sha1(TTK_VERSION_STR).toHex().constData()) +
                                QString("Built on %1 (%2)\n").arg(buildTime, md5.constData()) +
                                QString("Based on Qt %1(%2)\n").arg(QT_VERSION_STR,
#ifdef Q_OS_WIN
                                "MinGW 32bit"));
#else
                                "GCC x86_64"));
#endif
}

MusicMessageAboutDialog::~MusicMessageAboutDialog()
{
    delete m_networkRequest;
    delete m_ui;
}

void MusicMessageAboutDialog::downloadFinished(const QString &bytes)
{
    m_ui->counterLabel->setText(tr("Counter: %1").arg(bytes));
}
