#include "musicbackgroundremotewidget.h"
#include "musicdownloadqueuerequest.h"
#include "musicdownloadbingskinrequest.h"
#include "musicdownloadthunderskinrequest.h"
#include "musicdownloadbirdskinrequest.h"
#include "musicextractwrapper.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"

#include <QDir>

MusicBackgroundRemoteWidget::MusicBackgroundRemoteWidget(QWidget *parent)
    : QWidget(parent),
      m_currentIndex(-1),
      m_downloadRequest(nullptr)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    m_backgroundList = new MusicBackgroundListWidget(this);
    hbox->addWidget(m_backgroundList);
    setLayout(hbox);

    m_networkRequest = new MusicDownloadQueueRequest(TTK::Download::Background, this);
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished(QString)));
    connect(m_backgroundList, SIGNAL(itemClicked(int,QString)), parent, SLOT(remoteListWidgetItemClicked(int,QString)));
}

MusicBackgroundRemoteWidget::~MusicBackgroundRemoteWidget()
{
    abort();
    delete m_backgroundList;
    delete m_networkRequest;
    delete m_downloadRequest;
}

void MusicBackgroundRemoteWidget::abort()
{
    m_networkRequest->abort();
}

void MusicBackgroundRemoteWidget::downLoadFinished(const QString &bytes)
{
    if(m_groups.isEmpty())
    {
        return;
    }

    MusicBackgroundImage image;
    outputRemoteSkin(image, bytes);
    if(!image.isValid())
    {
        image.m_pix = QPixmap(":/image/lb_none_image");
    }

    m_backgroundList->updateItem(image, bytes);
}

void MusicBackgroundRemoteWidget::downLoadFinished(const MusicSkinRemoteGroupList &bytes)
{
    m_groups = bytes;
}

void MusicBackgroundRemoteWidget::startToRequest(const QString &prefix)
{
    if(m_groups.isEmpty())
    {
        return;
    }

    const QString &path = QString("%1%2").arg(CACHE_DIR_FULL, m_groups[m_currentIndex].m_group);
    QDir().mkpath(path);

    m_backgroundList->clearItems();

    MusicDownloadQueueDataList datas;
    for(const MusicSkinRemoteItem &item : qAsConst(m_groups[m_currentIndex].m_items))
    {
        m_backgroundList->addCellItem(":/image/lb_none_image", false);
        MusicDownloadQueueData data;
        data.m_url = item.m_url;
        data.m_path = QString("%1/%2%3").arg(path).arg(item.m_index).arg(prefix);
        datas << data;
    }

    m_networkRequest->addQueue(datas);
    m_networkRequest->startToRequest();
}



MusicBackgroundDailyWidget::MusicBackgroundDailyWidget(QWidget *parent)
    : MusicBackgroundRemoteWidget(parent)
{
    m_currentIndex = 0;
    m_backgroundList->setType(MusicBackgroundListWidget::DailyModule);
}

void MusicBackgroundDailyWidget::initialize()
{
    if(!m_downloadRequest)
    {
        m_downloadRequest = new MusicDownloadBingSkinRequest(this);
        connect(m_downloadRequest, SIGNAL(downLoadDataChanged(MusicSkinRemoteGroupList)), SLOT(downLoadFinished(MusicSkinRemoteGroupList)));
        m_downloadRequest->startToRequest();
    }
    else
    {
        startToRequest(TKM_FILE);
    }
}

void MusicBackgroundDailyWidget::outputRemoteSkin(MusicBackgroundImage &image, const QString &data)
{
    if(m_groups.isEmpty() || m_currentIndex < 0)
    {
        return;
    }

    for(const MusicSkinRemoteItem &item : qAsConst(m_groups[m_currentIndex].m_items))
    {
        if(QFileInfo(data).baseName().toInt() == item.m_index)
        {
            image.m_item.m_name = item.m_name;
            image.m_item.m_useCount = item.m_useCount;
            image.m_pix = QPixmap(data);
            break;
        }
    }
}

void MusicBackgroundDailyWidget::downLoadFinished(const MusicSkinRemoteGroupList &bytes)
{
    MusicBackgroundRemoteWidget::downLoadFinished(bytes);
    startToRequest(TKM_FILE);
}



MusicBackgroundOnlineWidget::MusicBackgroundOnlineWidget(QWidget *parent)
    : MusicBackgroundRemoteWidget(parent),
      m_functionsWidget(nullptr)
{
    m_backgroundList->setType(MusicBackgroundListWidget::OnlineModule);
}

MusicBackgroundOnlineWidget::~MusicBackgroundOnlineWidget()
{
    delete m_functionsWidget;
}

void MusicBackgroundOnlineWidget::initialize()
{
    if(m_typeBox->count() != 0)
    {
        m_typeBox->setCurrentIndex(0);
    }

    if(!m_downloadRequest)
    {
        m_downloadRequest = new MusicDownloadThunderSkinRequest(this);
        connect(m_downloadRequest, SIGNAL(downLoadDataChanged(MusicSkinRemoteGroupList)), SLOT(downLoadFinished(MusicSkinRemoteGroupList)));
        m_downloadRequest->startToRequest();
    }
    else
    {
        m_backgroundList->clearItems();
    }
}

QWidget* MusicBackgroundOnlineWidget::createFunctionsWidget(bool revert, QWidget *object)
{
    if(!m_functionsWidget)
    {
        m_functionsWidget = new QWidget(object);
        m_functionsWidget->setGeometry(24, 70, 585, 25);
        m_functionsWidget->hide();

        QHBoxLayout *hbox = new QHBoxLayout(m_functionsWidget);
        hbox->setContentsMargins(9, 0, 0, 9);
        hbox->addStretch(1);

        m_typeBox = new QComboBox(m_functionsWidget);
        m_typeBox->setFixedSize(100, 20);
        m_typeBox->addItem(tr("Select One"));
        m_typeBox->hide();
        TTK::Widget::generateComboBoxFormat(m_typeBox);

        m_allButton = new QPushButton(tr("All"), m_functionsWidget);
        m_allButton->setStyleSheet(TTK::UI::PushButtonStyle02 + QString("QPushButton{ %1 }").arg(TTK::UI::ColorStyle07));
        m_allButton->setCursor(QCursor(Qt::PointingHandCursor));
        m_allButton->setFixedSize(35, 20);
#ifdef Q_OS_UNIX
        m_allButton->setFocusPolicy(Qt::NoFocus);
#endif
        m_allButton->hide();

        m_functionsWidget->setLayout(hbox);
        connect(m_typeBox, SIGNAL(currentIndexChanged(int)), SLOT(currentTypeChanged(int)));
    }

    QHBoxLayout *layout = TTKObjectCast(QHBoxLayout*, m_functionsWidget->layout());
    if(revert)
    {
        layout->removeWidget(m_typeBox);
        layout->addWidget(m_allButton);
        m_allButton->show();
        m_typeBox->hide();
    }
    else
    {
        layout->removeWidget(m_allButton);
        layout->addWidget(m_typeBox);
        m_allButton->hide();
        m_typeBox->show();
    }

    return m_functionsWidget;
}

void MusicBackgroundOnlineWidget::outputRemoteSkin(MusicBackgroundImage &image, const QString &data)
{
    if(m_groups.isEmpty() || m_currentIndex < 0)
    {
        return;
    }

    const int index = QFileInfo(data).baseName().toInt();
    MusicSkinRemoteItemList &items = m_groups[m_currentIndex].m_items;
    if(index >= 0 || index < items.count())
    {
        MusicSkinRemoteItem &item = items[index];
        image.m_item.m_name = item.m_name;
        image.m_item.m_useCount = item.m_useCount;
        MusicExtractWrapper::outputThunderSkin(image.m_pix, data);
    }
}

void MusicBackgroundOnlineWidget::currentTypeChanged(int index)
{
    index -= 1; // remove first index because it is tips
    if(index < 0 || index >= m_groups.count())
    {
        return;
    }

    if(m_currentIndex != index)
    {
        m_networkRequest->abort();
    }

    m_currentIndex = index;
    startToRequest(TKM_FILE);
}

void MusicBackgroundOnlineWidget::downLoadFinished(const MusicSkinRemoteGroupList &bytes)
{
    MusicBackgroundRemoteWidget::downLoadFinished(bytes);
    m_typeBox->blockSignals(true);
    for(int i = 0; i < bytes.count(); ++i)
    {
        QString title(m_groups[i].m_group);
        m_typeBox->addItem(title.remove(SKIN_THUNDER_DIR));
    }

    m_typeBox->setCurrentIndex(0);
    m_typeBox->blockSignals(false);
}
