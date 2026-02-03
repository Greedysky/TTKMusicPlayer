#include "musictoplistquerywidget.h"
#include "musicdownloadqueryfactory.h"
#include "musiccoverrequest.h"
#include "musictoplistquerycategorypopwidget.h"

MusicToplistQueryWidget::MusicToplistQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent),
      m_categoryButton(nullptr)
{
    m_tableWidget = new MusicItemQueryTableWidget(this);
    m_tableWidget->hide();

    MusicAbstractQueryRequest *req = G_DOWNLOAD_QUERY_PTR->makeToplistRequest(this);
    m_tableWidget->setQueryInput(req);

    connect(req, SIGNAL(downloadDataChanged(QString)), SLOT(queryAllFinished()));
    connect(req, SIGNAL(createToplistItem(MusicResultDataItem)), SLOT(createToplistItem(MusicResultDataItem)));
}

MusicToplistQueryWidget::~MusicToplistQueryWidget()
{
    delete m_categoryButton;
}

void MusicToplistQueryWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_tableWidget->queryInput()->startToSearch({});
    createLabels();
}

void MusicToplistQueryWidget::resizeGeometry()
{
    m_tableWidget->resizeGeometry();

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 410;

    Data *data = &m_resizeWidgets[0];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width - 20));

    data = &m_resizeWidgets[1];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[2];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[3];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width + 410 * 10));
}

void MusicToplistQueryWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicToplistQueryWidget::createLabels()
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    createFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWidget);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWidget);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> Rank </font>"));
    grid->addWidget(firstLabel);
    //
    QWidget *categoryWidget = new QWidget(function);
    QHBoxLayout *categoryWidgetLayout = new QHBoxLayout(categoryWidget);

    m_categoryButton = new MusicToplistQueryCategoryPopWidget(categoryWidget);
    m_categoryButton->setCategory(m_tableWidget->queryInput()->queryServer(), this);

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
    nameLabel->setStyleSheet(TTK::UI::FontStyle01);

    QLabel *playCountLabel = new QLabel(topLineWidget);
    playCountLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    QLabel *updateTimeLabel = new QLabel(topLineWidget);
    updateTimeLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    QLabel *descriptionLabel = new QLabel(topLineWidget);
    descriptionLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    descriptionLabel->setWordWrap(true);

    topLineLayout->addWidget(nameLabel);
    topLineLayout->addWidget(playCountLabel);
    topLineLayout->addWidget(updateTimeLabel);
    topLineLayout->addWidget(descriptionLabel);
    topLineWidget->setLayout(topLineLayout);

    topFuncLayout->addWidget(m_iconLabel);
    topFuncLayout->addWidget(topLineWidget);
    topFuncWidget->setLayout(topFuncLayout);
    grid->addWidget(topFuncWidget);

    QWidget *functionWidget = new QWidget(this);
    functionWidget->setStyleSheet(TTK::UI::PushButtonStyle03);
    QHBoxLayout *hLayout = new QHBoxLayout(functionWidget);

    m_songButton = new QPushButton(functionWidget);
    m_songButton->setText(tr("SongItems"));
    m_songButton->setFixedSize(100, 25);
    m_songButton->setCursor(QCursor(Qt::PointingHandCursor));

    hLayout->addWidget(m_songButton);
    hLayout->addStretch(1);
    functionWidget->setLayout(hLayout);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_songButton, 0);
    QtButtonGroupConnect(buttonGroup, m_container, setCurrentIndex, TTK_SLOT);

#ifdef Q_OS_UNIX
    m_songButton->setFocusPolicy(Qt::NoFocus);
#endif
    grid->addWidget(functionWidget);
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWidget->layout()->addWidget(function);

    m_resizeWidgets.append({nameLabel, nameLabel->font()});
    m_resizeWidgets.append({playCountLabel, playCountLabel->font()});
    m_resizeWidgets.append({updateTimeLabel, updateTimeLabel->font()});
    m_resizeWidgets.append({descriptionLabel, descriptionLabel->font()});
}

void MusicToplistQueryWidget::createToplistItem(const MusicResultDataItem &item)
{
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != TTK_NULL_STR)
    {
        MusicCoverRequest *req = G_DOWNLOAD_QUERY_PTR->makeCoverRequest(this);
        connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        req->startToRequest(item.m_coverUrl);
    }

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 410;

    Data *data = &m_resizeWidgets[0];
    data->m_label->setToolTip(item.m_name);
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width - 20));

    data = &m_resizeWidgets[1];
    data->m_label->setToolTip(tr("PlayCount: %1").arg(item.m_count));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[2];
    data->m_label->setToolTip(tr("UpdateTime: %1").arg(item.m_time));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[3];
    data->m_label->setToolTip(tr("Description: %1").arg(item.m_description));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width + 410 * 10));
}

void MusicToplistQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_categoryButton->setToolTip(category.m_value);
        m_categoryButton->setText(TTK::Widget::elidedText(m_categoryButton->font(), category.m_value, Qt::ElideRight, m_categoryButton->width() - 20));
        m_categoryButton->closeMenu();

        m_songButton->setText(tr("SongItems"));
        m_tableWidget->queryInput()->startToSearch(category.m_key);
    }
}
