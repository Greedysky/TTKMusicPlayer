#include "musicmessageaboutdialog.h"
#include "ui_musicmessageaboutdialog.h"
#include "ttkversion.h"
#include "musicuiobject.h"
#include "musicdownloadcounterpvthread.h"

MusicMessageAboutDialog::MusicMessageAboutDialog(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicMessageAboutDialog)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->confirmButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->confirmButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_ui->confirmButton->setFocusPolicy(Qt::NoFocus);
#endif

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->confirmButton, SIGNAL(clicked()), SLOT(close()));

    //
    m_counterPVThread = new MusicDownloadCounterPVThread(this);
    connect(m_counterPVThread, SIGNAL(downLoadDataChanged(QString)), SLOT(musicGetCounterFinished(QString)));
    m_counterPVThread->startToDownload();
    //
}

MusicMessageAboutDialog::~MusicMessageAboutDialog()
{
    delete m_counterPVThread;
    delete m_ui;
}

void MusicMessageAboutDialog::musicGetCounterFinished(const QString &data)
{
    m_ui->counterLabel->setText(tr("Counter: %1").arg(data));
}

int MusicMessageAboutDialog::exec()
{
    m_ui->iconLabel->setPixmap(QPixmap(":/image/lb_logo"));
    m_ui->textLabel->setText(QString("TTKMusicplayer Version: %1\n").arg(TTKMUSIC_VERSION_STR) + m_ui->textLabel->text());
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
