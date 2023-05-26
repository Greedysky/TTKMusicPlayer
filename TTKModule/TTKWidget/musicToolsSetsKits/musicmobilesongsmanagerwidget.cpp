#include "musicmobilesongsmanagerwidget.h"
#include "ui_musicmobilesongsmanagerwidget.h"
#include "musicsongssummariziedwidget.h"
#include "musicsongsmanagerthread.h"
#include "musicconnectionpool.h"
#include "musictoastlabel.h"

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
    TTK::Widget::setTransparent(this, 150);
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
        item->setText(" " + TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(0) - 20));
        QtItemSetTextAlignment(item, Qt::AlignLeft | Qt::AlignVCenter);
        setItem(i, 0, item);

                         item = new QTableWidgetItem;
        item->setToolTip(TTK::Number::sizeByteToLabel(fin.size()));
        item->setText(TTK::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
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

void MusicMobileSongsTableWidget::itemCellEntered(int row, int column)
{
    MusicAbstractSongsListTableWidget::itemCellEntered(row, column);
    if(column == 3 || column == 4)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    else
    {
        unsetCursor();
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
    menu.setStyleSheet(TTK::UI::MenuStyle02);
    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"), this, SLOT(playClicked()));
    menu.addAction(tr("Download More..."), this, SLOT(showDownloadWidget()));
    menu.addSeparator();

    createMoreMenu(&menu);

    const bool status = !m_songs->isEmpty();
    menu.addAction(tr("Song Info..."), this, SLOT(showFileInformation()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_local_file"), tr("Open File Dir"), this, SLOT(openFileDir()))->setEnabled(status);
    menu.addAction(QIcon(":/contextMenu/btn_ablum"), tr("Ablum"), this, SLOT(showAlbumQueryWidget()));
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
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->toolWidget->setStyleSheet(QString("#toolWidget{%1}").arg(TTK::UI::BackgroundStyle03));

    m_ui->allSelectedcheckBox->setStyleSheet(TTK::UI::CheckBoxStyle03);
    m_ui->allSelectedcheckBox->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->allSelectedcheckBox->setText(tr("All"));
    connect(m_ui->allSelectedcheckBox, SIGNAL(clicked(bool)), SLOT(selectedAllItems(bool)));

    m_ui->auditionButton->setStyleSheet(TTK::UI::ToolButtonStyle01 + TTK::UI::ToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_audition); }");
    m_ui->addButton->setStyleSheet(TTK::UI::ToolButtonStyle01 + TTK::UI::ToolButtonStyle02 + "QToolButton{ image:url(:/contextMenu/btn_add); }");

    m_ui->auditionButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->addButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->searchLineLabel->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->loadingLabel->setType(MusicGifLabelWidget::Module::CicleBlue);
    m_ui->toolWidget->setStyleSheet(TTK::UI::BackgroundStyle10);

    connect(m_ui->auditionButton, SIGNAL(clicked()), SLOT(auditionButtonClick()));
    connect(m_ui->addButton, SIGNAL(clicked()), SLOT(auditionButtonClick()));
    connect(m_ui->searchLineEdit, SIGNAL(cursorPositionChanged(int,int)), SLOT(searchResultChanged(int,int)));

    connect(m_ui->songlistTable, SIGNAL(cellClicked(int,int)), SLOT(itemCellClicked(int,int)));
    connect(m_ui->songlistTable, SIGNAL(cellDoubleClicked(int,int)), SLOT(itemDoubleClicked(int,int)));

#ifdef Q_OS_UNIX
    m_ui->allSelectedcheckBox->setFocusPolicy(Qt::NoFocus);
    m_ui->searchLineLabel->setFocusPolicy(Qt::NoFocus);
#endif

    m_thread = new MusicSongsManagerThread(this);
    connect(m_thread, SIGNAL(searchFilePathChanged(QStringList)), SLOT(searchFilePathChanged(QStringList)));

    G_CONNECTION_PTR->setValue(className(), this);
    G_CONNECTION_PTR->connect(className(), MusicSongsSummariziedWidget::className());
}

MusicMobileSongsManagerWidget::~MusicMobileSongsManagerWidget()
{
    G_CONNECTION_PTR->removeValue(this);
    G_SINGLE_MANAGER_PTR->removeObject(className());
    clearAllItems();
    m_thread->stop();
    delete m_thread;
    delete m_ui;
}

void MusicMobileSongsManagerWidget::findExtraDevicePath(const QString &dir)
{
    TTK_INFO_STREAM("Start fetch result");
    m_thread->setFindFilePath(dir);
    m_thread->stop();
    m_thread->start();
    m_ui->loadingLabel->run(true);
}

void MusicMobileSongsManagerWidget::selectedAllItems(bool checked)
{
    if(!checked)
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
        MusicToastLabel::popup(tr("Please select one item first"));
        return;
    }

    selectedItemsToPlaylist();
}

void MusicMobileSongsManagerWidget::itemCellClicked(int row, int column)
{
    switch(column)
    {
        case 3:
        case 4: itemDoubleClicked(row, column); break;
        default: break;
    }
}

void MusicMobileSongsManagerWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    mappedSearchRow(m_ui->searchLineEdit->text().length(), row);
    Q_EMIT addSongToPlaylist(QStringList(m_containerItems[row]));
}

void MusicMobileSongsManagerWidget::searchFilePathChanged(const QStringList &path)
{
    TTK_INFO_STREAM("Stop fetch result");

    clearAllItems();
    clearSearchResult();
    m_ui->searchLineEdit->clear();

    m_containerItems = path;
    m_ui->songlistTable->addCellItems(m_containerItems);
    m_ui->loadingLabel->run(false);
}

void MusicMobileSongsManagerWidget::searchResultChanged(int, int column)
{
    TTKIntList result;
    for(int i = 0; i < m_containerItems.count(); ++i)
    {
        if(QFileInfo(m_containerItems[i]).fileName().contains(m_ui->searchLineEdit->text(), Qt::CaseInsensitive))
        {
            result << i;
        }
    }

    QStringList data;
    for(const int index : qAsConst(result))
    {
        data.append(m_containerItems[index]);
    }

    m_searchResultLevel = column;
    m_searchResultCache.insert(column, result);

    clearAllItems();
    m_ui->songlistTable->addCellItems(data);
}

void MusicMobileSongsManagerWidget::clearAllItems()
{
    m_ui->songlistTable->removeItems();
    if(m_ui->allSelectedcheckBox->isChecked())
    {
        m_ui->allSelectedcheckBox->click();
    }
}

void MusicMobileSongsManagerWidget::selectedItemsToPlaylist()
{
    TTKIntSet auditionRow; //if selected multi rows
    for(QTableWidgetItem *item : m_ui->songlistTable->selectedItems())
    {
        int row = item->row();
        mappedSearchRow(m_ui->searchLineEdit->text().length(), row);
        auditionRow.insert(row);
    }

    TTKIntList indexs = auditionRow.values();
    std::sort(indexs.begin(), indexs.end());

    QStringList items;
    for(const int index : qAsConst(indexs))
    {
        items << m_containerItems[index];
    }

    Q_EMIT addSongToPlaylist(items);
}
