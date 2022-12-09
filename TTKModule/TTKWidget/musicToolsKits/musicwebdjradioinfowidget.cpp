#include "musicwebdjradioinfowidget.h"
#include "musicdjradioprogramcategoryrequest.h"
#include "musicsettingmanager.h"
#include "musicdownloadcoverrequest.h"

#include <qmath.h>

MusicWebDJRadioInfoTableWidget::MusicWebDJRadioInfoTableWidget(QWidget *parent)
    : MusicItemQueryTableWidget(parent)
{

}

MusicWebDJRadioInfoTableWidget::~MusicWebDJRadioInfoTableWidget()
{
    removeItems();
}

void MusicWebDJRadioInfoTableWidget::setQueryInput(MusicAbstractQueryRequest *query)
{
    MusicItemQueryTableWidget::setQueryInput(query);
    if(parent()->metaObject()->indexOfSlot("queryAllFinished()") != -1)
    {
        connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), parent(), SLOT(queryAllFinished()));
    }
}



MusicWebDJRadioInfoWidget::MusicWebDJRadioInfoWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_queryTableWidget = new MusicWebDJRadioInfoTableWidget(this);
    m_queryTableWidget->hide();

    MusicAbstractQueryRequest *d = new MusicDJRadioProgramCategoryRequest(this);
    m_queryTableWidget->setQueryInput(d);
    connect(d, SIGNAL(createCategoryItem(MusicResultDataItem)), SLOT(createProgramCategoryItem(MusicResultDataItem)));
}

void MusicWebDJRadioInfoWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_queryTableWidget->startSearchQuery(name);
}

void MusicWebDJRadioInfoWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicWebDJRadioInfoWidget::resizeWidget()
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

        data = &m_resizeWidgets[2];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[3];
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
    }
}

void MusicWebDJRadioInfoWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicWebDJRadioInfoWidget::createProgramCategoryItem(const MusicResultDataItem &item)
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
        data->m_label->setToolTip(tr("Singer: %1").arg(item.m_nickName));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[2];
        data->m_label->setToolTip(tr("PlayCount: %1").arg(item.m_playCount));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

        data = &m_resizeWidgets[3];
        data->m_label->setToolTip(tr("UpdateTime: %1").arg(item.m_updateTime));
        data->m_label->setText(MusicUtils::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
    }
}

void MusicWebDJRadioInfoWidget::createLabels()
{
    initFirstWidget();
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
    firstLabel->setText(tr("<font color=#158FE1> DJRadio > %1 </font>").arg(m_currentPlaylistItem.m_name));
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
    QLabel *singerLabel = new QLabel(topLineWidget);
    singerLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    singerLabel->setText(TTK_DEFAULT_STR);
    QLabel *playCountLabel = new QLabel(topLineWidget);
    playCountLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    QLabel *updateTimeLabel = new QLabel(topLineWidget);
    updateTimeLabel->setStyleSheet(MusicUIObject::MQSSColorStyle04 + MusicUIObject::MQSSFontStyle03);
    updateTimeLabel->setText(TTK_DEFAULT_STR);

    topLineLayout->addWidget(nameLabel);
    topLineLayout->addWidget(singerLabel);
    topLineLayout->addWidget(playCountLabel);
    topLineLayout->addWidget(updateTimeLabel);
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
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWindow->layout()->addWidget(function);

    m_resizeWidgets.push_back({nameLabel, nameLabel->font()});
    m_resizeWidgets.push_back({singerLabel, singerLabel->font()});
    m_resizeWidgets.push_back({playCountLabel, playCountLabel->font()});
    m_resizeWidgets.push_back({updateTimeLabel, updateTimeLabel->font()});
}
