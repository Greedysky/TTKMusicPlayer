#include "musicnetworktestwidget.h"
#include "ui_musicnetworktestwidget.h"
#include "musicbgthememanager.h"
#include "musicuiobject.h"
#include "musicnetworktestthread.h"
#include "musicutils.h"

MusicNetworkTestWidget::MusicNetworkTestWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicNetworkTestWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_totalUp = 0;
    m_totalDown = 0;

    ui->suspensionButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->suspensionButton->setCursor(QCursor((Qt::PointingHandCursor)));
    ui->testButton->setStyleSheet(MusicUIObject::MPushButtonStyle06);
    ui->testButton->setCursor(QCursor((Qt::PointingHandCursor)));
    connect(ui->suspensionButton, SIGNAL(clicked()), SLOT(suspensionOpen()));
    connect(ui->testButton, SIGNAL(clicked()), SLOT(networkTestStart()));

    m_thead = new MusicNetworkTestThread(this);
    connect(m_thead, SIGNAL(networkData(long,long)), SLOT(networkData(long,long)));
    m_thead->start();
}

MusicNetworkTestWidget::~MusicNetworkTestWidget()
{
    m_thead->stopAndQuitThread();
    delete m_thead;
    delete ui;
}

void MusicNetworkTestWidget::networkData(long upload, long download)
{
    m_totalUp += upload;
    m_totalDown += download;

    ui->uploadSpeedValue->setText(MusicUtils::speed2Label(upload));
    ui->downloadSpeedValue->setText(MusicUtils::speed2Label(download));
    ui->uploadAllSpeedValue->setText(MusicUtils::speed2Label(m_totalUp));
    ui->downloadAllSpeedValue->setText(MusicUtils::speed2Label(m_totalDown));
}

void MusicNetworkTestWidget::suspensionOpen()
{

}

void MusicNetworkTestWidget::networkTestStart()
{

}

int MusicNetworkTestWidget::exec()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    return MusicAbstractMoveDialog::exec();
}
