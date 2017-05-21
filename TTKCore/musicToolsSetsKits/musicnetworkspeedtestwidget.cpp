#include "musicnetworkspeedtestwidget.h"
#include "ui_musicnetworkspeedtestwidget.h"
#include "musicuiobject.h"
#include "musicnetworkspeedtestthread.h"
#include "musicnumberutils.h"
#include "musicdatadownloadthread.h"
#include "musicmessagebox.h"
#include "musicnetworkthread.h"
#include "musicnetworkspeedsuspensionwidget.h"

#include <QMenu>
#include <QActionGroup>

MusicNetworkSpeedTestWidget::MusicNetworkSpeedTestWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicNetworkSpeedTestWidget), m_thread(nullptr), m_testDownload(nullptr),
      m_suspension(nullptr)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_totalUp = 0;
    m_totalDown = 0;
    m_testAverage = 0;
    m_ui->speedWidget->setAnimating(false);
    m_ui->speedWidget->setRatio(50);

    m_ui->suspensionButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->suspensionButton->setCursor(QCursor((Qt::PointingHandCursor)));
    m_ui->settingButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->settingButton->setCursor(QCursor((Qt::PointingHandCursor)));
    m_ui->testButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->testButton->setCursor(QCursor((Qt::PointingHandCursor)));
    connect(m_ui->suspensionButton, SIGNAL(clicked()), SLOT(suspensionOpen()));
    connect(m_ui->testButton, SIGNAL(clicked()), SLOT(networkTestStart()));

    m_thread = new MusicNetworkSpeedTestThread(this);
    connect(m_thread, SIGNAL(networkData(ulong,ulong)), SLOT(networkData(ulong,ulong)));
    m_thread->start();

    settingButton();

    m_testTimer.setInterval(5*MT_S2MS);
    connect(&m_testTimer, SIGNAL(timeout()), SLOT(networkTestStop()));
}

MusicNetworkSpeedTestWidget::~MusicNetworkSpeedTestWidget()
{
    delete m_actionGroup;
    m_testTimer.stop();
    delete m_thread;
    delete m_testDownload;
    delete m_ui;
}

QString MusicNetworkSpeedTestWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkSpeedTestWidget::settingButton()
{
    m_actionGroup = new QActionGroup(this);
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QStringList list = m_thread->getNewtworkNames();
    foreach(const QString &var, list)
    {
        m_actionGroup->addAction(menu->addAction(var));
    }

    m_ui->settingButton->setMenu(menu);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(actionTriggered(QAction*)));
}

void MusicNetworkSpeedTestWidget::networkData(ulong upload, ulong download)
{
    m_totalUp += upload;
    m_totalDown += download;

    m_ui->uploadSpeedValue->setText(MusicUtils::Number::speed2Label(upload));
    m_ui->downloadSpeedValue->setText(MusicUtils::Number::speed2Label(download));
    m_ui->uploadAllSpeedValue->setText(MusicUtils::Number::speed2Label(m_totalUp));
    m_ui->downloadAllSpeedValue->setText(MusicUtils::Number::speed2Label(m_totalDown));

    if(m_testTimer.isActive())
    {
        int value = MusicUtils::Number::sizeByte2KByte(download);
        if(value > 100*m_ui->speedWidget->ratio())
        {
            value = 100*m_ui->speedWidget->ratio();
        }
        m_testAverage += value;
        m_ui->speedWidget->setValue(value);
    }
}

void MusicNetworkSpeedTestWidget::suspensionOpen()
{
    delete m_suspension;
    m_suspension = new MusicNetworkSpeedSuspensionWidget;
    m_suspension->setAvailableNewtworkNames(m_thread->getAvailableNewtworkNames());
    m_suspension->show();
}

void MusicNetworkSpeedTestWidget::networkTestStart()
{
    m_ui->testButton->setEnabled(false);
    m_testTimer.stop();
    delete m_testDownload;
    m_testDownload = new MusicDataDownloadThread(MusicCryptographicHash::decryptData(testUrl, URL_KEY), testName,
                         MusicDownLoadThreadAbstract::Download_BigBG, this);
    if(M_NETWORK_PTR->isOnline())
    {
        m_testDownload->startToDownload();
    }
    m_testTimer.start();
}

void MusicNetworkSpeedTestWidget::actionTriggered(QAction *action)
{
    if(action->icon().isNull())
    {
        action->setIcon(QIcon(":/contextMenu/btn_selected"));
    }
    else
    {
        action->setIcon(QIcon());
    }

    QStringList selected;
    QList<QAction*> actions = m_actionGroup->actions();
    foreach(QAction *action, actions)
    {
#if defined Q_OS_WIN
        if(!action->icon().isNull())
        {
            selected << action->text();
        }
    }
#elif defined Q_OS_UNIX
        action->setIcon(QIcon());
    }
    action->setIcon(QIcon(":/contextMenu/btn_selected"));
    selected << action->text();
#endif
    m_thread->setAvailableNewtworkNames(selected);
    if(m_suspension)
    {
        m_suspension->setAvailableNewtworkNames(selected);
    }
}

void MusicNetworkSpeedTestWidget::networkTestStop()
{
    delete m_testDownload;
    m_testDownload = nullptr;
    m_testTimer.stop();
    ///remove temp file
    QFile::remove(testName);
    m_ui->testButton->setEnabled(true);

    MusicMessageBox message(this);
    message.setText(tr("Average is %1 kb/s").arg(m_testAverage/5));
    message.exec();
    m_testAverage = 0;
}

void MusicNetworkSpeedTestWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveWidget::show();
}
