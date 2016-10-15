#include "musicnetworktestwidget.h"
#include "ui_musicnetworktestwidget.h"
#include "musicuiobject.h"
#include "musicnetworktestthread.h"
#include "musicnumberutils.h"
#include "musicdatadownloadthread.h"
#include "musicmessagebox.h"
#include "musicnetworksuspensionwidget.h"

#include <QMenu>
#include <QActionGroup>

MusicNetworkTestWidget::MusicNetworkTestWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicNetworkTestWidget), m_thread(nullptr), m_testDownload(nullptr),
      m_suspension(nullptr)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_totalUp = 0;
    m_totalDown = 0;
    m_testAverage = 0;
    ui->speedWidget->setAnimating(false);
    ui->speedWidget->setRatio(50);

    ui->suspensionButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->suspensionButton->setCursor(QCursor((Qt::PointingHandCursor)));
    ui->settingButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->settingButton->setCursor(QCursor((Qt::PointingHandCursor)));
    ui->testButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->testButton->setCursor(QCursor((Qt::PointingHandCursor)));
    connect(ui->suspensionButton, SIGNAL(clicked()), SLOT(suspensionOpen()));
    connect(ui->testButton, SIGNAL(clicked()), SLOT(networkTestStart()));

    m_thread = new MusicNetworkTestThread(this);
    connect(m_thread, SIGNAL(networkData(ulong,ulong)), SLOT(networkData(ulong,ulong)));
    m_thread->start();

    settingButton();

    m_testTimer.setInterval(5*MT_S2MS);
    connect(&m_testTimer, SIGNAL(timeout()), SLOT(networkTestStop()));
}

MusicNetworkTestWidget::~MusicNetworkTestWidget()
{
    delete m_actionGroup;
    m_testTimer.stop();
    delete m_thread;
    delete m_testDownload;
    delete ui;
}

QString MusicNetworkTestWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkTestWidget::settingButton()
{
    m_actionGroup = new QActionGroup(this);
    QMenu *menu = new QMenu(this);
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QStringList list = m_thread->getNewtworkNames();
    foreach(const QString &var, list)
    {
        m_actionGroup->addAction(menu->addAction(var));
    }

    ui->settingButton->setMenu(menu);
    connect(m_actionGroup, SIGNAL(triggered(QAction*)), SLOT(actionTriggered(QAction*)));
}

void MusicNetworkTestWidget::networkData(ulong upload, ulong download)
{
    m_totalUp += upload;
    m_totalDown += download;

    ui->uploadSpeedValue->setText(MusicUtils::Number::speed2Label(upload));
    ui->downloadSpeedValue->setText(MusicUtils::Number::speed2Label(download));
    ui->uploadAllSpeedValue->setText(MusicUtils::Number::speed2Label(m_totalUp));
    ui->downloadAllSpeedValue->setText(MusicUtils::Number::speed2Label(m_totalDown));

    if(m_testTimer.isActive())
    {
        int value = MusicUtils::Number::sizeByte2KByte(download);
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
    delete m_suspension;
    m_suspension = new MusicNetworkSuspensionWidget;
    m_suspension->setAvailableNewtworkNames(m_thread->getAvailableNewtworkNames());
    m_suspension->show();
}

void MusicNetworkTestWidget::networkTestStart()
{
    ui->testButton->setEnabled(false);
    m_testTimer.stop();
    delete m_testDownload;
    m_testDownload = new MusicDataDownloadThread(MusicCryptographicHash::decryptData(testUrl, URL_KEY), testName,
                             MusicDownLoadThreadAbstract::Download_BigBG, this);
    m_testDownload->startToDownload();
    m_testTimer.start();
}

void MusicNetworkTestWidget::actionTriggered(QAction *action)
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

void MusicNetworkTestWidget::networkTestStop()
{
    delete m_testDownload;
    m_testDownload = nullptr;
    m_testTimer.stop();
    ///remove temp file
    QFile::remove(testName);
    ui->testButton->setEnabled(true);

    MusicMessageBox message(this);
    message.setText(tr("Average is %1 kb/s").arg(m_testAverage/5));
    message.exec();
    m_testAverage = 0;
}

void MusicNetworkTestWidget::show()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveWidget::show();
}
