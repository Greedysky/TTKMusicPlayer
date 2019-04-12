#include "musictoplistfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"
#include "musicstringutils.h"
#include "musicdownloadsourcethread.h"
#include "musicsettingmanager.h"
#include "musictoplistfoundcategorypopwidget.h"

MusicToplistFoundTableWidget::MusicToplistFoundTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicToplistFoundTableWidget::~MusicToplistFoundTableWidget()
{
    clearAllItems();
}

void MusicToplistFoundTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}



MusicToplistFoundWidget::MusicToplistFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_categoryButton = nullptr;

    m_foundTableWidget = new MusicToplistFoundTableWidget(this);
    m_foundTableWidget->hide();
    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getToplistThread(this);
    m_foundTableWidget->setQueryInput(m_downloadThread);

    connect(m_downloadThread, SIGNAL(createToplistInfoItem(MusicResultsItem)), SLOT(createToplistInfoItem(MusicResultsItem)));
}

MusicToplistFoundWidget::~MusicToplistFoundWidget()
{
    delete m_categoryButton;
}

void MusicToplistFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);

    m_downloadThread->setQueryAllRecords(true);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, QString());

    createLabels();
}

void MusicToplistFoundWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicToplistFoundWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();

    if(!m_resizeWidgets.isEmpty())
    {
        int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN;

        QLabel *label = m_resizeWidgets[0];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));

        label = m_resizeWidgets[1];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));

        label = m_resizeWidgets[2];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));

        label = m_resizeWidgets[3];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410 + width));
    }
}

void MusicToplistFoundWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicToplistFoundWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    initFirstWidget();
    m_container->show();

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
    firstLabel->setText(tr("<font color=#158FE1> Rank </font>"));
    grid->addWidget(firstLabel);
    //
    QWidget *categoryWidget = new QWidget(function);
    QHBoxLayout *categoryWidgetLayout = new QHBoxLayout(categoryWidget);
    m_categoryButton = new MusicToplistFoundCategoryPopWidget(categoryWidget);
    m_categoryButton->setCategory(m_downloadThread->getQueryServer(), this);
    categoryWidgetLayout->addWidget(m_categoryButton);
    categoryWidgetLayout->addStretch(1);
    categoryWidget->setLayout(categoryWidgetLayout);
    grid->addWidget(categoryWidget);
    //
    QWidget *topFuncWidget = new QWidget(function);
    QHBoxLayout *topFuncLayout = new QHBoxLayout(topFuncWidget);

    m_iconLabel = new QLabel(topFuncWidget);
    m_iconLabel->setPixmap(QPixmap(":/image/lb_warning").scaled(180, 180));
    m_iconLabel->setFixedSize(210, 180);
    //
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

    //
    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);
    //

    QWidget *functionWidget = new QWidget(this);
    functionWidget->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    QHBoxLayout *hlayout = new QHBoxLayout(functionWidget);
    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("songItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(m_songButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);
    QButtonGroup *group = new QButtonGroup(this);
    group->addButton(m_songButton, 0);
    connect(group, SIGNAL(buttonClicked(int)), m_container, SLOT(setCurrentIndex(int)));

#ifdef Q_OS_UNIX
    m_songButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    //
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets << nameLabel << playCountLabel << updateTimeLabel << descriptionLabel;
}

void MusicToplistFoundWidget::createToplistInfoItem(const MusicResultsItem &item)
{
    if(!m_resizeWidgets.isEmpty())
    {
        QLabel *label = m_resizeWidgets[0];
        label->setToolTip(item.m_name);
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410));

        label = m_resizeWidgets[1];
        label->setToolTip(tr("PlayCount: %1").arg(item.m_playCount));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410));

        label = m_resizeWidgets[2];
        label->setToolTip(tr("UpdateTime: %1").arg(item.m_updateTime));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 410));

        label = m_resizeWidgets[3];
        label->setToolTip(tr("Description: %1").arg(item.m_description));
        label->setText(label->toolTip());
    }

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
    {
        download->startToDownload(item.m_coverUrl);
    }
}

void MusicToplistFoundWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_categoryButton->setText(category.m_name);
        m_categoryButton->closeMenu();

        m_songButton->setText(tr("songItems"));
        m_downloadThread->setQueryAllRecords(true);
        m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, category.m_id);
    }
}
