#include "musicsoundkmicrosearchwidget.h"
#include "musicdownloadquerykwmoviethread.h"
#include "musicdownloadquerybdlearnthread.h"
#include "musiclocalsongsearchedit.h"
#include "musicgiflabelwidget.h"
#include "musicmessagebox.h"
#include "musicuiobject.h"
#include "musicitemdelegate.h"
#include "musicdownloadwidget.h"

#include <QButtonGroup>

MusicSoundKMicroSearchTableWidget::MusicSoundKMicroSearchTableWidget(QWidget *parent)
    : MusicQueryItemTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 290);
    headerview->resizeSection(2, 24);
    headerview->resizeSection(3, 24);
    headerview->resizeSection(4, 24);

    m_queryMovieMode = true;
    viewport()->setStyleSheet(MusicUIObject::MBackgroundStyle02);
    m_defaultBkColor = Qt::black;
}

MusicSoundKMicroSearchTableWidget::~MusicSoundKMicroSearchTableWidget()
{
    clearAllItems();
}

void MusicSoundKMicroSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!M_NETWORK_PTR->isOnline())   //no network connection
    {
        clearAllItems();
        emit showDownLoadInfoFor(MusicObject::DW_DisConnection);
        return;
    }

    m_loadingLabel->run(true);

    if(m_queryMovieMode)
    {
        MusicDownLoadQueryKWMovieThread *d = new MusicDownLoadQueryKWMovieThread(this);
        d->setQueryExtraMovie(false);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
        setQueryInput( d );
        m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MovieQuery, text);
    }
    else
    {
        MusicDownLoadQueryBDLearnThread *d = new MusicDownLoadQueryBDLearnThread(this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
        setQueryInput( d );
        m_downLoadManager->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, text);
    }
}

void MusicSoundKMicroSearchTableWidget::musicDownloadLocal(int row)
{
    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], m_queryMovieMode ? MusicDownLoadQueryThreadAbstract::MovieQuery : MusicDownLoadQueryThreadAbstract::MusicQuery);
    download->show();
}

void MusicSoundKMicroSearchTableWidget::setQueryMovieFlag(bool flag)
{
    m_queryMovieMode = flag;
}

void MusicSoundKMicroSearchTableWidget::clearAllItems()
{
    MusicQueryItemTableWidget::clearAllItems();
    setColumnCount(5);
}

void MusicSoundKMicroSearchTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, false);
    item->setBackgroundColor(m_defaultBkColor);
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName + " - " + songItem.m_songName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
    item->setTextColor(QColor(100, 100, 100));
    item->setTextAlignment(Qt::AlignCenter);
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randToGetStrength()));
    setItem(count, 2, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/tiny/lb_server_type"));
    item->setToolTip(songItem.m_type);
    setItem(count, 3, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(":/contextMenu/btn_audition"));
    setItem(count, 4, item);
}

void MusicSoundKMicroSearchTableWidget::itemDoubleClicked(int row, int column)
{
    if(column <= 0)
    {
        return;
    }

    dataDownloadPlay(row);
}

void MusicSoundKMicroSearchTableWidget::listCellEntered(int row, int column)
{
    MusicQueryItemTableWidget::listCellEntered(row, column);
    QTableWidgetItem *it = item(row, 0);
    if(it)
    {
        it->setBackgroundColor(m_defaultBkColor);
    }
}

void MusicSoundKMicroSearchTableWidget::listCellClicked(int row, int column)
{
    MusicQueryItemTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 4:
            dataDownloadPlay(row);
            break;
        default:
            break;
    }
}

void MusicSoundKMicroSearchTableWidget::dataDownloadPlay(int row)
{
    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicMessageBox message;
        message.setText(tr("Please Select One Item First!"));
        message.exec();
        return;
    }

    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    foreach(const MusicObject::MusicSongAttribute &attr, musicSongInfos[row].m_songAttrs)
    {
        emit mediaUrlChanged(m_queryMovieMode, attr.m_url, m_queryMovieMode ? QString() : musicSongInfos[row].m_lrcUrl);
    }
}

void MusicSoundKMicroSearchTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryItemTableWidget::contextMenuEvent(event);
    QMenu rightClickMenu(this);
    createContextMenu(rightClickMenu);

    rightClickMenu.exec(QCursor::pos());
}



MusicSoundKMicroSearchWidget::MusicSoundKMicroSearchWidget(QWidget *parent)
    : MusicAbstractMoveSingleWidget(parent)
{
    ///Remove the title bar
    setWindowFlags(windowFlags() | Qt::Tool);
    blockMoveOption(true);

    raise();
    resize(408, 498);

    QVBoxLayout *layout = new QVBoxLayout(m_container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *topWidget = new QLabel(tr(" Search"), this);
    topWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06 + MusicUIObject::MColorStyle01);
    topWidget->setFixedHeight(35);

    QWidget *searchWidget = new QWidget(this);
    searchWidget->setStyleSheet(MusicUIObject::MBackgroundStyle15);
    searchWidget->setFixedHeight(35);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(5, 0, 0, 0);
    searchWidget->setLayout(searchLayout);

    m_searchEdit = new MusicLocalSongSearchEdit(this);
    m_searchEdit->setFixedHeight(25);
    m_searchEdit->setStyleSheet(MusicUIObject::MBorderStyle04);
    QPushButton *searchButton = new QPushButton(searchWidget);
    searchButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    searchButton->setIcon(QIcon(":/tiny/btn_search_main_hover"));
    searchButton->setCursor(QCursor(Qt::PointingHandCursor));
    searchButton->setIconSize(QSize(25, 25));
    QRadioButton *mvButton = new QRadioButton(tr("MV"), searchWidget);
    mvButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    QRadioButton *songButton = new QRadioButton(tr("Song"), searchWidget);
    songButton->setStyleSheet(MusicUIObject::MRadioButtonStyle01);
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(mvButton, 0);
    group->addButton(songButton, 1);
    connect(group, SIGNAL(buttonClicked(int)), SLOT(setQueryMovieFlag(int)));
    searchLayout->addWidget(mvButton);
    searchLayout->addWidget(songButton);
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addWidget(searchButton);
#ifdef Q_OS_UNIX
    mvButton->setFocusPolicy(Qt::NoFocus);
    songButton->setFocusPolicy(Qt::NoFocus);
    searchButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_searchTableWidget = new MusicSoundKMicroSearchTableWidget(this);

    layout->addWidget(topWidget);
    layout->addWidget(searchWidget);
    layout->addWidget(m_searchTableWidget);
    m_container->setLayout(layout);

    m_queryMovieMode = true;
    mvButton->setChecked(true);

    connect(searchButton, SIGNAL(clicked()), SLOT(startToSearch()));
    connect(m_searchEdit, SIGNAL(enterFinished(QString)), SLOT(startToSearch()));
    connect(m_searchTableWidget, SIGNAL(restartSearchQuery(QString)), SLOT(setCurrentSongName(QString)));
}

MusicSoundKMicroSearchWidget::~MusicSoundKMicroSearchWidget()
{
    delete m_searchEdit;
    delete m_searchTableWidget;
}

void MusicSoundKMicroSearchWidget::connectTo(QObject *obj)
{
    connect(m_searchTableWidget, SIGNAL(mediaUrlChanged(bool,QString,QString)), obj, SLOT(mediaUrlChanged(bool,QString,QString)));
}

void MusicSoundKMicroSearchWidget::startSeachKMicro(const QString &name)
{
    m_searchEdit->setText(name);
    startToSearch();
}

void MusicSoundKMicroSearchWidget::startToSearch()
{
    m_searchTableWidget->setQueryMovieFlag(m_queryMovieMode);
    m_searchTableWidget->startSearchQuery(m_searchEdit->text());
}
void MusicSoundKMicroSearchWidget::setQueryMovieFlag(int flag)
{
    m_queryMovieMode = (flag == 0);
}

void MusicSoundKMicroSearchWidget::setCurrentSongName(const QString &name)
{
    Q_UNUSED(name);
    MusicMessageBox message;
    message.setText(tr("Search Not Supported!"));
    message.exec();
}
