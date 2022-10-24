#include "musicmobilesongsmanagerwidget.h"
#include "ui_musicmobilesongsmanagerwidget.h"
#include "musicsongsmanagerthread.h"
#include "musicsongssummariziedwidget.h"
#include "musictoastlabel.h"
#include "musicconnectionpool.h"

MusicMobileSongsTableWidget::MusicMobileSongsTableWidget(QWidget *parent)
    : MusicAbstractSongsListTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(5);

    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 460);
    headerview->resizeSection(1, 85);
    headerview->resizeSection(2, 105);
    headerview->resizeSection(3, 26);
    headerview->resizeSection(4, 26);

    m_songs = new MusicSongList;
    MusicUtils::Widget::setTransparent(this, 150);
}

MusicMobileSongsTableWidget::~MusicMobileSongsTableWidget()
{
    removeItems();
    delete m_songs;
}

void MusicMobileSongsTableWidget::addCellItems(const QStringList &songs)
{
    setRowCount(songs.count());
    QHeaderView *headerview = horizontalHeader();

    for(int i = 0; i < songs.count(); ++i)
    {
        const QFileInfo fin(songs[i]);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setToolTip(fin.fileName());
        item->setText(" " + MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(0) - 20));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(MusicUtils::Number::sizeByte2Label(fin.size()));
        item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
        QtItemSetTextAlignment(item, Qt::AlignRight | Qt::AlignVCenter);
        setItem(i, 1, item);

                         item = new QTableWidgetItem(fin.lastModified().date().toString(Qt::ISODate));
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 2, item);

                         item = new QTableWidgetItem;
        item->setIcon(QIcon(":/contextMenu/btn_audition"));
        setItem(i, 3, item);

                         item = new QTableWidgetItem;
        item->setIcon(QIcon(":/contextMenu/btn_add"));
        setItem(i, 4, item);

        m_songs->append(MusicSong(fin.absoluteFilePath()));
    }
}

void MusicMobileSongsTableWidget::removeItems()
{
    MusicAbstractSongsListTableWidget::removeItems();
    m_songs->clear();
}

void MusicMobileSongsTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicAbstractSongsListTableWidget::contextMenuEvent(event);

    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(musicPlayClicked()));
    menu.addAction(tr("Download More..."), this, SLOT(musicSongDownload()));
    menu.addSeparator();

    createMoreMenu(&menu);

    const bool status = !m_songs->isEmpty();
    menu.addAction(tr("Song Info..."), this, SLOT(musicFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(musicOpenFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(musicAlbumQueryWidget()));
    menu.addSeparator();

    menu.exec(QCursor::pos());
}



MusicMobileSongsManagerWidget::MusicMobileSongsManagerWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      MusicItemSearchInterfaceClass(),
      m_ui(new Ui::MusicMobileSongsManagerWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setAttribute(Qt::WA_DeleteOnClose);
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->toolWidget->setStyleSheet(QString("#toolWidget{%1}").arg(MusicUIObject::MQSSBackgroundStyle07));

    m_ui->allSelectedcheckBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("All"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->auditionButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle01 + MusicUIObject::MQSSToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_audition); }");
    m_ui->addButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle01 + MusicUIObject::MQSSToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_add); }");

    m_ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->loadingLabel->setType(MusicGifLabelWidget::Module::CicleBlue);
    m_ui->toolWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle12);

    connect(m_ui->auditionButton, SIGNAL(clicked()), SLOT(auditionButtonClick()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(addButtonClick()));
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(searchResultChanged(int,int)));

    connect(m_ui->songlistTable, SIGNAL(cellClicked(int,int)), SLOT(itemCellOnClick(int,int)));
    connect(m_ui->songlistTable, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));

#ifdef Q_OS_UNIX
    m_ui->allSelectedcheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->searchLineLabel->setFocusPolicy(Qt::NoFocus);
#endif

    m_thread = new MusicSongsManagerThread(this);
    connect(m_thread, SIGNAL(searchFilePathChanged(QStringList)), SLOT(searchFilePathChanged(QStringList)));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->poolConnect(className(), MusicSongsSummariziedWidget::className());
}

MusicMobileSongsManagerWidget::~MusicMobileSongsManagerWidget()
{
    G_CONNECTION_PTR->removeValue(className());
    G_SINGLE_MANAGER_PTR->removeObject(className());
    clearAllItems();
    m_thread->stopAndQuitThread();
    delete m_thread;
    delete m_ui;
}

void MusicMobileSongsManagerWidget::findExtraDevicePath(const QString &dir)
{
    TTK_INFO_STREAM("Start fetch result");
    m_thread->setFindFilePath(dir);
    m_thread->stopAndQuitThread();
    m_thread->start();
    m_ui->loadingLabel->run(true);
}

void MusicMobileSongsManagerWidget::selectedAllItems(bool check)
{
    if(!check)
    {
        m_ui->songlistTable->clearSelection();
        m_ui->songlistTable->setCurrentIndex(QModelIndex());
    }
    else
    {
        m_ui->songlistTable->selectAll();
    }
}

void MusicMobileSongsManagerWidget::auditionButtonClick()
{
    if(m_ui->songlistTable->selectedItems().count() > 0)
    {
        selectedItemsToPlaylist();
        return;
    }

    if(m_ui->songlistTable->rowCount() <= 0 || m_ui->songlistTable->currentRow() < 0)
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    selectedItemsToPlaylist();
}

void MusicMobileSongsManagerWidget::addButtonClick()
{
    if(m_ui->songlistTable->selectedItems().count() > 0)
    {
        selectedItemsToPlaylist();
        return;
    }

    if(m_ui->songlistTable->rowCount() <= 0 || m_ui->songlistTable->currentRow() < 0)
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    selectedItemsToPlaylist();
}

void MusicMobileSongsManagerWidget::itemCellOnClick(int row, int column)
{
    switch(column)
    {
        case 3:
        case 4:
        {
            if(!m_searchResultCache.isEmpty())
            {
                const int count = m_ui->searchLineEdit->text().trimmed().count();
                row = m_searchResultCache.value(count)[row];
                m_ui->searchLineEdit->clear();
                m_searchResultCache.clear();
            }
            Q_EMIT addSongToPlaylist(QStringList(m_containerItems[row]));
            break;
        }
        default: break;
    }
}

void MusicMobileSongsManagerWidget::itemDoubleClicked(int row, int)
{
    if(!m_searchResultCache.isEmpty())
    {
        const int count = m_ui->searchLineEdit->text().trimmed().count();
        row = m_searchResultCache.value(count)[row];
        m_ui->searchLineEdit->clear();
        m_searchResultCache.clear();
    }
    Q_EMIT addSongToPlaylist(QStringList(m_containerItems[row]));
}

void MusicMobileSongsManagerWidget::searchFilePathChanged(const QStringList &path)
{
    TTK_INFO_STREAM("Stop fetch result");

    clearAllItems();
    m_ui->searchLineEdit->clear();
    m_searchResultCache.clear();

    m_containerItems = path;
    m_ui->songlistTable->addCellItems(m_containerItems);
    m_ui->loadingLabel->run(false);
}

void MusicMobileSongsManagerWidget::searchResultChanged(int, int column)
{
    TTKIntList result;
    for(int i = 0; i < m_containerItems.count(); ++i)
    {
        if(QFileInfo(m_containerItems[i]).absolutePath().contains(m_ui->searchLineEdit->text().trimmed(), Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    QStringList data;
    for(const int index : qAsConst(result))
    {
        data.append(m_containerItems[index]);
    }

    clearAllItems();
    m_searchResultCache.insert(column, result);
    m_ui->songlistTable->addCellItems(data);
}

void MusicMobileSongsManagerWidget::clearAllItems()
{
    if(m_ui->allSelectedcheckBox->isChecked())
    {
        m_ui->allSelectedcheckBox->click();
    }

    m_ui->songlistTable->removeItems();
}

void MusicMobileSongsManagerWidget::selectedItemsToPlaylist()
{
    TTKIntSet auditionRow; //if selected multi rows
    for(QTableWidgetItem *item : m_ui->songlistTable->selectedItems())
    {
        if(!m_searchResultCache.isEmpty())
        {
            const int count = m_ui->searchLineEdit->text().trimmed().count();
            auditionRow.insert(m_searchResultCache.value(count)[item->row()]);
        }
        else
        {
            auditionRow.insert(item->row());
        }
    }

    m_ui->searchLineEdit->clear();
    m_searchResultCache.clear();

    TTKIntList indexs = auditionRow.values();
    std::sort(indexs.begin(), indexs.end());

    QStringList names;
    for(const int index : qAsConst(indexs))
    {
        names << m_containerItems[index];
    }

    Q_EMIT addSongToPlaylist(names);
}
