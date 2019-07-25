#include "musiccloudmanagerwidget.h"
#include "musicuiobject.h"
#include "musicsemaphoreloop.h"
#include "musicdownloadsourcethread.h"
#include "musicitemdelegate.h"
#include "musicnumberutils.h"
#include "musicopenfilewidget.h"
#include "musicsettingmanager.h"
#include "musiccloudfileinformationwidget.h"
#include "musicdatadownloadthread.h"
#include "musiccoreutils.h"
#include "musicfileutils.h"
#include "musicstringutils.h"
#include "musicformats.h"
#include "musictime.h"
#include "musicmessagebox.h"
#include "musicconnectionpool.h"
#include "musiccloudtablewidget.h"
#include "musicsourceupdatethread.h"

#include "qiniu/qnconf.h"
#include "qiniu/qnsimplelistdata.h"
#include "qiniu/qnsimpleuploaddata.h"
#include "qiniu/qnsimpledeletedata.h"

#///QJson import
#include "qjson/parser.h"

#define QN_BUCKET       "music"
#define QN_CLOUD        "cloud"

Q_DECLARE_METATYPE(MusicCloudDataItem)

MusicCloudManagerTableWidget::MusicCloudManagerTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicCloudUploadTableWidget::getClassName());

    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 370);
    headerview->resizeSection(2, 110);
    headerview->resizeSection(3, 50);
    headerview->resizeSection(4, 110);

    m_uploading = false;
    m_openFileWidget = nullptr;
    m_totalFileSzie = 0;

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    MusicUtils::Widget::setTransparent(this, 0);
    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    m_progressBarDelegate = new MusicProgressBarDelegate(this);
    setItemDelegateForColumn(2, m_progressBarDelegate);

    m_networkManager = new QNetworkAccessManager(this);
    m_qnListData = new QNSimpleListData(m_networkManager, this);
    m_qnDeleteData = new QNSimpleDeleteData(m_networkManager, this);
    m_qnUploadData = new QNSimpleUploadData(m_networkManager, this);
    connect(m_qnListData, SIGNAL(receiveFinshed(QNDataItems)), SLOT(receiveDataFinshed(QNDataItems)));
    connect(m_qnDeleteData, SIGNAL(deleteFileFinished(bool)), SLOT(deleteFileFinished(bool)));
    connect(m_qnUploadData, SIGNAL(uploadFileFinished(QString)), SLOT(uploadFileFinished(QString)));
}

MusicCloudManagerTableWidget::~MusicCloudManagerTableWidget()
{
    M_CONNECTION_PTR->removeValue(getClassName());
    delete m_qnListData;
    delete m_qnDeleteData;
    delete m_qnUploadData;
    delete m_networkManager;
    delete m_openFileWidget;
    delete m_progressBarDelegate;
}

bool MusicCloudManagerTableWidget::getKey()
{
    MusicSemaphoreLoop loop;
    connect(this, SIGNAL(getKeyFinished()), &loop, SLOT(quit()));

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(keyDownLoadFinished(QByteArray)));
    const QString &buketUrl = M_SETTING_PTR->value(MusicSettingManager::QiNiuDataConfigChoiced).toString();
    download->startToDownload(MusicUtils::Algorithm::mdII(buketUrl, false) + QN_CLOUD);

    loop.exec();
    updateListToServer();

    return !QNConf::ACCESS_KEY.isEmpty() && !QNConf::SECRET_KEY.isEmpty();
}

void MusicCloudManagerTableWidget::resizeWindow()
{
    const int w = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(1, (w - WINDOW_WIDTH_MIN) + 370);

    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 1);
        it->setText(MusicUtils::Widget::elidedText(font(), it->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 31));
    }

    if(m_openFileWidget)
    {
        m_openFileWidget->adjustRect(width(), height());
    }
}

void MusicCloudManagerTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudManagerTableWidget::keyDownLoadFinished(const QByteArray &data)
{
    QJson::Parser parser;
    bool ok;
    QVariant dt = parser.parse(data, &ok);
    if(ok)
    {
        QVariantMap value = dt.toMap();
        QNConf::ACCESS_KEY = value["key"].toString();
        QNConf::SECRET_KEY = value["secret"].toByteArray();
    }
    emit getKeyFinished();
}

void MusicCloudManagerTableWidget::receiveDataFinshed(const QNDataItems &items)
{
    clear();
    m_totalFileSzie = 0;

    const int count = items.count();
    if(count == 0)
    {
        emit updateLabelMessage(tr("List Is Empty!"));
        createUploadFileWidget();
        return;
    }

    foreach(const QNDataItem &item, items)
    {
        MusicCloudDataItem data;
        data.m_id = QString::number(MusicTime::timeStamp());
        data.m_path = item.m_name.trimmed();
        data.m_state = MusicCloudDataItem::Successed;
        data.m_dataItem = item;
        m_totalFileSzie += item.m_size;

        createItem(data);
    }

    emit updateLabelMessage(tr("List Update Finished!"));
    emit updataSizeLabel(m_totalFileSzie);
}

void MusicCloudManagerTableWidget::uploadFileFinished(const QString &time)
{
    if(time == TTK_ERROR_STR)
    {
        emit uploadFileError(m_currentDataItem);
    }

    const int row = FindUploadItemRow(time);
    if(row != -1)
    {
        QTableWidgetItem *it = item(row, 0);
        if(it)
        {
            MusicCloudDataItem data = it->data(MUSIC_DATAS_ROLE).value<MusicCloudDataItem>();
            data.m_state = MusicCloudDataItem::Successed;
            it->setData(MUSIC_DATAS_ROLE, QVariant::fromValue<MusicCloudDataItem>(data));
            m_totalFileSzie += data.m_dataItem.m_size;
        }
        emit updataSizeLabel(m_totalFileSzie);
    }
    else
    {
        emit uploadFileError(m_currentDataItem);
    }

    startToUploadFile();
}

void MusicCloudManagerTableWidget::deleteFileFinished(bool state)
{
    emit updateLabelMessage(state ? tr("Delete The Current File Success!") : tr("Delete The Current File Error!"));
}

void MusicCloudManagerTableWidget::updateListToServer()
{
    emit updateLabelMessage(tr("List Updating"));
    m_qnListData->listDataToServer(QN_BUCKET);
}

void MusicCloudManagerTableWidget::deleteFileToServer()
{
    if(currentRow() < 0 || m_uploading)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    const MusicCloudDataItem &data = it->data(MUSIC_DATAS_ROLE).value<MusicCloudDataItem>();
    removeRow(currentRow());
    m_qnDeleteData->deleteDataToServer(QN_BUCKET, data.m_dataItem.m_name);
    m_totalFileSzie -= data.m_dataItem.m_size;
    emit updataSizeLabel(m_totalFileSzie);

    createUploadFileWidget();
}

void MusicCloudManagerTableWidget::deleteFilesToServer()
{
    if(m_uploading)
    {
        return;
    }

    selectAll();
    const MIntList deleteList(getMultiIndexSet());

    for(int i=deleteList.count() - 1; i>=0; --i)
    {
        int index = deleteList[i];
        QTableWidgetItem *it = item(index, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(MUSIC_DATAS_ROLE).value<MusicCloudDataItem>();
        removeRow(index); //Delete the current row
        m_qnDeleteData->deleteDataToServer(QN_BUCKET, data.m_dataItem.m_name);

        m_totalFileSzie -= data.m_dataItem.m_size;
        emit updataSizeLabel(m_totalFileSzie);
    }
}

void MusicCloudManagerTableWidget::downloadFileToServer()
{
    if(currentRow() < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    QTableWidgetItem *it = item(currentRow(), 0);
    if(it == nullptr)
    {
        return;
    }

    const MusicCloudDataItem &data = it->data(MUSIC_DATAS_ROLE).value<MusicCloudDataItem>();
    const QString &buketUrl = M_SETTING_PTR->value(MusicSettingManager::QiNiuMusicConfigChoiced).toString();
    const QString &url = m_qnUploadData->getDownloadUrl(MusicUtils::Algorithm::mdII(buketUrl, false), data.m_dataItem.m_name);

    MusicDataDownloadThread *download = new MusicDataDownloadThread(url, MusicUtils::String::musicPrefix() + data.m_dataItem.m_name, MusicObject::DownloadMusic, this);
    download->setRecordType(MusicObject::RecordCloudDownload);
    download->startToDownload();
}

void MusicCloudManagerTableWidget::uploadFilesToServer()
{
    const QString filter(MusicFormats::supportFormatsFilterDialogString().join(";;"));
    const QStringList &paths = MusicUtils::File::getOpenFilesDialog(this, filter);
    uploadFilesToServer(paths);
}

void MusicCloudManagerTableWidget::uploadFileDirToServer()
{
    const QString &path = QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        delete m_openFileWidget;
        m_openFileWidget = nullptr;

        foreach(const QFileInfo &info, MusicUtils::File::getFileListByDir(path, MusicFormats::supportFormatsFilterString(), true))
        {
            MusicCloudDataItem item;
            item.m_id = QString::number(MusicTime::timeStamp());
            item.m_path = path;
            item.m_state = MusicCloudDataItem::Waited;
            item.m_dataItem.m_name = info.fileName().trimmed();
            item.m_dataItem.m_putTime = item.m_id.toULongLong();
            item.m_dataItem.m_size = info.size();

            createItem(item);
        }

        if(!m_uploading)
        {
            startToUploadFile();
        }
    }

    createUploadFileWidget();
}

void MusicCloudManagerTableWidget::reuploadFilesToServer(const QStringList &items)
{
    uploadFilesToServer(items);
}

void MusicCloudManagerTableWidget::uploadProgress(const QString &time, qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal != 0)
    {
        const int value = MStatic_cast(int, (bytesSent*1.0/bytesTotal)*100);
        const int row = FindUploadItemRow(time);
        if(row != -1)
        {
            QTableWidgetItem *it = item(row, 2);
            if(it)
            {
                it->setData(MUSIC_PROCS_ROLE, value);
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
    MusicCloudDataItem data(it->data(MUSIC_DATAS_ROLE).value<MusicCloudDataItem>());
    w.setFileInformation(&data.m_dataItem);
    w.exec();
}

void MusicCloudManagerTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
//    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu menu(this);

    QMenu uploadMenu(tr("upload"), &menu);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    uploadMenu.setStyleSheet(MusicUIObject::MMenuStyle02);
    uploadMenu.addAction(tr("uploadFile"), this, SLOT(uploadFilesToServer()));
    uploadMenu.addAction(tr("uploadFiles"), this, SLOT(uploadFileDirToServer()));

    menu.addMenu(&uploadMenu);
    menu.addAction(tr("deleteFile"), this, SLOT(deleteFileToServer()))->setEnabled(!m_uploading);
    menu.addAction(tr("deleteFiles"), this, SLOT(deleteFilesToServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("download"), this, SLOT(downloadFileToServer()))->setEnabled(!m_uploading);
    menu.addAction(tr("updateFiles"), this, SLOT(updateListToServer()))->setEnabled(!m_uploading);
    menu.addSeparator();
    menu.addAction(tr("musicInfo..."), this, SLOT(showFileInformationWidget()));

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

    foreach(const QString &path, paths)
    {
        MusicCloudDataItem item;
        const QFileInfo info(path);
        item.m_id = QString::number(MusicTime::timeStamp());
        item.m_path = path;
        item.m_state = MusicCloudDataItem::Waited;
        item.m_dataItem.m_name = info.fileName().trimmed();
        item.m_dataItem.m_putTime = item.m_id.toULongLong();
        item.m_dataItem.m_size = info.size();

        MusicUtils::Core::sleep(MT_MS);

        createItem(item);
    }

    if(!m_uploading)
    {
        startToUploadFile();
    }
}

void MusicCloudManagerTableWidget::createUploadFileWidget()
{
    if(rowCount() != 0)
    {
        return;
    }

    if(m_openFileWidget == nullptr)
    {
        m_openFileWidget = new MusicOpenFileWidget(this);
        connect(m_openFileWidget, SIGNAL(uploadFileClicked()), SLOT(uploadFilesToServer()));
        connect(m_openFileWidget, SIGNAL(uploadFilesClicked()), SLOT(uploadFileDirToServer()));
        m_openFileWidget->adjustRect(width(), height());
    }
    m_openFileWidget->raise();
    m_openFileWidget->show();
}

void MusicCloudManagerTableWidget::startToUploadFile()
{
    m_uploading = true;
    emit updateLabelMessage(tr("Files Is Uploading!"));

    m_currentDataItem = FindWaitedItemRow();
    if(!m_currentDataItem.isValid())
    {
        uploadDone();
        return;
    }

    QFile file(m_currentDataItem.m_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        startToUploadFile();
    }

    m_qnUploadData->uploadDataToServer(m_currentDataItem.m_id, file.readAll(), QN_BUCKET,
                                       m_currentDataItem.m_dataItem.m_name,
                                       m_currentDataItem.m_dataItem.m_name);
    file.close();
}

void MusicCloudManagerTableWidget::createItem(const MusicCloudDataItem &data)
{
    int row = rowCount();
    setRowCount(row + 1);

    QHeaderView *headerview = horizontalHeader();

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_DATAS_ROLE, QVariant::fromValue<MusicCloudDataItem>(data));
    setItem(row, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(data.m_dataItem.m_name);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 20));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row, 1, item);

                      item = new QTableWidgetItem;
    item->setData(MUSIC_PROCS_ROLE, data.m_dataItem.m_hash.isEmpty() ? 0 : 100);
    setItem(row, 2, item);

                      item = new QTableWidgetItem;
    item->setToolTip(MusicUtils::Number::size2Label(data.m_dataItem.m_size));
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(3) - 5));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, 3, item);

                      item = new QTableWidgetItem;
    item->setToolTip(QDateTime::fromMSecsSinceEpoch(data.m_dataItem.m_putTime).toString("yyyy-MM-dd"));
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(4) - 5));
    item->setTextColor(QColor(MusicUIObject::MColorStyle12_S));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    setItem(row, 4, item);
}

int MusicCloudManagerTableWidget::FindUploadItemRow(const QString &time) const
{
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(MUSIC_DATAS_ROLE).value<MusicCloudDataItem>();
        if(data.m_id == time)
        {
            return i;
        }
    }
    return -1;
}

MusicCloudDataItem MusicCloudManagerTableWidget::FindWaitedItemRow() const
{
    for(int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem *it = item(i, 0);
        if(it == nullptr)
        {
            continue;
        }

        const MusicCloudDataItem &data = it->data(MUSIC_DATAS_ROLE).value<MusicCloudDataItem>();
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
    setStyleSheet(MusicUIObject::MBackgroundStyle17 + MusicUIObject::MColorStyle09);

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
    pLabel->setStyleSheet(MusicUIObject::MColorStyle13);

    QLabel *iLabel = new QLabel(tr("Sharing Of Cloud Disk Songs"), topWidget);
    QFont iLabelFont = iLabel->font();
    iLabelFont.setPixelSize(15);
    iLabel->setFont(iLabelFont);
    QLabel *sLabel = new QLabel(tr("Size"), topWidget);

    m_sizeValueBar = new QProgressBar(topWidget);
    m_sizeValueBar->setRange(0, 100);
    m_sizeValueBar->setValue(0);
    m_sizeValueBar->setStyleSheet(MusicUIObject::MProgressBar01);

    m_sizeValueLabel = new QLabel("0.0M/10.0G", topWidget);

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
    uploadButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    uploadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *downloadButton = new QPushButton(tr("Download"), functionWidget);
    downloadButton->setFixedSize(70, 30);
    downloadButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

    QPushButton *deleteButton = new QPushButton(tr("Delete"), functionWidget);
    deleteButton->setFixedSize(70, 30);
    deleteButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
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
    labelWidget->setStyleSheet(MusicUIObject::MBackgroundStyle03);
    QHBoxLayout *labelWidgetLayout = new QHBoxLayout(labelWidget);
    functionWidgetLayout->setContentsMargins(10, 20, 10, 10);
    QLabel *label1 = new QLabel(tr("Song"), this);
    label1->setAlignment(Qt::AlignCenter);
    label1->setStyleSheet(MusicUIObject::MFontStyle01);
    labelWidgetLayout->addWidget(label1, 100);
    m_resizeLabels << label1;

    QLabel *label2 = new QLabel(tr("FileSize"), this);
    label2->setAlignment(Qt::AlignCenter);
    label2->setStyleSheet(MusicUIObject::MFontStyle01);
    labelWidgetLayout->addWidget(label2, 1);

    QLabel *label3 = new QLabel(tr("UploadTime"), this);
    label3->setAlignment(Qt::AlignCenter);
    label3->setStyleSheet(MusicUIObject::MFontStyle01);
    labelWidgetLayout->addWidget(label3, 1);

    labelWidget->setLayout(labelWidgetLayout);
    mainLayout->addWidget(labelWidget);
    //
    m_managerTableWidget = new MusicCloudManagerTableWidget(this);
    m_managerTableWidget->getKey();

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
    const int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
    if(!m_resizeLabels.isEmpty())
    {
        m_resizeLabels[0]->setFixedWidth((width - WINDOW_WIDTH_MIN) + 540);
    }
}

void MusicCloudManagerWidget::updataSizeLabel(qint64 size)
{
    m_sizeValueLabel->setText(QString("%1/10.0G").arg(MusicUtils::Number::size2Label(size)));
    m_sizeValueBar->setValue(size*100/(10*MH_GB2B));
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
