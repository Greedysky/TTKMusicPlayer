#include "musicsoundkmicrosearchwidget.h"
#include "musickwquerymovierequest.h"
#include "musicbdquerylearnrequest.h"
#include "musicitemsearchedit.h"
#include "musicgiflabelwidget.h"
#include "musictoastlabel.h"
#include "musicitemdelegate.h"
#include "musicdownloadwidget.h"

#include <QButtonGroup>

MusicSoundKMicroSearchTableWidget::MusicSoundKMicroSearchTableWidget(QWidget *parent)
    : MusicItemSearchTableWidget(parent)
{
    setColumnCount(5);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 290);
    headerview->resizeSection(2, 24);
    headerview->resizeSection(3, 24);
    headerview->resizeSection(4, 24);

    m_queryMovieMode = true;
    viewport()->setStyleSheet(MusicUIObject::MQSSBackgroundStyle02);
    m_backgroundColor = Qt::black;
}

MusicSoundKMicroSearchTableWidget::~MusicSoundKMicroSearchTableWidget()
{
    clearAllItems();
}

void MusicSoundKMicroSearchTableWidget::startSearchQuery(const QString &text)
{
    if(!G_NETWORK_PTR->isOnline())   // no network connection
    {
        clearAllItems();
        return;
    }

    m_loadingLabel->run(true);

    if(m_queryMovieMode)
    {
        MusicKWQueryMovieRequest *d = new MusicKWQueryMovieRequest(this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
        setQueryInput(d);
        m_networkRequest->startToSearch(MusicAbstractQueryRequest::MovieQuery, text);
    }
    else
    {
        MusicBDQueryLearnRequest *d = new MusicBDQueryLearnRequest(this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(createFinishedItem()));
        setQueryInput(d);
        m_networkRequest->startToSearch(MusicAbstractQueryRequest::MusicQuery, text);
    }
}

void MusicSoundKMicroSearchTableWidget::musicDownloadLocal(int row)
{
    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    const MusicObject::MusicSongInformationList musicSongInfos(m_networkRequest->musicSongInfoList());
    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], m_queryMovieMode ? MusicAbstractQueryRequest::MovieQuery : MusicAbstractQueryRequest::MusicQuery);
    download->show();
}

void MusicSoundKMicroSearchTableWidget::setQueryMovieFlag(bool flag)
{
    m_queryMovieMode = flag;
}

void MusicSoundKMicroSearchTableWidget::clearAllItems()
{
    MusicItemSearchTableWidget::clearAllItems();
    setColumnCount(5);
}

void MusicSoundKMicroSearchTableWidget::createSearchedItem(const MusicSearchedItem &songItem)
{
    const int count = rowCount();
    setRowCount(count + 1);

    QHeaderView *headerview = horizontalHeader();
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(MUSIC_CHECK_ROLE, Qt::Unchecked);
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setBackground(m_backgroundColor);
#else
    item->setBackgroundColor(m_backgroundColor);
#endif
    setItem(count, 0, item);

                      item = new QTableWidgetItem;
    item->setToolTip(songItem.m_singerName + " - " + songItem.m_songName);
    item->setText(MusicUtils::Widget::elidedText(font(), item->toolTip(), Qt::ElideRight, headerview->sectionSize(1) - 15));
#if TTK_QT_VERSION_CHECK(5,13,0)
    item->setForeground(QColor(100, 100, 100));
#else
    item->setTextColor(QColor(100, 100, 100));
#endif
    setItem(count, 1, item);

                      item = new QTableWidgetItem;
    item->setIcon(QIcon(randSimulation()));
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

void MusicSoundKMicroSearchTableWidget::itemCellEntered(int row, int column)
{
    MusicItemSearchTableWidget::itemCellEntered(row, column);
    QTableWidgetItem *it = item(row, 0);
    if(it)
    {
#if TTK_QT_VERSION_CHECK(5,13,0)
        it->setBackground(m_backgroundColor);
#else
        it->setBackgroundColor(m_backgroundColor);
#endif
    }
}

void MusicSoundKMicroSearchTableWidget::itemCellClicked(int row, int column)
{
    MusicItemSearchTableWidget::itemCellClicked(row, column);
    switch(column)
    {
        case 4: dataDownloadPlay(row); break;
        default: break;
    }
}

void MusicSoundKMicroSearchTableWidget::dataDownloadPlay(int row)
{
    if(row < 0 || (row >= rowCount() - 1))
    {
        MusicToastLabel::popup(tr("Please select one item first!"));
        return;
    }

    const MusicObject::MusicSongInformationList musicSongInfos(m_networkRequest->musicSongInfoList());
    for(const MusicObject::MusicSongProperty &prop : qAsConst(musicSongInfos[row].m_songProps))
    {
        Q_EMIT mediaUrlChanged(m_queryMovieMode, prop.m_url, m_queryMovieMode ? QString() : musicSongInfos[row].m_lrcUrl);
    }
}

void MusicSoundKMicroSearchTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicItemSearchTableWidget::contextMenuEvent(event);
    QMenu menu(this);
    createContextMenu(menu);

    menu.exec(QCursor::pos());
}



MusicSoundKMicroSearchWidget::MusicSoundKMicroSearchWidget(QWidget *parent)
    : MusicAbstractMoveSingleWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::Tool);
    blockMoveOption(true);

    raise();
    resize(408, 498);

    QVBoxLayout *layout = new QVBoxLayout(m_container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *topWidget = new QLabel(tr(" Search"), this);
    topWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle06 + MusicUIObject::MQSSColorStyle01);
    topWidget->setFixedHeight(35);

    QWidget *searchWidget = new QWidget(this);
    searchWidget->setStyleSheet(MusicUIObject::MQSSBackgroundStyle11);
    searchWidget->setFixedHeight(35);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(5, 0, 5, 0);
    searchWidget->setLayout(searchLayout);

    m_searchEdit = new MusicItemQueryEdit(searchWidget);
    m_searchEdit->setFixedHeight(25);
    QRadioButton *mvButton = new QRadioButton(tr("Movie"), searchWidget);
    mvButton->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);
    QRadioButton *songButton = new QRadioButton(tr("Song"), searchWidget);
    songButton->setStyleSheet(MusicUIObject::MQSSRadioButtonStyle01);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(mvButton, 0);
    buttonGroup->addButton(songButton, 1);
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(buttonGroup, SIGNAL(idClicked(int)), SLOT(setQueryMovieFlag(int)));
#else
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(setQueryMovieFlag(int)));
#endif
    searchLayout->addWidget(mvButton, 1);
    searchLayout->addWidget(songButton, 1);
    searchLayout->addStretch(1);
    searchLayout->addWidget(m_searchEdit, 10);
#ifdef Q_OS_UNIX
    mvButton->setFocusPolicy(Qt::NoFocus);
    songButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_searchTableWidget = new MusicSoundKMicroSearchTableWidget(this);

    layout->addWidget(topWidget);
    layout->addWidget(searchWidget);
    layout->addWidget(m_searchTableWidget);
    m_container->setLayout(layout);

    m_queryMovieMode = true;
    mvButton->setChecked(true);

    connect(m_searchEdit, SIGNAL(clicked()), SLOT(startToSearch()));
    connect(m_searchEdit->editor(), SIGNAL(enterFinished(QString)), SLOT(startToSearch()));
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
    m_searchEdit->editor()->setText(name);
    startToSearch();
}

void MusicSoundKMicroSearchWidget::startToSearch()
{
    m_searchTableWidget->setQueryMovieFlag(m_queryMovieMode);
    m_searchTableWidget->startSearchQuery(m_searchEdit->editor()->text());
}
void MusicSoundKMicroSearchWidget::setQueryMovieFlag(int flag)
{
    m_queryMovieMode = (flag == 0);
}

void MusicSoundKMicroSearchWidget::setCurrentSongName(const QString &name)
{
    Q_UNUSED(name);
    MusicToastLabel::popup(tr("Search not supported!"));
}
