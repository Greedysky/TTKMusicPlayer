#include "musiclrcdownloadbatchwidget.h"
#include "musicabstractqueryrequest.h"
#include "ui_musiclrcdownloadbatchwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicdownloaddatarequest.h"

MusicLrcDownloadBatchTableWidget::MusicLrcDownloadBatchTableWidget(QWidget *parent)
    : MusicAbstractTableWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 50);
    headerview->resizeSection(1, 150);
    headerview->resizeSection(2, 100);
    headerview->resizeSection(3, 190);
    headerview->resizeSection(4, 60);

    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);
}

void MusicLrcDownloadBatchTableWidget::addCellItems(const MusicSongList &items)
{
    setRowCount(items.count());
    QHeaderView *headerview = horizontalHeader();

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSong &v = items[i];

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(v.path());
        item->setText(QString::number(i + 1));
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.path());
        item->setText(TTK::Widget::elidedText(font(), v.artistBack(), Qt::ElideRight, headerview->sectionSize(1) - 10));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 1, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.path());
        item->setText(TTK::Widget::elidedText(font(), v.artistFront(), Qt::ElideRight, headerview->sectionSize(2) - 10));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 2, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.path());
        item->setText(TTK::Widget::elidedText(font(), v.name(), Qt::ElideRight, headerview->sectionSize(3) - 10));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 3, item);

                          item = new QTableWidgetItem;
        item->setToolTip(v.path());
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 4, item);
    }
}



MusicLrcDownloadBatchWidget::MusicLrcDownloadBatchWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicLrcDownloadBatchWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->skipAlreadyLrcCheckBox->setStyleSheet(TTK::UI::CheckBoxStyle01);
    m_ui->saveToLrcDirRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
    m_ui->saveToCurrentDirRadioBox->setStyleSheet(TTK::UI::RadioButtonStyle01);
#ifdef Q_OS_UNIX
    m_ui->skipAlreadyLrcCheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->saveToLrcDirRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->saveToCurrentDirRadioBox->setFocusPolicy(Qt::NoFocus);
    m_ui->addButton->setFocusPolicy(Qt::NoFocus);
    m_ui->downloadButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_ui->addButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->downloadButton->setStyleSheet(TTK::UI::PushButtonStyle04);

    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));

    m_ui->skipAlreadyLrcCheckBox->setChecked(true);
    m_ui->saveToLrcDirRadioBox->setChecked(true);
}

MusicLrcDownloadBatchWidget::~MusicLrcDownloadBatchWidget()
{
    TTKRemoveSingleWidget(className());
    delete m_ui;
}

void MusicLrcDownloadBatchWidget::addButtonClicked()
{
    m_localSongs = m_ui->selectedAreaWidget->selectedSongItems();
    m_ui->tableWidget->addCellItems(m_localSongs);
}

void MusicLrcDownloadBatchWidget::downloadButtonClicked()
{
    m_ui->addButton->setEnabled(false);
    m_ui->downloadButton->setEnabled(false);

    for(int i = 0; i < m_ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *it = m_ui->tableWidget->item(i, 4);
        if(it)
        {
            it->setText({});
        }
    }

    const bool skip = m_ui->skipAlreadyLrcCheckBox->isChecked();
    const bool lrcDir = m_ui->saveToLrcDirRadioBox->isChecked();

    for(int i = 0; i < m_localSongs.count(); ++i)
    {
        QTableWidgetItem *it = m_ui->tableWidget->item(i, 4);
        if(!it)
        {
            continue;
        }

        it->setText("...");

        MusicSong *song = &m_localSongs[i];
        const QString &prefix = lrcDir ? TTK::String::lrcDirPrefix() : QFileInfo(song->path()).path() + TTK_SEPARATOR;
        const QString &path = QString("%1/%2%3").arg(prefix, song->name(), LRC_FILE);
        if(skip && QFile::exists(path))
        {
            it->setForeground(QColor(TTK::UI::Color02));
            it->setText(tr("Skip"));
            continue;
        }

        TTKSemaphoreLoop loop;
        MusicAbstractQueryRequest *d = G_DOWNLOAD_QUERY_PTR->makeQueryRequest(this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
        d->startToSearch(MusicAbstractQueryRequest::QueryType::Music, song->name().trimmed());
        loop.exec();

        if(!d->isEmpty())
        {
            const TTK::MusicSongInformation info(d->songInfoList().front());
            MusicAbstractDownLoadRequest *d = G_DOWNLOAD_QUERY_PTR->makeLrcRequest(info.m_lrcUrl, path, this);
            connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
            d->startRequest();
            loop.exec();

            it->setForeground(QColor(0, 0xFF, 0));
            it->setText(tr("Finish"));
        }
        else
        {
            it->setForeground(QColor(0xFF, 0, 0));
            it->setText(tr("Error"));
        }
    }

    m_ui->addButton->setEnabled(true);
    m_ui->downloadButton->setEnabled(true);
}
