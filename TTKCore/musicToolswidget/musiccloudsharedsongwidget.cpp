#include "musiccloudsharedsongwidget.h"
#include "musicnumberdefine.h"
#include "musicclickedlabel.h"
#include "musicplayer.h"
#include "qnconf.h"

#include <QPainter>
#include <QBoxLayout>
#include <QTimer>
#include <QMenu>
#include <QFileDialog>
#include <QEventLoop>
#include <QPushButton>

#define BUCKET          "music"
#define QN_ACCESS_KEY   "L2GGQ-ttIlTScXVtXOdwPF2ftQAEiVK1qor5KCu3"
#define QN_SECRET_KEY   "FXiQ8EWibo-9tIlWRS3UAJOqv94pM1QViU2Gw25y"

MusicCloudSharedSongTableWidget::MusicCloudSharedSongTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setColumnCount(3);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 10);
    headerview->resizeSection(1, 266);
    headerview->resizeSection(2, 35);

    QNConf::ACCESS_KEY = QN_ACCESS_KEY;
    QNConf::SECRET_KEY = QN_SECRET_KEY;

    m_uploading = false;
    m_uploadFileWidget = nullptr;

    m_timerToUpload = new QTimer(this);
    m_timerToUpload->setInterval(MT_S2MS);
    m_networkManager = new QNetworkAccessManager(this);
    m_qnListData = new QNSimpleListData(m_networkManager, this);
    m_qnUploadData = new QNSimpleUploadData(m_networkManager, this);
    connect(m_timerToUpload, SIGNAL(timeout()), SLOT(startToUploadFile()));
    connect(m_qnListData, SIGNAL(receiveFinshed(QNDataItems)), SLOT(receiveDataFinshed(QNDataItems)));
    connect(m_qnUploadData, SIGNAL(uploadFileFinished(QString)), SLOT(uploadFileFinished(QString)));

    QTimer::singleShot(MT_MS*100, this, SLOT(updateList()));
}

MusicCloudSharedSongTableWidget::~MusicCloudSharedSongTableWidget()
{
    m_timerToUpload->stop();
    delete m_timerToUpload;
    delete m_uploadFileWidget;
    delete m_qnListData;
    delete m_qnUploadData;
    delete m_networkManager;
}

QString MusicCloudSharedSongTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudSharedSongTableWidget::listCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}

void MusicCloudSharedSongTableWidget::updateList()
{
    emit updateLabelMessage(tr("List Updating"));
    m_qnListData->listDataToServer(BUCKET);
}

void MusicCloudSharedSongTableWidget::receiveDataFinshed(const QNDataItems &items)
{
    clear();
    int count = items.count();
    if(count == 0)
    {
        emit updateLabelMessage(tr("List Is Empty!"));
        createUploadFileWidget();
        return;
    }

    delete m_uploadFileWidget;
    m_uploadFileWidget = nullptr;

    setRowCount(count);
    for(int i=0; i<count; ++i)
    {
        QNDataItem dataItem = items[i];
        QTableWidgetItem *item = new QTableWidgetItem;
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), dataItem.m_name, Qt::ElideRight, 260));
        item->setToolTip(dataItem.m_name);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setText(MusicUtils::UWidget::elidedText(font(), MusicUtils::UNumber::size2Number(dataItem.m_size),
                                                              Qt::ElideRight, 30));
        item->setToolTip(MusicUtils::UNumber::size2Label(dataItem.m_size));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);
    }
    emit updateLabelMessage(tr("List Update Finished!"));
}

void MusicCloudSharedSongTableWidget::uploadFileFinished(const QString &name)
{
    for(int i=0; i<m_waitedFiles.count(); ++i)
    {
        UploadData *data = &m_waitedFiles[i];
        if(data->m_path == m_currentUploadFileName)
        {
            data->m_state = 1;
        }
    }
    emit uploadDone();
    qDebug() << "ss" << name;
}

void MusicCloudSharedSongTableWidget::uploadFileToServer()
{
    QStringList path = QFileDialog::getOpenFileNames(this,
                       QString(), "./", "File (*.mp3)");
    if(!path.isEmpty())
    {
        foreach(QString file, path)
        {
            UploadData data;
            data.m_path = file;
            data.m_name = QFileInfo(file).fileName();
            data.m_state = 0;
            m_waitedFiles << data;
        }
    }
    m_timerToUpload->stop();
    m_timerToUpload->start();
}

void MusicCloudSharedSongTableWidget::uploadFilesToServer()
{
    QString path =  QFileDialog::getExistingDirectory(this, QString(), "./");
    if(!path.isEmpty())
    {
        foreach(QFileInfo file, MusicUtils::UCore::findFile(path, MusicPlayer::supportFormatsFilterString()))
        {
            UploadData data;
            data.m_path = file.absoluteFilePath();
            data.m_name = file.fileName();
            data.m_state = 0;
            m_waitedFiles << data;
        }
    }
    m_timerToUpload->stop();
    m_timerToUpload->start();
}

void MusicCloudSharedSongTableWidget::startToUploadFile()
{
    if(m_waitedFiles.isEmpty() || m_uploading)
    {
        return;
    }

    m_uploading = true;
    m_timerToUpload->stop();
    bool needUpload;
    foreach(UploadData data, m_waitedFiles)
    {
        if(data.m_state == 0)
        {
            needUpload = true;
            break;
        }
    }
    if(!needUpload)
    {
        return;
    }

    foreach(UploadData data, m_waitedFiles)
    {
        QFile file(m_currentUploadFileName = data.m_path);
        if(!file.open(QIODevice::ReadOnly) || data.m_state == 1)
        {
            continue;
        }

        QEventLoop loop(this);
        connect(this, SIGNAL(uploadDone()), &loop, SLOT(quit()));
        m_qnUploadData->uploadDataToServer(file.readAll(), BUCKET, data.m_name, data.m_name);
        loop.exec();
        file.close();
    }
    m_uploading = false;

    updateList();
}

void MusicCloudSharedSongTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractTableWidget::contextMenuEvent(event);
    QMenu menu(this);
    QMenu uploadMenu(tr("upload"), &menu);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    uploadMenu.setStyleSheet(MusicUIObject::MMenuStyle02);

    uploadMenu.addAction(tr("uploadFile"), this, SLOT(uploadFileToServer()));
    uploadMenu.addAction(tr("uploadFiles"), this, SLOT(uploadFilesToServer()));

    menu.addMenu(&uploadMenu);

    menu.exec(QCursor::pos());
}

void MusicCloudSharedSongTableWidget::createUploadFileWidget()
{
    if(m_uploadFileWidget == nullptr)
    {
        m_uploadFileWidget = new QWidget(this);

        QVBoxLayout *layout = new QVBoxLayout(m_uploadFileWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        MusicClickedLabel *uploadFile = new MusicClickedLabel(m_uploadFileWidget);
        MusicClickedLabel *uploadDirs = new MusicClickedLabel(m_uploadFileWidget);
        connect(uploadFile, SIGNAL(clicked()), SLOT(uploadFileToServer()));
        connect(uploadDirs, SIGNAL(clicked()), SLOT(uploadFilesToServer()));
        layout->addWidget(uploadFile);
        layout->addWidget(uploadDirs);
        m_uploadFileWidget->setLayout(layout);

        uploadFile->setText(tr("<u>uploadFile</u>"));
        uploadDirs->setText(tr("<u>uploadDirs</u>"));
        m_uploadFileWidget->resize(100, 50);
        m_uploadFileWidget->setStyleSheet("background:red");

        int x = (width() - m_uploadFileWidget->width())/2;
        int y = (height() - m_uploadFileWidget->height())/2;
        m_uploadFileWidget->move(x, y);
    }
    m_uploadFileWidget->raise();
    m_uploadFileWidget->show();
}



MusicCloudSharedSongWidget::MusicCloudSharedSongWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_tableWidget = new MusicCloudSharedSongTableWidget(this);
    connect(m_tableWidget, SIGNAL(updateLabelMessage(QString)), SLOT(updateLabelMessage(QString)));

    QWidget *bottomWidget = new QWidget(this);
    bottomWidget->setFixedHeight(40);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomWidget);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("background:red");
    QPushButton *button = new QPushButton(this);
    button->setCursor(QCursor(Qt::PointingHandCursor));
    button->setFixedWidth(40);

    bottomLayout->addWidget(m_statusLabel);
    bottomLayout->addWidget(button);
    bottomWidget->setLayout(bottomLayout);

    layout->addWidget(m_tableWidget);
    layout->addWidget(bottomWidget);
    setLayout(layout);
}

MusicCloudSharedSongWidget::~MusicCloudSharedSongWidget()
{
    delete m_tableWidget;
    delete m_statusLabel;
}

QString MusicCloudSharedSongWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudSharedSongWidget::updateLabelMessage(const QString &text)
{
    m_statusLabel->setText(text);
}

void MusicCloudSharedSongWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255, 50));
    painter.end();

    QWidget::paintEvent(event);
}

void MusicCloudSharedSongWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
