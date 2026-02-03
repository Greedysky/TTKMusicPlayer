#include "musicwebdjradioinfowidget.h"
#include "musicdjradioprogramcategoryrequest.h"
#include "musiccoversourcerequest.h"

MusicWebDJRadioInfoWidget::MusicWebDJRadioInfoWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    delete m_statusLabel;
    m_statusLabel = nullptr;

    m_tableWidget = new MusicItemQueryTableWidget(this);
    m_tableWidget->hide();

    MusicAbstractQueryRequest *req = new MusicDJRadioProgramCategoryRequest(this);
    m_tableWidget->setQueryInput(req);

    connect(req, SIGNAL(downloadDataChanged(QString)), SLOT(queryAllFinished()));
    connect(req, SIGNAL(createCategoryItem(MusicResultDataItem)), SLOT(createProgramCategoryItem(MusicResultDataItem)));
}

void MusicWebDJRadioInfoWidget::setCurrentValue(const QString &value)
{
    MusicAbstractItemQueryWidget::setCurrentValue(value);
    m_tableWidget->startToSearchByValue(value);
}

void MusicWebDJRadioInfoWidget::resizeGeometry()
{
    m_tableWidget->resizeGeometry();

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 390;

    Data *data = &m_resizeWidgets[0];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[1];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[2];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[3];
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
}

void MusicWebDJRadioInfoWidget::queryAllFinished()
{
    setSongCountText();
}

void MusicWebDJRadioInfoWidget::createProgramCategoryItem(const MusicResultDataItem &item)
{
    m_currentPlaylistItem = item;

    createLabels();

    if(m_resizeWidgets.isEmpty())
    {
        return;
    }

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *req = new MusicCoverSourceRequest(this);
        connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        req->startToRequest(item.m_coverUrl);
    }

    const int width = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - WINDOW_WIDTH_MIN + 390;

    Data *data = &m_resizeWidgets[0];
    data->m_label->setToolTip(item.m_name);
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[1];
    data->m_label->setToolTip(tr("Singer: %1").arg(item.m_nickName));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[2];
    data->m_label->setToolTip(tr("PlayCount: %1").arg(item.m_count));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));

    data = &m_resizeWidgets[3];
    data->m_label->setToolTip(tr("UpdateTime: %1").arg(item.m_time));
    data->m_label->setText(TTK::Widget::elidedText(data->m_font, data->m_label->toolTip(), Qt::ElideRight, width));
}

void MusicWebDJRadioInfoWidget::createLabels()
{
    createFirstWidget();
    m_container->show();

    layout()->removeWidget(m_mainWidget);
    QScrollArea *scrollArea = new QScrollArea(this);
    TTK::Widget::generateVScrollAreaStyle(scrollArea, m_mainWidget);
    layout()->addWidget(scrollArea);

    QWidget *function = new QWidget(m_mainWidget);
    function->setStyleSheet(TTK::UI::CheckBoxStyle01 + TTK::UI::PushButtonStyle03);
    QVBoxLayout *grid = new QVBoxLayout(function);

    QWidget *firstTopFuncWidget = new QWidget(function);
    QHBoxLayout *firstTopFuncLayout = new QHBoxLayout(firstTopFuncWidget);

    QLabel *firstLabel = new QLabel(function);
    firstLabel->setText(tr("<font color=#158FE1> DJRadio > %1 </font>").arg(m_currentPlaylistItem.m_name));

    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(90, 30);
    backButton->setStyleSheet(TTK::UI::PushButtonStyle03);
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
    nameLabel->setStyleSheet(TTK::UI::FontStyle01);
    nameLabel->setText(TTK_DEFAULT_STR);

    QLabel *singerLabel = new QLabel(topLineWidget);
    singerLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
    singerLabel->setText(TTK_DEFAULT_STR);

    QLabel *playCountLabel = new QLabel(topLineWidget);
    playCountLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);

    QLabel *updateTimeLabel = new QLabel(topLineWidget);
    updateTimeLabel->setStyleSheet(TTK::UI::ColorStyle04 + TTK::UI::FontStyle03);
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
    backButton->setFocusPolicy(Qt::NoFocus);
    m_songButton->setFocusPolicy(Qt::NoFocus);
#endif

    grid->addWidget(functionWidget);
    grid->addWidget(m_container);
    grid->addStretch(1);

    function->setLayout(grid);
    m_mainWidget->layout()->addWidget(function);

    m_resizeWidgets.append({nameLabel, nameLabel->font()});
    m_resizeWidgets.append({singerLabel, singerLabel->font()});
    m_resizeWidgets.append({playCountLabel, playCountLabel->font()});
    m_resizeWidgets.append({updateTimeLabel, updateTimeLabel->font()});
}
