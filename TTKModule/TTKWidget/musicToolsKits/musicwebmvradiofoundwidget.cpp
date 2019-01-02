#include "musicwebmvradiofoundwidget.h"
#include "musicwebmvradiofoundcategorypopwidget.h"
#include "musicwebmvradioinfowidget.h"
#include "musicmvradiocategorythread.h"
#include "musicdownloadsourcethread.h"
#include "musictinyuiobject.h"
#include "musicotherdefine.h"

#define WIDTH_LABEL_SIZE   150
#define HEIGHT_LABEL_SIZE  100
#define LINE_SPACING_SIZE  200

MusicWebMVRadioFoundItemWidget::MusicWebMVRadioFoundItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry((WIDTH_LABEL_SIZE - 30)/2, (HEIGHT_LABEL_SIZE - 30)/2, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(MusicUIObject::MKGTinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(currentItemClicked()));

#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);
    m_nameLabel->setStyleSheet(MusicUIObject::MColorStyle01 + MusicUIObject::MFontStyle05 + MusicUIObject::MFontStyle01);
    m_nameLabel->setText(" - ");
}

MusicWebMVRadioFoundItemWidget::~MusicWebMVRadioFoundItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
}

void MusicWebMVRadioFoundItemWidget::setMusicResultsItem(const MusicResultsItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(MusicUtils::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    if(!item.m_coverUrl.isEmpty() && item.m_coverUrl != COVER_URL_NULL)
    {
        download->startToDownload(item.m_coverUrl);
    }

    m_playButton->hide();
}

void MusicWebMVRadioFoundItemWidget::downLoadFinished(const QByteArray &data)
{
    QPixmap pix;
    pix.loadFromData(data);
    if(!pix.isNull())
    {
        QPixmap cv(":/image/lb_album_cover");
        cv = cv.scaled(m_iconLabel->size());
        pix = pix.scaled(m_iconLabel->size());
        MusicUtils::Widget::fusionPixmap(pix, cv, QPoint(0, 0));
        m_iconLabel->setPixmap(pix);
    }

    m_playButton->raise();
}

void MusicWebMVRadioFoundItemWidget::currentItemClicked()
{
    emit currentItemClicked(m_itemData);
}

void MusicWebMVRadioFoundItemWidget::enterEvent(QEvent *event)
{
    QLabel::enterEvent(event);

    m_playButton->show();
    m_nameLabel->hide();
}

void MusicWebMVRadioFoundItemWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);

    m_playButton->hide();
    m_nameLabel->show();
}



MusicWebMVRadioFoundWidget::MusicWebMVRadioFoundWidget(QWidget *parent)
    : MusicFoundAbstractWidget(parent)
{
    m_container->show();
    layout()->removeWidget(m_mainWindow);
    layout()->addWidget(m_container);
    m_container->addWidget(m_mainWindow);

    m_firstInit = false;
    m_infoWidget = nullptr;
    m_gridLayout = nullptr;
    m_categoryButton = nullptr;

    m_downloadThread = new MusicMVRadioCategoryThread(this);
    connect(m_downloadThread, SIGNAL(createCategoryItem(MusicResultsItem)), SLOT(createCategoryItem(MusicResultsItem)));
}

MusicWebMVRadioFoundWidget::~MusicWebMVRadioFoundWidget()
{
    delete m_infoWidget;
    delete m_gridLayout;
    delete m_categoryButton;
    delete m_downloadThread;
}

void MusicWebMVRadioFoundWidget::setSongName(const QString &name)
{
    MusicFoundAbstractWidget::setSongName(name);
    m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, QString());
}

void MusicWebMVRadioFoundWidget::setSongNameById(const QString &id)
{
    Q_UNUSED(id);
}

void MusicWebMVRadioFoundWidget::resizeWindow()
{
    if(m_infoWidget)
    {
        m_infoWidget->resizeWindow();
    }

    if(!m_resizeWidgets.isEmpty() && m_gridLayout)
    {
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->removeWidget(m_resizeWidgets[i]);
        }

        const int lineNumber = width()/LINE_SPACING_SIZE;
        for(int i=0; i<m_resizeWidgets.count(); ++i)
        {
            m_gridLayout->addWidget(m_resizeWidgets[i], i/lineNumber, i%lineNumber, Qt::AlignCenter);
        }
    }
}

void MusicWebMVRadioFoundWidget::createCategoryItem(const MusicResultsItem &item)
{
    if(!m_firstInit)
    {
        delete m_statusLabel;
        m_statusLabel = nullptr;

        m_container->removeWidget(m_mainWindow);
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet(MusicUIObject::MScrollBarStyle01);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setAlignment(Qt::AlignLeft);
        scrollArea->setWidget(m_mainWindow);
        m_container->addWidget(scrollArea);

        m_firstInit = true;
        QHBoxLayout *mainlayout = MStatic_cast(QHBoxLayout*, m_mainWindow->layout());
        QWidget *containTopWidget = new QWidget(m_mainWindow);
        QHBoxLayout *containTopLayout  = new QHBoxLayout(containTopWidget);
        containTopLayout->setContentsMargins(30, 0, 30, 0);
        m_categoryButton = new MusicWebMVRadioFoundCategoryPopWidget(m_mainWindow);
        m_categoryButton->setCategory(MUSIC_MOVIE_RADIO, this);
        containTopLayout->addWidget(m_categoryButton);
        containTopLayout->addStretch(1);
        containTopWidget->setLayout(containTopLayout);

        QFrame *line = new QFrame(m_mainWindow);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet(MusicUIObject::MColorStyle06);

        QWidget *containWidget = new QWidget(m_mainWindow);
        m_gridLayout = new QGridLayout(containWidget);
        m_gridLayout->setVerticalSpacing(35);
        containWidget->setLayout(m_gridLayout);

        mainlayout->addWidget(containTopWidget);
        mainlayout->addWidget(line);
        mainlayout->addWidget(containWidget);

        mainlayout->addStretch(1);
    }

    MusicWebMVRadioFoundItemWidget *label = new MusicWebMVRadioFoundItemWidget(this);
    connect(label, SIGNAL(currentItemClicked(MusicResultsItem)), SLOT(currentRadioClicked(MusicResultsItem)));
    label->setMusicResultsItem(item);

    const int lineNumber = width()/LINE_SPACING_SIZE;
    m_gridLayout->addWidget(label, m_resizeWidgets.count()/lineNumber, m_resizeWidgets.count()%lineNumber, Qt::AlignCenter);
    m_resizeWidgets << label;
}

void MusicWebMVRadioFoundWidget::currentRadioClicked(const MusicResultsItem &item)
{
    delete m_infoWidget;
    m_infoWidget = new MusicWebMVRadioInfoWidget(this);
    connect(m_infoWidget, SIGNAL(backToMainMenu()), SLOT(backToMainMenuClicked()));
    m_infoWidget->setSongName(item.m_id);

    m_container->addWidget(m_infoWidget);
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_1);
}

void MusicWebMVRadioFoundWidget::backToMainMenuClicked()
{
    m_container->setCurrentIndex(PLAYLIST_WINDOW_INDEX_0);
}

void MusicWebMVRadioFoundWidget::categoryChanged(const MusicResultsCategoryItem &category)
{
    if(m_categoryButton)
    {
        m_songNameFull.clear();
        m_categoryButton->setText(category.m_name);
        m_categoryButton->closeMenu();

        while(!m_resizeWidgets.isEmpty())
        {
            QWidget *w = m_resizeWidgets.takeLast();
            m_gridLayout->removeWidget(w);
            delete w;
        }
        m_downloadThread->startToSearch(MusicDownLoadQueryThreadAbstract::OtherQuery, category.m_id);
    }
}
