#include "musiccloudmanagerwidget.h"
#include "musicdownloadsourcerequest.h"
#include "musicitemdelegate.h"
#include "musicopenfilewidget.h"
#include "musicsettingmanager.h"
#include "musiccloudfileinformationwidget.h"
#include "musicdownloaddatarequest.h"
#include "musiccoreutils.h"
#include "musicfileutils.h"
#include "musicstringutils.h"
#include "musicformats.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"
#include "musiccloudtablewidget.h"

#include "qsync/qsyncconfig.h"
#include "qsync/qsynclistdata.h"
#include "qsync/qsyncuploaddata.h"
#include "qsync/qsyncdeletedata.h"
#include "qsync/qsyncdownloaddata.h"

#define OS_CLOUD_URL        "cloud"

Q_DECLARE_METATYPE(MusicCloudDataItem)

MusicCloudManagerTableWidget::MusicCloudManagerTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicCloudUploadTableWidget::className());

    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 360);
    headerview->resizeSection(2, 110);
    headerview->resizeSection(3, 50);
    headerview->resizeSection(4, 120);

    m_uploading = false;
    m_cancel = false;
    m_openFileWidget = nullptr;
    m_totalFileSzie = 0;

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle03);

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_progressBarDelegate);

    m_manager = new QNetworkAccessManager(this);
    m_syncListData = new QSyncListData(m_manager, this);
    m_syncDeleteData = new QSyncDeleteData(m_manager, this);
    m_syncUploadData = new QSyncUploadData(m_manager, this);
    m_syncDownloadData = new QSyncDownloadData(m_manager, this);

    connect(m_syncListData, SIGNAL(receiveFinshed(QSyncDataItemList)), SLOT(receiveDataFinshed(QSyncDataItemList)));
    connect(m_syncDeleteData, SIGNAL(deleteFileFinished(bool)), SLOT(deleteFileFinished(bool)));
    connect(m_syncUploadData, SIGNAL(uploadFileFinished(QString)), SLOT(uploadFileFinished(QString)));
}

MusicCloudManagerTableWidget::~MusicCloudManagerTableWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    delete m_syncListData;
    delete m_syncDeleteData;
    delete m_syncUploadData;
    delete m_syncDownloadData;
    delete m_manager;
    delete m_openFileWidget;
    delete m_progressBarDelegate;
}

bool MusicCloudManagerTableWidget::queryCloudKey()
{
    MusicSemaphoreLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

    MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
    connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(QSyncUtils::makeDataBucketUrl() + OS_CLOUD_URL);

    loop.exec();
    updateListToServer();

    return !QSyncConfig::NAME.isEmpty() && !QSyncConfig::KEY.isEmpty();
}

void MusicCloudManagerTableWidget::resizeWindow()
{
    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (width - WINDOW_WIDTH_MIN) + 360);

    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));
    }

    if(m_openFileWidget)
    {
        m_openFileWidget->adjustWidgetRect(this->width(), height());
    }
}

void MusicCloudManagerTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudManagerTableWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_LOGGER_ERROR("Input byte data is empty");
    }
    else
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            QSyncConfig::NAME = value["key"].toString();
            QSyncConfig::KEY = value["secret"].toByteArray();
        }
    }
    Q_EMIT finished();
}

void MusicCloudManagerTableWidget::receiveDataFinshed(const QSyncDataItemList &items)
{
    clear();
    m_totalFileSzie = 0;

    const int count = items.count();
    if(count == 0)
    {
        Q_EMIT updateLabelMessage(tr("List is empty!"));
        createUploadFileModule();
        return;
    }

    for(const QSyncDataItem &item : qAsConst(items))
    {
        MusicCloudDataItem data;
        data.m_id = QString::number(MusicTime::timestamp());
        data.m_path = item.m_name.trimmed();
        data.m_state = MusicCloudDataItem::Successed;
        data.m_dataItem = item;
        m_totalFileSzie += item.m_size;

        createItem(data);
    }

    Q_EMIT updateLabelMessage(tr("List update finished!"));
    Q_EMIT updataSizeLabel(m_totalFileSzie);
}

void MusicCloudManagerTableWidget::uploadFileFinished(const QString &time)
{
    if(time == TTK_NAN_STR)
    {
        Q_EMIT uploadFileError(m_currentDataItem);
    }

    const int row = FindUploadItemRow(time);
    if(row != -1)
    {
        QTableWidgetItem *it = item(row, 0);
        if(it)
        {
            MusicCloudDataItem data = it->data(MUSIC_DATA_ROLE).value<MusicCloudDataItem>();
            data.m_state = MusicCloudDataItem::Successed;
            it->setData(MUSIC_DATA_ROLE, QVariant::fromValue<MusicCloudDataItem>(data));
            m_totalFileSzie += data.m_dataItem.m_size;
        }
        Q_EMIT updataSizeLabel(m_totalFileSzie);
    }
    else
    {
        Q_EMIT uploadFileError(m_currentDataItem);
    }

    startToUploadFile();
}

void MusicCloudManagerTableWidget::deleteFileFinished(bool state)
{
    Q_EMIT updateLabelMessage(state ? tr("Delete current file success!") : tr("Delete current file error!"));
}

void MusicCloudManagerTableWidget::updateListToServer()
{
    Q_EMIT updateLabelMessage(tr("List updating"));
    m_syncListData->listDataOperator(MUSIC_BUCKET);
}

void MusicCloudManagerTableWidget::deleteFileToServer()
{
    if(currentRow() < 0 || m_uploading)
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    const MusicCloudDataItem &data = it->data(MUSIC_DATA_ROLE).value<MusicCloudDataItem>();
    removeRow(currentRow());
    m_syncDeleteData->deleteDataOperator(MUSIC_BUCKET, data.m_dataItem.m_name);
    m_totalFileSzie -= data.m_dataItem.m_size;
    Q_EMIT updataSizeLabel(m_totalFileSzie);

    createUploadFileModule();
}

void MusicCloudManagerTableWidget::deleteFilesToServer()
{
    if(m_uploading)
    {
        return;
    }

    selectAll();
    const TTKIntList deleteList(multiSelectedIndex());

    for(int i = deleteList.count() - 1; i >= 0; --i)
    {
        const int index = deleteList[i];
        QTableWidgetItem *it = item(index, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(MUSIC_DATA_ROLE).value<MusicCloudDataItem>();
        removeRow(index);
        m_syncDeleteData->deleteDataOperator(MUSIC_BUCKET, data.m_dataItem.m_name);

        m_totalFileSzie -= data.m_dataItem.m_size;
        Q_EMIT updataSizeLabel(m_totalFileSzie);
    }
}

void MusicCloudManagerTableWidget::downloadFileToServer()
{
    if(currentRow() < 0)
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    const MusicCloudDataItem &data = it->data(MUSIC_DATA_ROLE).value<MusicCloudDataItem>();
    const QString &url = m_syncDownloadData->downloadUrl(MUSIC_BUCKET, data.m_dataItem.m_name);

    MusicDownloadDataRequest *download = new MusicDownloadDataRequest(url, MusicUtils::String::musicPrefix() + data.m_dataItem.m_name, MusicObject::DownloadMusic, this);
    download->setRecordType(MusicObject::RecordCloudDownload);
    download->startToDownload();
}

void MusicCloudManagerTableWidget::cancelUploadFilesToServer()
{
    m_cancel = true;
}

void MusicCloudManagerTableWidget::uploadFilesToServer()
{
    const QStringList &files = MusicUtils::File::openFilesDialog(this, MusicFormats::supportMusicInputFormats());
    uploadFilesToServer(files);
}

void MusicCloudManagerTableWidget::uploadFileDirToServer()
{
    const QString &path = MusicUtils::File::openDirectoryDialog(this);
    if(!path.isEmpty())
    {
        delete m_openFileWidget;
        m_openFileWidget = nullptr;

        for(const QFileInfo &fin : MusicUtils::File::fileListByPath(path, MusicFormats::supportMusicInputFilterFormats()))
        {
            MusicCloudDataItem item;
            item.m_id = QString::number(MusicTime::timestamp());
            item.m_path = path;
            item.m_state = MusicCloudDataItem::Waited;
            item.m_dataItem.m_name = fin.fileName().trimmed();
            item.m_dataItem.m_putTime = fin.lastModified().toString(MUSIC_YEAR_TIME_FORMAT);
            item.m_dataItem.m_size = fin.size();

            createItem(item);
        }

        if(!m_uploading)
        {
            startToUploadFile();
        }
    }

    createUploadFileModule();
}

void MusicCloudManagerTableWidget::reuploadFilesToServer(const QStringList &items)
{
    uploadFilesToServer(items);
}

void MusicCloudManagerTableWidget::uploadProgress(const QString &time, qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal != 0)
    {
        const int value = TTKStatic_cast(int, (bytesSent*1.0/bytesTotal) * 100);
        const int row = FindUploadItemRow(time);
        if(row != -1)
        {
            QTableWidgetItem *it = item(row, 2);
            if(it)
            {
                it->setData(MUSIC_PROGRESS_ROLE, value);
            }
        }
    }
}

void MusicCloudManagerTableWidget::uploadDone()
{
    m_uploading = false;
    m_currentDataItem.clear();
    updateListToServer();
}

void MusicCloudManagerTableWidget::showFileInformationWidget()
{
    if(currentRow() < 0)
    {
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    MusicCloudFileInformationWidget w;
    MusicCloudDataItem data(it->data(MUSIC_DATA_ROLE).value<MusicCloudDataItem>());
    w.setFileInformation(&data.m_dataItem);
    w.exec();
}

void MusicCloudManagerTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    QMenu uploadMenu(tr("Upload"), &menu);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    if(m_currentDataItem.isValid() && m_currentDataItem.m_state == MusicCloudDataItem::Waited)
    {
        uploadMenu.addAction(tr("Cancel Upload"), this, SLOT(cancelUploadFilesToServer()));
    }

    uploadMenu.addAction(tr("Upload File"), this, SLOT(uploadFilesToServer()));
    uploadMenu.addAction(tr("Upload Files"), this, SLOT(uploadFileDirToServer()));
    MusicUtils::Widget::adjustMenuPosition(&uploadMenu);

    menu.addMenu(&uploadMenu);
    menu.addAction(tr("Delete File"), this, SLOT(deleteFileToServer()))->setEnabled(!m_uploading);
    menu.addAction(tr("Delete Files"), this, SLOT(deleteFilesToServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("Download"), this, SLOT(downloadFileToServer()))->setEnabled(!m_uploading);
    menu.addAction(tr("Update List"), this, SLOT(updateListToServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformationWidget()));

    MusicUtils::Widget::adjustMenuPosition(&menu);
    menu.exec(QCursor::pos());
}

void MusicCloudManagerTableWidget::uploadFilesToServer(const QStringList &paths)
{
    if(paths.isEmpty())
    {
        return;
    }

    delete m_openFileWidget;
    m_openFileWidget = nullptr;

    for(const QString &path : qAsConst(paths))
    {
        MusicCloudDataItem item;
        const QFileInfo fin(path);
        item.m_id = QString::number(MusicTime::timestamp());
        item.m_path = path;
        item.m_state = MusicCloudDataItem::Waited;
        item.m_dataItem.m_name = fin.fileName().trimmed();
        item.m_dataItem.m_putTime = fin.lastModified().toString(MUSIC_YEAR_TIME_FORMAT);
        item.m_dataItem.m_size = fin.size();

        MusicUtils::Core::sleep(MT_MS);

        createItem(item);
    }

    if(!m_uploading)
    {
        startToUploadFile();
    }
}

void MusicCloudManagerTableWidget::createUploadFileModule()
{
    if(rowCount() != 0)
    {
        return;
    }

    if(m_openFileWidget == nullptr)
    {
        m_openFileWidget = new MusicOpenFileWidget(this);
        connect(m_openFileWidget, SIGNAL(uploadFileClicked()), SLOT(uploadFilesToServer()));
        connect(m_openFileWidget, SIGNAL(uploadDirClicked()), SLOT(uploadFileDirToServer()));
        m_openFileWidget->adjustWidgetRect(width(), height());
    }
    m_openFileWidget->raise();
    m_openFileWidget->show();
}

void MusicCloudManagerTableWidget::startToUploadFile()
{
    m_uploading = true;
    Q_EMIT updateLabelMessage(tr("Files is uploading..."));

    if(m_cancel)
    {
        m_cancel = false;
        uploadDone();
        return;
    }

    m_currentDataItem = FindWaitedItemRow();
    if(!m_currentDataItem.isValid())
    {
        uploadDone();
        return;
    }

    m_syncUploadData->uploadDataOperator(m_currentDataItem.m_id, MUSIC_BUCKET,
                                       m_currentDataItem.m_dataItem.m_name,
                                       m_currentDataItem.m_path);
}

void MusicCloudManagerTableWidget::createItem(const MusicCloudDataItem &data)
{
    int row = rowCount();
    setRowCount(row + 1);

    QHeaderView *headerview = horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_DATA_ROLE, QVariant::fromValue<MusicCloudDataItem>(data));
    setItem(row, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(data.m_dataItem.m_name);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(MusicUIObject::MQSSColor01));
#else
    item->setTextColor(QColor(MusicUIObject::MQSSColor01));
#endif
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROGRESS_ROLE, data.m_dataItem.m_hash.isEmpty() ? 0 : 100);
    setItem(row, 2, item);

                      item = new QTableWidgetItem;
    item->setToolTip(MusicUtils::Number::sizeByte2Label(data.m_dataItem.m_size));
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(3) - 5));
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(MusicUIObject::MQSSColor01));
#else
    item->setTextColor(QColor(MusicUIObject::MQSSColor01));
#endif
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, 3, item);

                      item = new QTableWidgetItem;
    item->setToolTip(data.m_dataItem.m_putTime);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(4) - 5));
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(MusicUIObject::MQSSColor01));
#else
    item->setTextColor(QColor(MusicUIObject::MQSSColor01));
#endif
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, 4, item);
}

int MusicCloudManagerTableWidget::FindUploadItemRow(const QString &time) const
{
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(MUSIC_DATA_ROLE).value<MusicCloudDataItem>();
        if(data.m_id == time)
        {
            return i;
        }
    }
    return -1;
}

MusicCloudDataItem MusicCloudManagerTableWidget::FindWaitedItemRow() const
{
    for(int i = 0; i < rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(MUSIC_DATA_ROLE).value<MusicCloudDataItem>();
        if(data.m_state == MusicCloudDataItem::Waited)
        {
            return data;
        }
    }
    return MusicCloudDataItem();
}



MusicCloudManagerWidget::MusicCloudManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle17 + MusicUIObject::MQSSColorStyle09);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);
    //
    QWidget *topWidget = new QWidget(this);
    QHBoxLayout *topWidgetLayout = new QHBoxLayout(topWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *pLabel = new QLabel(tr("Personal Cloud"), topWidget);
    QFont pLabelFont = pLabel->font();
    pLabelFont.setPixelSize(20);
    pLabel->setFont(pLabelFont);
    pLabel->setStyleSheet(MusicUIObject::MQSSColorStyle13);

    QLabel *iLabel = new QLabel(tr("Sharing of cloud disk songs"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);
    QLabel *sLabel = new QLabel(tr("Size"), topWidget);

    m_sizeValueBar = new QProgressBar(topWidget);
    m_sizeValueBar->setRange(0, 100);
    m_sizeValueBar->setValue(0);
    m_sizeValueBar->setStyleSheet(MusicUIObject::MQSSProgressBar01);

    m_sizeValueLabel = new QLabel("0.0M/40.0G", topWidget);

    topWidgetLayout->addWidget(pLabel);
    topWidgetLayout->addWidget(iLabel);
    topWidgetLayout->addStretch(1);
    topWidgetLayout->addWidget(sLabel);
    topWidgetLayout->addWidget(m_sizeValueBar);
    topWidgetLayout->addWidget(m_sizeValueLabel);
    topWidget->setLayout(topWidgetLayout);
    mainLayout->addWidget(topWidget);
    //
    QWidget *functionWidget = new QWidget(this);
    QHBoxLayout *functionWidgetLayout = new QHBoxLayout(functionWidget);
    functionWidgetLayout->setContentsMargins(10, 10, 10, 10);

    QPushButton *uploadButton = new QPushButton(tr("Upload"), functionWidget);
    uploadButton->setFixedSize(70, 30);
    uploadButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    uploadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *downloadButton = new QPushButton(tr("Download"), functionWidget);
    downloadButton->setFixedSize(70, 30);
    downloadButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *deleteButton = new QPushButton(tr("Delete"), functionWidget);
    deleteButton->setFixedSize(70, 30);
    deleteButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    deleteButton->setCursor(QCursor(Qt::PointingHandCursor));

    QLabel *statusLabel = new QLabel(this);

    functionWidgetLayout->addWidget(uploadButton);
    functionWidgetLayout->addWidget(downloadButton);
    functionWidgetLayout->addWidget(deleteButton);
    functionWidgetLayout->addStretch(1);
    functionWidgetLayout->addWidget(statusLabel);
    functionWidget->setLayout(functionWidgetLayout);
    mainLayout->addWidget(functionWidget);
#ifdef Q_OS_UNIX
    uploadButton->setFocusPolicy(Qt::NoFocus);
    downloadButton->setFocusPolicy(Qt::NoFocus);
    deleteButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(uploadButton, SIGNAL(clicked(bool)), SLOT(uploadFilesToServer()));
    connect(downloadButton, SIGNAL(clicked(bool)), SLOT(downloadFileToServer()));
    connect(deleteButton, SIGNAL(clicked(bool)), SLOT(deleteFileToServer()));
    //
    QWidget *labelWidget = new QWidget(this);
    labelWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle03);
    QHBoxLayout *labelWidgetLayout = new QHBoxLayout(labelWidget);
    functionWidgetLayout->setContentsMargins(10, 20, 10, 10);

    QLabel *label1 = new QLabel(tr("Song"), this);
    label1->setAlignment(Qt::AlignCenter);
    label1->setStyleSheet(MusicUIObject::MQSSFontStyle01);
    labelWidgetLayout->addWidget(label1, 100);
    m_resizeWidgets << label1;

    QLabel *label2 = new QLabel(tr("FileSize"), this);
    label2->setAlignment(Qt::AlignCenter);
    label2->setStyleSheet(MusicUIObject::MQSSFontStyle01);
    labelWidgetLayout->addWidget(label2, 1);

    QLabel *label3 = new QLabel(tr("UploadTime"), this);
    label3->setAlignment(Qt::AlignCenter);
    label3->setStyleSheet(MusicUIObject::MQSSFontStyle01);
    labelWidgetLayout->addWidget(label3, 1);
    labelWidgetLayout->addStretch(3);

    labelWidget->setLayout(labelWidgetLayout);
    mainLayout->addWidget(labelWidget);
    //
    m_managerTableWidget = new MusicCloudManagerTableWidget(this);
    m_managerTableWidget->queryCloudKey();

    connect(m_managerTableWidget, SIGNAL(updateLabelMessage(QString)), statusLabel, SLOT(setText(QString)));
    connect(m_managerTableWidget, SIGNAL(updataSizeLabel(qint64)), SLOT(updataSizeLabel(qint64)));

    mainLayout->addWidget(m_managerTableWidget);
}

MusicCloudManagerWidget::~MusicCloudManagerWidget()
{
    delete m_sizeValueLabel;
    delete m_sizeValueBar;
    delete m_managerTableWidget;
}

void MusicCloudManagerWidget::resizeWindow()
{
    m_managerTableWidget->resizeWindow();
    if(!m_resizeWidgets.isEmpty())
    {
        const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        m_resizeWidgets[0]->setFixedWidth((width - WINDOW_WIDTH_MIN) + 540);
    }
}

void MusicCloudManagerWidget::updataSizeLabel(qint64 size)
{
    m_sizeValueLabel->setText(QString("%1/40.0G").arg(MusicUtils::Number::sizeByte2Label(size)));
    m_sizeValueBar->setValue(size * 100 / (10 * MH_GB2B));
}

void MusicCloudManagerWidget::downloadFileToServer()
{
    m_managerTableWidget->downloadFileToServer();
}

void MusicCloudManagerWidget::deleteFileToServer()
{
    m_managerTableWidget->deleteFileToServer();
}

void MusicCloudManagerWidget::uploadFilesToServer()
{
    m_managerTableWidget->uploadFilesToServer();
}

void MusicCloudManagerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeWindow();
}
