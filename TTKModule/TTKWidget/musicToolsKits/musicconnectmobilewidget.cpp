#include "musicconnectmobilewidget.h"
#include "musicconnecttransferwidget.h"
#include "musicanimationstackedwidget.h"
#include "musicwidgetutils.h"
#include "musictoolsetsuiobject.h"
#include "musicuiobject.h"
#include "musicotherdefine.h"

#include "qrencode/qrcodewidget.h"

#ifdef Q_OS_WIN
#  include <qt_windows.h>
#endif

#include <QDebug>
MusicConnectMobileWidget::MusicConnectMobileWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setContentsMargins(0, 0, 0, 0);
    m_stackedWidget = new MusicAnimationStackedWidget(this);
    m_stackedWidget->setLength(320, MusicAnimationStackedWidget::LeftToRight);
    vBox->addWidget(m_stackedWidget);
    setLayout(vBox);

    initFirstWidget();
    initSecondWidget();
    initThirdWidget();

    m_stackedWidget->setCurrentIndex(CONNECT_MOBILE_INDEX_0);
}

MusicConnectMobileWidget::~MusicConnectMobileWidget()
{
    delete m_stackedWidget;
}

void MusicConnectMobileWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicConnectMobileWidget::initFirstWidget()
{
    QWidget *firstWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(firstWidget);
    QLabel *textLabel = new QLabel(tr("Please choose the way"), firstWidget);
    textLabel->setStyleSheet(MusicUIObject::MFontStyle05 + MusicUIObject::MColorStyle12);

    QPushButton *firButton = new QPushButton(tr("Wired Mode"), firstWidget);
    firButton->setStyleSheet(MusicUIObject::MPushButtonStyle11 + MusicUIObject::MColorStyle12);
    firButton->setIcon(QIcon(":/toolSets/lb_wired"));
    firButton->setIconSize(QSize(50, 50));
    firButton->setFixedSize(200, 90);
    connect(firButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetSecond()));

    QPushButton *secButton = new QPushButton(tr("Wirel Mode"), firstWidget);
    secButton->setStyleSheet(MusicUIObject::MPushButtonStyle11 + MusicUIObject::MColorStyle12);
    secButton->setIcon(QIcon(":/toolSets/lb_wireless"));
    secButton->setIconSize(QSize(50, 50));
    secButton->setFixedSize(200, 90);
    connect(secButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetThird()));

#ifdef Q_OS_UNIX
    firButton->setFocusPolicy(Qt::NoFocus);
    secButton->setFocusPolicy(Qt::NoFocus);
#endif

    vBox->addStretch(1);
    vBox->addWidget(textLabel, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(firButton, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(secButton, 0 ,Qt::AlignCenter);
    vBox->addStretch(1);

    firstWidget->setLayout(vBox);

    m_stackedWidget->addWidget(firstWidget);
}

void MusicConnectMobileWidget::initSecondWidget()
{
    QWidget *secondWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(secondWidget);
    ///////////////////////////////////////////////////////
    QWidget *topWidget = new QWidget(secondWidget);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);

    QPushButton *backButton = new QPushButton(tr("< "), topWidget);
    backButton->setStyleSheet(MusicUIObject::MPushButtonStyle12);
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked()), SLOT(changeStatckedWidgetFirst()));

    QPushButton *deviceButton = new QPushButton(topWidget);
    deviceButton->setStyleSheet(MusicUIObject::MKGTransferChangedDev);
    deviceButton->setFixedSize(80, 20);
    deviceButton->setCursor(Qt::PointingHandCursor);

    topWidgetLayout->addWidget(backButton);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(deviceButton);
    ///////////////////////////////////////////////////////

    QLabel *pixLabel = new QLabel(secondWidget);
    pixLabel->setPixmap(QPixmap(":/toolSets/lb_mobile_usb"));

    QLabel *label1 = new QLabel(tr("use Wired Mode"), secondWidget);
    label1->setStyleSheet(MusicUIObject::MFontStyle03);
    ///////////////////////////////////////////////////////
    QWidget *buttonWidget = new QWidget(secondWidget);
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout(buttonWidget);

    QPushButton *importSong = new QPushButton(secondWidget);
    importSong->setStyleSheet(MusicUIObject::MKGTransferSong);
    importSong->setFixedSize(130, 96);
    importSong->setCursor(Qt::PointingHandCursor);
    connect(importSong, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    QPushButton *importRing = new QPushButton(secondWidget);
    importRing->setStyleSheet(MusicUIObject::MKGTransferRing);
    importRing->setFixedSize(130, 96);
    importRing->setCursor(Qt::PointingHandCursor);
    connect(importRing, SIGNAL(clicked()), SLOT(openTransferFiles2Mobile()));

    buttonWidgetLayout->addWidget(importSong);
    buttonWidgetLayout->addWidget(importRing);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    importSong->setFocusPolicy(Qt::NoFocus);
    importRing->setFocusPolicy(Qt::NoFocus);
#endif

    vBox->addWidget(topWidget);
    vBox->addStretch(4);
    vBox->addWidget(pixLabel, 0, Qt::AlignCenter);
    vBox->addWidget(label1, 0, Qt::AlignCenter);
    vBox->addStretch(2);
    vBox->addWidget(buttonWidget, 0, Qt::AlignCenter);
    vBox->addStretch(3);

    secondWidget->setLayout(vBox);

    m_stackedWidget->addWidget(secondWidget);
}

void MusicConnectMobileWidget::initThirdWidget()
{
    QWidget *thirdWidget = new QWidget(this);
    QVBoxLayout *vBox = new QVBoxLayout(thirdWidget);

    QPushButton *backButton = new QPushButton(tr("< "), thirdWidget);
    backButton->setStyleSheet(MusicUIObject::MPushButtonStyle12);
    backButton->setFixedSize(45, 25);
    backButton->setCursor(Qt::PointingHandCursor);
    connect(backButton, SIGNAL(clicked(bool)), SLOT(changeStatckedWidgetFirst()));

    QLabel *label1 = new QLabel(tr("use mobile app to connect"), thirdWidget);
    label1->setStyleSheet(MusicUIObject::MFontStyle05);

    QRCodeQWidget *code = new QRCodeQWidget(QByteArray(), QSize(130, 130), this);
    code->setMargin(8);
    code->setIcon(":/image/lb_app_logo", 0.23);

    QLabel *label2 = new QLabel(tr("\t1. client and app must in the same wifi"), thirdWidget);
    label2->setStyleSheet(MusicUIObject::MFontStyle03);
    QLabel *label3 = new QLabel(tr("\t2. use scanning by mobile app"), thirdWidget);
    label3->setStyleSheet(MusicUIObject::MFontStyle03);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
#endif

    vBox->addWidget(backButton);
    vBox->addStretch(3);
    vBox->addWidget(label1, 0, Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(code, 0, Qt::AlignCenter);
    vBox->addStretch(1);
    vBox->addWidget(label2, 0, Qt::AlignVCenter);
    vBox->addWidget(label3, 0, Qt::AlignVCenter);
    vBox->addStretch(5);

    thirdWidget->setLayout(vBox);

    m_stackedWidget->addWidget(thirdWidget);

    getRemovableDrive();
}

void MusicConnectMobileWidget::changeStatckedWidgetFirst()
{
    m_stackedWidget->start(0);
}

void MusicConnectMobileWidget::changeStatckedWidgetSecond()
{
    m_stackedWidget->start(1);
}

void MusicConnectMobileWidget::changeStatckedWidgetThird()
{
    m_stackedWidget->start(2);
}

void MusicConnectMobileWidget::openTransferFiles2Mobile()
{
    MusicConnectTransferWidget w(this);
    w.exec();
}

bool MusicConnectMobileWidget::GetDisksProperty(const QString &drive)
{
#ifdef Q_OS_WIN
    STORAGE_PROPERTY_QUERY query;
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    QString name = drive;
    name = "\\\\.\\" + name.left(name.size() - 1);

    HANDLE hDevice = CreateFile(
        name.toStdWString().c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        NULL,
        nullptr
    );

    if(hDevice == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    STORAGE_DESCRIPTOR_HEADER *pHeader = (STORAGE_DESCRIPTOR_HEADER *)malloc(sizeof(STORAGE_DESCRIPTOR_HEADER));
    if(nullptr == pHeader)
    {
        return false;
    }

    DWORD bytes;
    BOOL bResult = DeviceIoControl(
        hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(query),
        pHeader,
        sizeof(STORAGE_DESCRIPTOR_HEADER),
        &bytes,
        nullptr
    );

    if(!bResult)
    {
        free(pHeader);
        CloseHandle(hDevice);
        return false;
    }

    PSTORAGE_DEVICE_DESCRIPTOR pDev = (STORAGE_DEVICE_DESCRIPTOR *)malloc(pHeader->Size);
    bResult = DeviceIoControl(
        hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(query),
        pDev,
        pHeader->Size,
        &bytes,
        nullptr
    );

    free(pHeader);
    CloseHandle(hDevice);
    if(pDev->BusType==BusTypeUsb)
    {
        free(pDev);
        return true;
    }

    free(pDev);
    return false;
#else
    return false;
#endif
}

QString MusicConnectMobileWidget::getRemovableDrive()
{
#ifdef Q_OS_WIN
    const QFileInfoList &drives = QDir::drives();
    foreach(const QFileInfo &drive, drives)
    {
        const QString &path = drive.absoluteDir().absolutePath();
        if(GetDriveTypeW(path.toStdWString().c_str()) == DRIVE_REMOVABLE)
        {

        }
        else if(GetDriveTypeW(path.toStdWString().c_str()) == DRIVE_FIXED)
        {
            if(!GetDisksProperty(path))
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        ULARGE_INTEGER freeAvailable;
        ULARGE_INTEGER totalNumberOfBytes;
        ULARGE_INTEGER totalNumberOfFreeBytes;

        if(GetDiskFreeSpaceEx(path.toStdWString().c_str(), &freeAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            qDebug() << path;
            qDebug() << freeAvailable.QuadPart / MH_GB2B; //GB
            qDebug() << totalNumberOfBytes.QuadPart / MH_GB2B; //GB
        }
    }
#endif
    return QString();
}

