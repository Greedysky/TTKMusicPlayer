#include "musicwebmvradioinfowidget.h"
#include "musicmvradioprogramrequest.h"
#include "musicsettingmanager.h"
#include "musicdownloadcoverrequest.h"
#include "musicdownloadwidget.h"
#include "musicrightareawidget.h"

#include <qmath.h>
Q_DECLARE_METATYPE(MusicObject::MusicSongInformation)

MusicWebMVRadioInfoTableWidget::MusicWebMVRadioInfoTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicWebMVRadioInfoTableWidget::~MusicWebMVRadioInfoTableWidget()
{
    removeItems();
}

void MusicWebMVRadioInfoTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}

void MusicWebMVRadioInfoTableWidget::musicDownloadLocal(int row)
{
    const MusicObject::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(row < 0 || row >= songInfos.count())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(songInfos[row], MusicAbstractQueryRequest::QueryType::Movie);
    download->show();
}

void MusicWebMVRadioInfoTableWidget::itemCellClicked(int row, int column)
{
    MusicQueryTableWidget::itemCellClicked(row, column);
    switch(column)
    {
        case 5:
        case 6:
        {
            const MusicObject::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
            if(row < 0 || row >= songInfos.count())
            {
                return;
            }
            MusicRightAreaWidget::instance()->musicMovieRadioSearch(QVariant::fromValue<MusicObject::MusicSongInformation>(songInfos[row]));
            break;
        }
        case 7: musicDownloadLocal(row); break;
        default: break;
    }
}

void MusicWebMVRadioInfoTableWidget::itemDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    itemCellClicked(row, 5);
}

void MusicWebMVRadioInfoTableWidget::actionChanged(QAction *action)
{
    const int row = currentRow();
    const MusicObject::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(row < 0 || row >= songInfos.count())
    {
        return;
    }

    switch(action->data().toInt())
    {
        case 0: itemCellClicked(row, 6); break;
        case 1: itemCellClicked(row, 7); break;
        default: break;
    }
}

void MusicWebMVRadioInfoTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    MusicQueryTableWidget::contextMenuEvent(event);

    QMenu menu;
    menu.setStyleSheet(MusicUIObject::MQSSMenuStyle02);

    const int row = currentRow();
    const MusicObject::MusicSongInformationList songInfos(m_networkRequest->songInfoList());
    if(row < 0 || row >= songInfos.count())
    {
        return;
    }

    menu.addAction(QIcon(":/contextMenu/btn_play"), tr("Play"))->setData(0);
    menu.addAction(tr("Download More..."))->setData(1);
    connect(&menu, SIGNAL(triggered(QAction*)), SLOT(actionChanged(QAction*)));

    menu.exec(QCursor::pos());
}



MusicWebMVRadioInfoWidget::MusicWebMVRadioInfoWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_queryTableWidget = new MusicWebMVRadioInfoTableWidget(this);
    m_queryTableWidget->hide();

    MusicAbstractQueryRequest *d = new MusicMVRadioProgramRequest(this);
    m_queryTableWidget->setQueryInput(d);
    connect(d, SIGNAL(createMVRadioItem(MusicResultDataItem)), SLOT(createMVRadioProgramItem(MusicResultDataItem)));
}

void MusicWebMVRadioInfoWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_queryTableWidget->startSearchQuery(name);
}

void MusicWebMVRadioInfoWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicWebMVRadioInfoWidget::resizeWidget()
{
    m_queryTableWidget->resizeSection();

    if(!m_resizeWidgets.isEmpty())
    {
        int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN + 390;

        TTKResizeWidget *data = &m_resizeWidgets[0];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[1];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
    }
}

void MusicWebMVRadioInfoWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicWebMVRadioInfoWidget::createMVRadioProgramItem(const MusicResultDataItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(!m_resizeWidgets.isEmpty())
    {
        if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != TTK_NULL_STR)
        {
            MusicDownloadCoverRequest *d = new MusicDownloadCoverRequest(this);
            connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
            d->startRequest(item.m_coverUrl);
        }

        int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width();
            width = width - WINDOW_WIDTH_MIN + 390;

        TTKResizeWidget *data = &m_resizeWidgets[0];
        data->m_label->setToolTip(item.m_name);
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[1];
        data->m_label->setToolTip(tr("Type: %1").arg(item.m_nickName));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
    }
}

void MusicWebMVRadioInfoWidget::downloadMVsButtonClicked()
{
    m_queryTableWidget->downloadBatchData(false);
}

void MusicWebMVRadioInfoWidget::createLabels()
{
    initThirdWidget();
    m_container->show();

    layout()->removeWidget(m_mainWindow);
    QScrollArea *scrollArea = new QScrollArea(this);
    MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWindow);
    function->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01 + MusicUIObject::MQSSPushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QWidget *firstTopFuncWidget = new QWidget(function);
    QHBoxLayout *firstTopFuncLayout = new QHBoxLayout(firstTopFuncWidget);
    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> MVRadio > %1 </font>").arg(m_currentPlaylistItem.m_name));
    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(90, 30);
    backButton->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
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
    nameLabel->setStyleSheet(MusicUIObject::MQSSFontStyle01);
    nameLabel->setText(TTK_DEFAULT_STR);
    QLabel *typeLabel = new QLabel(topLineWidget);
    typeLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    typeLabel->setText(TTK_DEFAULT_STR);

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
    functionWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    QHBoxLayout *hlayout = new QHBoxLayout(functionWidget);
    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("SongItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));
    hlayout->addWidget(m_songButton);
    hlayout->addStretch(1);
    functionWidget->setLayout(hlayout);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_songButton, 0);
    QtButtonGroupConnect(buttonGroup, m_container, setCurrentIndex);

#ifdef Q_OS_UNIX
    backButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    //
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets.push_back({nameLabel, nameLabel->font()});
    m_resizeWidgets.push_back({typeLabel, typeLabel->font()});
}

void MusicWebMVRadioInfoWidget::initThirdWidget()
{
    QWidget *songWidget = new QWidget(this);
    QVBoxLayout *vlayout = new QVBoxLayout(songWidget);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);

    QWidget *middleFuncWidget = new QWidget(songWidget);
    middleFuncWidget->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
    QHBoxLayout *middleFuncLayout = new QHBoxLayout(middleFuncWidget);
    middleFuncLayout->setContentsMargins(0, 5, 0, 5);
    QLabel *marginLabel = new QLabel(middleFuncWidget);
    marginLabel->setFixedWidth(1);
    QCheckBox *allCheckBox = new QCheckBox(" " + tr("All"), middleFuncWidget);
    QPushButton *downloadButton = new QPushButton(tr("Download"), middleFuncWidget);
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
    connect(allCheckBox, SIGNAL(clicked(bool)), m_queryTableWidget, SLOT(checkedItemsState(bool)));
    connect(downloadButton, SIGNAL(clicked()), SLOT(downloadMVsButtonClicked()));

    vlayout->addWidget(middleFuncWidget);
    //
    vlayout->addWidget(m_queryTableWidget);
    vlayout->addStretch(1);
    songWidget->setLayout(vlayout);

    m_queryTableWidget->show();
    m_container->addWidget(songWidget);
}
