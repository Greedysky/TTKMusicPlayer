#include "musictoplistfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musicdownloadsourcethread.h"
#include "musicsettingmanager.h"
#include "musictoplistfoundcategorypopwidget.h"

#include <QCheckBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QScrollArea>

MusicTopListFoundTableWidget::MusicTopListFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicTopListFoundTableWidget::~MusicTopListFoundTableWidget()
{
    clearAllItems();
}

QString MusicTopListFoundTableWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicTopListFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}



MusicTopListFoundWidget::MusicTopListFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_categoryButton = nullptr;
    m_toplistTableWidget = new MusicTopListFoundTableWidget(this);

    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getToplistThread(this);
    m_toplistTableWidget->setQueryInput(m_downloadThread);
    connect(m_downloadThread, SIGNAL(createToplistInfoItem(MusicPlaylistItem)), SLOT(createToplistInfoItem(MusicPlaylistItem)));
}

MusicTopListFoundWidget::~MusicTopListFoundWidget()
{
    delete m_iconLabel;
    delete m_songItemsLabel;
    delete m_toplistTableWidget;
    delete m_categoryButton;
}

QString MusicTopListFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicTopListFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);

    m_downloadThread->setQueryAllRecords(true);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, QString());

    createLabels();
}

void MusicTopListFoundWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicTopListFoundWidget::resizeWindow()
{
    m_toplistTableWidget->resizeWindow();

    if(!m_resizeWidget.isEmpty())
    {
        int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        width = width - WINDOW_WIDTH_MIN;

        QLabel *label = m_resizeWidget[0];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));

        label = m_resizeWidget[1];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));

        label = m_resizeWidget[2];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));

        label = m_resizeWidget[3];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));
    }
}

void MusicTopListFoundWidget::queryAllFinished()
{
    MusicObject::MusicSongInformations musicSongInfos(m_toplistTableWidget->getMusicSongInfos());
    m_songItemsLabel->setText("|" + tr("songItems") + QString("(%1)").arg(musicSongInfos.count()));
}

void MusicTopListFoundWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01 + MusicUIObject::MPushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#169AF3> Rank </font>"));
    grid->addWidget(firstLabel);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *categoryWidget = new QWidget(function);
    QHBoxLayout *categoryWidgetLayout = new QHBoxLayout(categoryWidget);
    m_categoryButton = new MusicToplistFoundCategoryPopWidget(categoryWidget);
    m_categoryButton->setCategory(m_downloadThread->getQueryServer(), this);
    categoryWidgetLayout->addStretch(1);
    categoryWidgetLayout->addWidget(m_categoryButton);
    categoryWidget->setLayout(categoryWidgetLayout);
    grid->addWidget(categoryWidget);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(180, 180);

    ////////////////////////////////////////////////////////////////////////////
    QWidget *topLineWidget = new QWidget(topFuncWidget);
    QVBoxLayout *topLineLayout = new QVBoxLayout(topLineWidget);
    topLineLayout->setContentsMargins(10, 5, 5, 0);
    QLabel *nameLabel = new QLabel(topLineWidget);
    QFont toplistFont = nameLabel->font();
    toplistFont.setPixelSize(20);
    nameLabel->setFont(toplistFont);
    nameLabel->setStyleSheet(MusicUIObject::MFontStyle01);
    QLabel *playCountLabel = new QLabel(topLineWidget);
    playCountLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    QLabel *updateTimeLabel = new QLabel(topLineWidget);
    updateTimeLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    QLabel *descriptionLabel = new QLabel(topLineWidget);
    descriptionLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    descriptionLabel->setWordWrap(true);

    topLineLayout->addWidget(nameLabel);
    topLineLayout->addWidget(playCountLabel);
    topLineLayout->addWidget(updateTimeLabel);
    topLineLayout->addWidget(descriptionLabel);
    topLineWidget->setLayout(topLineLayout);

    ////////////////////////////////////////////////////////////////////////////
    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    ////////////////////////////////////////////////////////////////////////////
    m_songItemsLabel = new QLabel(function);
    m_songItemsLabel->setFixedHeight(50);
    m_songItemsLabel->setStyleSheet(MusicUIObject::MFontStyle04);
    grid->addWidget(m_songItemsLabel);

    QWidget *middleFuncWidget = new QWidget(function);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    m_allCheckBox = new QCheckBox(" " + tr("all"), middleFuncWidget);
    QPushButton *playButton = new QPushButton(tr("play"), middleFuncWidget);
    playButton->setIcon(QIcon(":/contextMenu/btn_play_white"));
    playButton->setIconSize(QSize(14, 14));
    playButton->setFixedSize(55, 25);
    playButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *addButton = new QPushButton(tr("add"), middleFuncWidget);
    addButton->setFixedSize(55, 25);
    addButton->setCursor(QCursor(Qt::PointingHandCursor));
    QPushButton *downloadButton = new QPushButton(tr("download"), middleFuncWidget);
    downloadButton->setFixedSize(55, 25);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    m_allCheckBox->setFocusPolicy(Qt::NoFocus);
    playButton->setFocusPolicy(Qt::NoFocus);
    addButton->setFocusPolicy(Qt::NoFocus);
    downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    middleFuncLayout->addWidget(marginLabel);
    middleFuncLayout->addWidget(m_allCheckBox);
    middleFuncLayout->addStretch(1);
    middleFuncLayout->addWidget(playButton);
    middleFuncLayout->addWidget(addButton);
    middleFuncLayout->addWidget(downloadButton);
    connect(m_allCheckBox, SIGNAL(clicked(bool)), m_toplistTableWidget, SLOT(setSelectedAllItems(bool)));
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadButtonClicked()));
    connect(addButton, SIGNAL(clicked()), SLOT(addButtonClicked()));
    grid->addWidget(middleFuncWidget);
    //////////////////////////////////////////////////////////////////////
    grid->addWidget(m_toplistTableWidget);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidget << nameLabel << playCountLabel << updateTimeLabel << descriptionLabel;
}

void MusicTopListFoundWidget::createToplistInfoItem(const MusicPlaylistItem &item)
{
    if(!m_resizeWidget.isEmpty())
    {
        QLabel *label = m_resizeWidget[0];
        label->setToolTip(item.m_name);
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410));

        label = m_resizeWidget[1];
        label->setToolTip(tr("PlayCount: %1").arg(item.m_playCount));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410));

        label = m_resizeWidget[2];
        label->setToolTip(tr("UpdateTime: %1").arg(item.m_updateTime));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410));

        label = m_resizeWidget[3];
        label->setToolTip(tr("Description: %1").arg(item.m_description));
        label->setText(label->toolTip());
    }

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != "null")
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicTopListFoundWidget::downLoadFinished(const QByteArray &data)
{
    if(m_iconLabel)
    {
        QPixmap pix;
        pix.loadFromData(data);
        m_iconLabel->setPixmap(pix.scaled(m_iconLabel->size()));
    }
}

void MusicTopListFoundWidget::playButtonClicked()
{
    m_toplistTableWidget->downloadDataFrom(true);
}

void MusicTopListFoundWidget::downloadButtonClicked()
{
    m_toplistTableWidget->downloadBatchData();
}

void MusicTopListFoundWidget::addButtonClicked()
{
    m_toplistTableWidget->downloadDataFrom(false);
}

void MusicTopListFoundWidget::categoryChanged(const MusicPlaylistCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_allCheckBox->setChecked(false);

        m_categoryButton->setText(category.m_name);
        m_categoryButton->closeMenu();

        m_songItemsLabel->clear();
        m_downloadThread->setQueryAllRecords(true);
        m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, category.m_id);
    }
}
