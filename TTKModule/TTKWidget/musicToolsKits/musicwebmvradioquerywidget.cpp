#include "musicwebmvradioquerywidget.h"
#include "musicwebmvradioquerycategorypopwidget.h"
#include "musicwebmvradioinfowidget.h"
#include "musicmvradiocategoryrequest.h"
#include "musicdownloadcoverrequest.h"
#include "musictinyuiobject.h"

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  100
#define LINE_SPACING_SIZE  200

MusicWebMVRadioQueryItemWidget::MusicWebMVRadioQueryItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry((WIDTH_LABEL_SIZE - 30) / 2, (HEIGHT_LABEL_SIZE - 30) / 2, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MQSSTinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);
    m_nameLabel->setStyleSheet(MusicUIObject::MQSSColorStyle01 + MusicUIObject::MQSSFontStyle04 + MusicUIObject::MQSSFontStyle01);
    m_nameLabel->setText(" - ");
}

MusicWebMVRadioQueryItemWidget::~MusicWebMVRadioQueryItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
}

void MusicWebMVRadioQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != TTK_NULL_STR)
    {
        MusicDownloadCoverRequest *d = new MusicDownloadCoverRequest(this);
        connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
        d->startRequest(item.m_coverUrl);
    }

    m_playButton->hide();
}

void MusicWebMVRadioQueryItemWidget::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicImageRenderer *render = new MusicImageRenderer(sender());
    connect(render, SIGNAL(renderFinished(QPixmap)), SLOT(renderFinished(QPixmap)));
    render->setInputData(bytes, m_iconLabel->size());
    render->start();
}

void MusicWebMVRadioQueryItemWidget::renderFinished(const QPixmap &data)
{
    m_iconLabel->setPixmap(data);
    m_playButton->raise();
}

void MusicWebMVRadioQueryItemWidget::currentItemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}

void MusicWebMVRadioQueryItemWidget::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);

    m_playButton->show();
    m_nameLabel->hide();
}

void MusicWebMVRadioQueryItemWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);

    m_playButton->hide();
    m_nameLabel->show();
}



MusicWebMVRadioQueryWidget::MusicWebMVRadioQueryWidget(QWidget *parent)
    : MusicAbstractItemQueryWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_initialized = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_categoryButton = nullptr;

    m_networkRequest = new MusicMVRadioCategoryRequest(this);
    connect(m_networkRequest, SIGNAL(createMVRadioItem(MusicResultDataItem)), SLOT(createMVRadioCategoryItem(MusicResultDataItem)));
}

MusicWebMVRadioQueryWidget::~MusicWebMVRadioQueryWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_categoryButton;
}

void MusicWebMVRadioQueryWidget::setSongName(const QString &name)
{
    MusicAbstractItemQueryWidget::setSongName(name);
    m_networkRequest->startToSearch(MusicAbstractQueryRequest::QueryType::Other, QString());
}

void MusicWebMVRadioQueryWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicWebMVRadioQueryWidget::resizeWidget()
{
    if(m_infoWidget)
    {
        m_infoWidget->resizeWidget();
    }

    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i].m_label);
        }

        const int lineNumber = width() / LINE_SPACING_SIZE;
        for(int i = 0; i < m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i].m_label, i / lineNumber, i % lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicWebMVRadioQueryWidget::createMVRadioCategoryItem(const MusicResultDataItem &item)
{
    if(!m_initialized)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        MusicUtils::Widget::generateVScrollAreaFormat(scrollArea, m_mainWindow);
        m_container->addWidget(scrollArea);

        m_initialized = true;
        QVBoxLayout *mainlayout = TTKObject_cast(QVBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QHBoxLayout *containTopLayout  = new QHBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);
        m_categoryButton = new MusicWebMVRadioQueryCategoryPopWidget(m_mainWindow);
        m_categoryButton->setCategory(MUSIC_MOVIE_RADIO, this);
        containTopLayout->addWidget(m_categoryButton);
        containTopLayout->addStretch(1);
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(MusicUIObject::MQSSColorStyle06);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(35);
        containWidget->setLayout(m_gridLayout);

        mainlayout->addWidget(containTopWidget);
        mainlayout->addWidget(line);
        mainlayout->addWidget(containWidget);

        mainlayout->addStretch(1);
    }

    MusicWebMVRadioQueryItemWidget *label = new MusicWebMVRadioQueryItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultDataItem)), SLOT(currentRadioClicked(MusicResultDataItem)));
    label->setResultDataItem(item);

    const int lineNumber = width() / LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count() / lineNumber, m_resizeWidgets.count() % lineNumber, Qt::AlignCenter);

    m_resizeWidgets.push_back({label, label->font()});
}

void MusicWebMVRadioQueryWidget::currentRadioClicked(const MusicResultDataItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebMVRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setSongName(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(1);
}

void MusicWebMVRadioQueryWidget::backToMainMenuClicked()
{
    m_container->setCurrentIndex(0);
}

void MusicWebMVRadioQueryWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_songNameFull.clear();
        m_categoryButton->setText(category.m_value);
        m_categoryButton->closeMenu();

        while(!m_resizeWidgets.isEmpty())
        {
            QWidget *w = m_resizeWidgets.takeLast().m_label;
            m_gridLayout->removeWidget(w);
            delete w;
        }
        m_networkRequest->startToSearch(MusicAbstractQueryRequest::QueryType::Other, category.m_key);
    }
}
