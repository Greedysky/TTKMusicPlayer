#include "musiclrcdownloadbatchwidget.h"
#include "musicabstractqueryrequest.h"
#include "ui_musiclrcdownloadbatchwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloaddatarequest.h"
#include "musicsemaphoreloop.h"
#include "musiccoreutils.h"
#include "musicuiobject.h"

#include <QScrollBar>

MusicLrcDownloadBatchTableWidget::MusicLrcDownloadBatchTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);

    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 50);
    headerview->resizeSection(1, 150);
    headerview->resizeSection(2, 100);
    headerview->resizeSection(3, 190);
    headerview->resizeSection(4, 60);
}

void MusicLrcDownloadBatchTableWidget::createAllItems(const MusicSongs &items)
{
    setRowCount(items.count());
    QHeaderView *headerview = horizontalHeader();
    for(int i=0; i<items.count(); ++i)
    {
        const MusicSong &song = items[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(song.getMusicPath());
        item->setText(QString::number(i + 1));
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.getMusicPath());
        item->setText(MusicUtils::Widget::elidedText(font(), song.getMusicArtistBack(), Qt::ElideRight, headerview->sectionSize(1) - 10));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.getMusicPath());
        item->setText(MusicUtils::Widget::elidedText(font(), song.getMusicArtistFront(), Qt::ElideRight, headerview->sectionSize(2) - 10));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.getMusicPath());
        item->setText(MusicUtils::Widget::elidedText(font(), song.getMusicName(), Qt::ElideRight, headerview->sectionSize(3) - 10));
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 3, item);

                          item = new QTableWidgetItem;
        item->setToolTip(song.getMusicPath());
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);
    }
}

void MusicLrcDownloadBatchTableWidget::itemCellClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
}



MusicLrcDownloadBatchWidget::MusicLrcDownloadBatchWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicLrcDownloadBatchWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->skipAlreadyLrcCheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
    m_ui->saveToLrcDirRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    m_ui->saveToCurrentDirRadioBox->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
#ifdef Q_OS_UNIX
    m_ui->skipAlreadyLrcCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->saveToLrcDirRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->saveToCurrentDirRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->addButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_ui->addButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);
    m_ui->downloadButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle04);

    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));

    m_ui->skipAlreadyLrcCheckBox->setChecked(true);
    m_ui->saveToLrcDirRadioBox->setChecked(true);
}

MusicLrcDownloadBatchWidget::~MusicLrcDownloadBatchWidget()
{
    delete m_ui;
}

void MusicLrcDownloadBatchWidget::addButtonClicked()
{
    m_localSongs = m_ui->selectedAreaWidget->getSelectedSongItems();
    m_ui->tableWidget->createAllItems(m_localSongs);
}

void MusicLrcDownloadBatchWidget::downloadButtonClicked()
{
    m_ui->addButton->setEnabled(false);
    m_ui->downloadButton->setEnabled(false);

    for(int i=0; i<m_ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *it = m_ui->tableWidget->item(i, 4);
        if(it)
        {
            it->setText(QString());
        }
    }

    const bool skip = m_ui->skipAlreadyLrcCheckBox->isChecked();
    const bool lrcDir = m_ui->saveToLrcDirRadioBox->isChecked();

    for(int i=0; i<m_localSongs.count(); ++i)
    {
        QTableWidgetItem *it = m_ui->tableWidget->item(i, 4);
        if(!it)
        {
            continue;
        }

        it->setText("...");

        MusicSong *song = &m_localSongs[i];
        const QString &prefix = lrcDir ? MusicUtils::String::lrcPrefix() : QFileInfo(song->getMusicPath()).path() + QDir::separator();
        const QString &path = QString("%1/%2%3").arg(prefix).arg(song->getMusicName()).arg(LRC_FILE);
        if(skip && QFile::exists(path))
        {
#if TTK_QT_VERSION_CHECK(5,13,0)
            it->setForeground(QColor(100, 100, 100));
#else
            it->setTextColor(QColor(100, 100, 100));
#endif
            it->setText(tr("Skip"));
            continue;
        }

        MusicSemaphoreLoop loop;
        MusicAbstractQueryRequest *d = M_DOWNLOAD_QUERY_PTR->getQueryRequest(this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        d->startToSearch(MusicAbstractQueryRequest::MusicQuery, song->getMusicName().trimmed());
        loop.exec();

        if(!d->isEmpty())
        {
            const MusicObject::MusicSongInformation info(d->getMusicSongInfos().first());
            MusicAbstractDownLoadRequest *d = M_DOWNLOAD_QUERY_PTR->getDownloadLrcRequest(info.m_lrcUrl, path, MusicObject::DownloadLrc, this);
            d->startToDownload();
            loop.exec();
#if TTK_QT_VERSION_CHECK(5,13,0)
            it->setForeground(QColor(0, 0xFF, 0));
#else
            it->setTextColor(QColor(0, 0xFF, 0));
#endif
            it->setText(tr("Finish"));
        }
        else
        {
#if TTK_QT_VERSION_CHECK(5,13,0)
            it->setForeground(QColor(0xFF, 0, 0));
#else
            it->setTextColor(QColor(0xFF, 0, 0));
#endif
            it->setText(tr("Error"));
        }
    }

    m_ui->addButton->setEnabled(true);
    m_ui->downloadButton->setEnabled(true);
}

void MusicLrcDownloadBatchWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
