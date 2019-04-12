#include "musicwebmvradioinfowidget.h"
#include "musicmvradioprogramthread.h"
#include "musicsettingmanager.h"
#include "musicdownloadsourcethread.h"
#include "musicdownloadwidget.h"
#include "musicrightareawidget.h"

#include <qmath.h>
Q_DECLARE_METATYPE(MusicObject::MusicSongInformation)

MusicWebMVRadioInfoTableWidget::MusicWebMVRadioInfoTableWidget(QWidget *parent)
    : MusicQueryFoundTableWidget(parent)
{

}

MusicWebMVRadioInfoTableWidget::~MusicWebMVRadioInfoTableWidget()
{
    clearAllItems();
}

void MusicWebMVRadioInfoTableWidget::setQueryInput(MusicDownLoadQueryThreadAbstract *query)
{
    MusicQueryFoundTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_downLoadManager, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}

void MusicWebMVRadioInfoTableWidget::listCellClicked(int row, int column)
{
    MusicQueryTableWidget::listCellClicked(row, column);
    switch(column)
    {
        case 5:
        case 6:
            {
                const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
                if(row < 0 || row >= musicSongInfos.count())
                {
                    return;
                }
                MusicRightAreaWidget::instance()->musicMovieRadioSearch(QVariant::fromValue<MusicObject::MusicSongInformation>(musicSongInfos[row]));
            }
            break;
        case 7:
            musicDownloadLocal(row);
            break;
        default:
            break;
    }
}

void MusicWebMVRadioInfoTableWidget::musicDownloadLocal(int row)
{
    const MusicObject::MusicSongInformations musicSongInfos(m_downLoadManager->getMusicSongInfos());
    if(row < 0 || row >= musicSongInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(musicSongInfos[row], MusicDownLoadQueryThreadAbstract::MovieQuery);
    download->show();
}

void MusicWebMVRadioInfoTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}



MusicWebMVRadioInfoWidget::MusicWebMVRadioInfoWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_foundTableWidget = new MusicWebMVRadioInfoTableWidget(this);
    m_foundTableWidget->hide();

    MusicMVRadioProgramThread *v = new MusicMVRadioProgramThread(this);
    m_foundTableWidget->setQueryInput(v);
    connect(v, SIGNAL(createCategoryItem(MusicResultsItem)), SLOT(createCategoryInfoItem(MusicResultsItem)));
}

void MusicWebMVRadioInfoWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_foundTableWidget->startSearchQuery(name);
}

void MusicWebMVRadioInfoWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicWebMVRadioInfoWidget::resizeWindow()
{
    m_foundTableWidget->resizeWindow();
    if(!m_resizeWidgets.isEmpty())
    {
        int width = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
        width = width - WINDOW_WIDTH_MIN + 390;

        QLabel *label = m_resizeWidgets[0];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, width));

        label = m_resizeWidgets[1];
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, width));
    }
}

void MusicWebMVRadioInfoWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicWebMVRadioInfoWidget::createCategoryInfoItem(const MusicResultsItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(!m_resizeWidgets.isEmpty())
    {
        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
        {
            download->startToDownload(item.m_coverUrl);
        }

        QLabel *label = m_resizeWidgets[0];
        label->setToolTip(item.m_name);
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 390));

        label = m_resizeWidgets[1];
        label->setToolTip(tr("Type: %1").arg(item.m_nickName));
        label->setText(MusicUtils::Widget::elidedText(label->font(), label->toolTip(), Qt::ElideRight, 390));
    }
}

void MusicWebMVRadioInfoWidget::downloadMVsButtonClicked()
{
    m_foundTableWidget->downloadBatchData(false);
}

void MusicWebMVRadioInfoWidget::createLabels()
{
    initThirdWidget();
    m_container->show();

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidget(m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MCheckBoxStyle01 + MusicUIObject::MPushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QWidget *firstTopFuncWidget = new QWidget(function);
    QHBoxLayout *firstTopFuncLayout = new QHBoxLayout(firstTopFuncWidget);
    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> MVRadio > %1 </font>").arg(m_currentPlaylistItem.m_name));
    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(90, 30);
    backButton->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    backButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(backButton, SIGNAL(clicked()), this, SIGNAL(backToMainMenu()));
    firstTopFuncLayout->addWidget(firstLabel);
    firstTopFuncLayout->addWidget(backButton);
    grid->addWidget(firstTopFuncWidget);
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
    QFont nameFont = nameLabel->font();
    nameFont.setPixelSize(20);
    nameLabel->setFont(nameFont);
    nameLabel->setStyleSheet(MusicUIObject::MFontStyle01);
    nameLabel->setText("-");
    QLabel *typeLabel = new QLabel(topLineWidget);
    typeLabel->setStyleSheet(MusicUIObject::MColorStyle04 + MusicUIObject::MFontStyle03);
    typeLabel->setText("-");

    topLineLayout->addWidget(nameLabel);
    topLineLayout->addStretch(1);
    topLineLayout->addWidget(typeLabel);
    topLineLayout->addStretch(5);
    topLineWidget->setLayout(topLineLayout);

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

    m_resizeWidgets << nameLabel << typeLabel;
}

void MusicWebMVRadioInfoWidget::initThirdWidget()
{
    QWidget *songWidget = new QWidget(this);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    QWidget *middleFuncWidget = new QWidget(songWidget);
    middleFuncWidget->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 5, 0, 5);
    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    QCheckBox *allCheckBox = new QCheckBox(" " + tr("all"), middleFuncWidget);
    QPushButton *downloadButton = new QPushButton(tr("download"), middleFuncWidget);
    downloadButton->setFixedSize(55, 25);
    downloadButton->setCursor(QCursor(Qt::PointingHandCursor));

#ifdef Q_OS_UNIX
    allCheckBox->setFocusPolicy(Qt::NoFocus);
    downloadButton->setFocusPolicy(Qt::NoFocus);
#endif

    middleFuncLayout->addWidget(marginLabel);
    middleFuncLayout->addWidget(allCheckBox);
    middleFuncLayout->addStretch(1);
    middleFuncLayout->addWidget(downloadButton);
    connect(allCheckBox, SIGNAL(clicked(bool)), m_foundTableWidget, SLOT(setSelectedAllItems(bool)));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadMVsButtonClicked()));

    vlayout->addWidget(middleFuncWidget);
    //
    vlayout->addWidget(m_foundTableWidget);
    vlayout->addStretch(1);
    songWidget->setLayout(vlayout);

    m_foundTableWidget->show();
    m_container->addWidget(songWidget);
}
