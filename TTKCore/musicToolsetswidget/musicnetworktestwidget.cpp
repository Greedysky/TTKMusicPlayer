#include "musicnetworktestwidget.h"
#include "ui_musicnetworktestwidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"
#include "musicnetworktestthread.h"
#include "musicutils.h"
#include "musicdatadownloadthread.h"
#include "musicmessagebox.h"

MusicNetworkTestWidget::MusicNetworkTestWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicNetworkTestWidget), m_thead(nullptr), m_testDownload(nullptr)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_totalUp = 0;
    m_totalDown = 0;
    m_testAverage = 0;
    ui->speedWidget->setAnimating(false);
    ui->speedWidget->setRatio(50);

    ui->suspensionButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->suspensionButton->setCursor(QCursor((Qt::PointingHandCursor)));
    ui->testButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->testButton->setCursor(QCursor((Qt::PointingHandCursor)));
    connect(ui->suspensionButton, SIGNAL(clicked()), SLOT(suspensionOpen()));
    connect(ui->testButton, SIGNAL(clicked()), SLOT(networkTestStart()));

    m_thead = new MusicNetworkTestThread(this);
    connect(m_thead, SIGNAL(networkData(ulong,ulong)), SLOT(networkData(ulong,ulong)));
    m_thead->start();

    m_testTimer.setInterval(5*1000);
    connect(&m_testTimer, SIGNAL(timeout()), SLOT(networkTestStop()));
}

MusicNetworkTestWidget::~MusicNetworkTestWidget()
{
    m_testTimer.stop();
    m_thead->stopAndQuitThread();
    delete m_thead;
    delete m_testDownload;
    delete ui;
}

void MusicNetworkTestWidget::networkData(ulong upload, ulong download)
{
    m_totalUp += upload/5;
    m_totalDown += download/5;

    ui->uploadSpeedValue->setText(MusicUtils::speed2Label(upload));
    ui->downloadSpeedValue->setText(MusicUtils::speed2Label(download));
    ui->uploadAllSpeedValue->setText(MusicUtils::speed2Label(m_totalUp));
    ui->downloadAllSpeedValue->setText(MusicUtils::speed2Label(m_totalDown));

    if(m_testTimer.isActive())
    {
        int value = MusicUtils::sizeByte2KByte(download/5);
        if(value > 100*ui->speedWidget->ratio())
        {
            value = 100*ui->speedWidget->ratio();
        }
        m_testAverage += value;
        ui->speedWidget->setValue(value);
    }
}

void MusicNetworkTestWidget::suspensionOpen()
{

}

void MusicNetworkTestWidget::networkTestStart()
{
    ui->testButton->setEnabled(false);
    m_testTimer.stop();
    delete m_testDownload;
    m_testDownload = new MusicDataDownloadThread(testUrl, testName,
                             MusicDownLoadThreadAbstract::Download_BigBG, this);
    m_testDownload->startToDownload();
    m_testTimer.start();
}

void MusicNetworkTestWidget::networkTestStop()
{
    delete m_testDownload;
    m_testDownload = NULL;
    m_testTimer.stop();
    ///remove temp file
    QFile::remove(testName);
    ui->testButton->setEnabled(true);

    MusicMessageBox message(this);
    message.setText(tr("Average is %1 kb/s").arg(m_testAverage/5));
    message.exec();
}

int MusicNetworkTestWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
