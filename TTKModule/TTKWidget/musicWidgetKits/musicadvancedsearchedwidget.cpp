#include "musicadvancedsearchedwidget.h"
#include "musiclocalsongsearchedit.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicrightareawidget.h"

#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>
MusicAdvancedSearchedWidget::MusicAdvancedSearchedWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MBackgroundStyle17);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);

    m_tabWidget = new QTabWidget(mainWidget);
    m_tabWidget->setFixedHeight(300);
    m_tabWidget->setStyleSheet(MusicUIObject::MTabWidgetStyle01);
    ////////////////////////////////////////////////////////////////////////////
    QWidget *songWidget = new QWidget(m_tabWidget);
    QVBoxLayout *songWidgetLayout = new QVBoxLayout(songWidget);
    QLabel *songLabel = new QLabel(tr("Input Song ID Or Url Link"), songWidget);
    songWidgetLayout->setSpacing(10);
    songWidgetLayout->addStretch(1);
    songWidgetLayout->addWidget(songLabel);
    songWidgetLayout->addWidget( createSearchPairWidget(&m_songEdit) );
    songWidgetLayout->addStretch(1);
    songWidget->setLayout(songWidgetLayout);
    m_tabWidget->addTab(songWidget, tr("Song"));
    ////////////////////////////////////////////////////////////////////////////
    QWidget *artistWidget = new QWidget(m_tabWidget);
    QVBoxLayout *artistWidgetLayout = new QVBoxLayout(artistWidget);
    QLabel *artistLabel = new QLabel(tr("Input Artist ID Or Url Link"), artistWidget);
    artistWidgetLayout->setSpacing(10);
    artistWidgetLayout->addStretch(1);
    artistWidgetLayout->addWidget(artistLabel);
    artistWidgetLayout->addWidget( createSearchPairWidget(&m_artistEdit) );
    artistWidgetLayout->addStretch(1);
    artistWidget->setLayout(artistWidgetLayout);
    m_tabWidget->addTab(artistWidget, tr("Artist"));
    ////////////////////////////////////////////////////////////////////////////
    QWidget *albumWidget = new QWidget(m_tabWidget);
    QVBoxLayout *albumWidgetLayout = new QVBoxLayout(albumWidget);
    QLabel *albumLabel = new QLabel(tr("Input Album ID Or Url Link"), albumWidget);
    albumWidgetLayout->setSpacing(10);
    albumWidgetLayout->addStretch(1);
    albumWidgetLayout->addWidget(albumLabel);
    albumWidgetLayout->addWidget( createSearchPairWidget(&m_albumEdit) );
    albumWidgetLayout->addStretch(1);
    albumWidget->setLayout(albumWidgetLayout);
    m_tabWidget->addTab(albumWidget, tr("Album"));
    ////////////////////////////////////////////////////////////////////////////
    QWidget *playlistWidget = new QWidget(m_tabWidget);
    QVBoxLayout *playlistWidgetLayout = new QVBoxLayout(playlistWidget);
    QLabel *playlistLabel = new QLabel(tr("Input Playlist ID Or Url Link"), playlistWidget);
    playlistWidgetLayout->setSpacing(10);
    playlistWidgetLayout->addStretch(1);
    playlistWidgetLayout->addWidget(playlistLabel);
    playlistWidgetLayout->addWidget( createSearchPairWidget(&m_playlistEdit) );
    playlistWidgetLayout->addStretch(1);
    playlistWidget->setLayout(playlistWidgetLayout);
    m_tabWidget->addTab(playlistWidget, tr("Playlist"));
    ////////////////////////////////////////////////////////////////////////////
    mainLayout->addStretch(1);
    mainLayout->addWidget(m_tabWidget);
    QPushButton *button = new QPushButton(tr("Search"), mainWidget);
    button->setFixedSize(90, 30);
    button->setStyleSheet(MusicUIObject::MPushButtonStyle03);
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button, SIGNAL(clicked()), SLOT(searchButtonClicked()));
#ifdef Q_OS_UNIX
    button->setFocusPolicy(Qt::NoFocus);
#endif
    mainLayout->addWidget(button, 0, Qt::AlignCenter);
    mainLayout->addStretch(1);

    updateServerPlaceholderText();

}

MusicAdvancedSearchedWidget::~MusicAdvancedSearchedWidget()
{
    delete m_tabWidget;
}

QString MusicAdvancedSearchedWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicAdvancedSearchedWidget::searchButtonClicked()
{
    switch(m_tabWidget->currentIndex())
    {
        case 0: break;
        case 1:
            {
                QRegExp regx("id=(\\d+)");
                int pos = m_albumEdit->text().indexOf(regx);
                QString key;
                if(pos != -1)
                {
                    key = regx.cap(1);
                }
                MusicRightAreaWidget::instance()->musicArtistFound(QString(), key);
                break;
            }
        case 2:
            {
                QRegExp regx("id=(\\d+)");
                int pos = m_albumEdit->text().indexOf(regx);
                QString key;
                if(pos != -1)
                {
                    key = regx.cap(1);
                }
                MusicRightAreaWidget::instance()->musicAlbumFound(QString(), key);
                break;
            }
        case 3:
            {
                QRegExp regx("id=(\\d+)");
                int pos = m_playlistEdit->text().indexOf(regx);
                QString key;
                if(pos != -1)
                {
                    key = regx.cap(1);
                }
                MusicRightAreaWidget::instance()->musicPlaylistFound(key);
                break;
            }
        default: break;
    }
}

QWidget *MusicAdvancedSearchedWidget::createSearchPairWidget(MusicLocalSongSearchEdit **input)
{
    QWidget *searchPair = new QWidget(this);
    searchPair->setFixedHeight(33);
    searchPair->setStyleSheet(MusicUIObject::MBorderStyle02);
    QHBoxLayout *searchPairLayout = new QHBoxLayout(searchPair);
    searchPairLayout->setContentsMargins(5, 2, 5, 0);
    QLabel *showIcon = new QLabel(searchPair);
    showIcon->setFixedSize(16, 16);
    showIcon->setStyleSheet(MusicUIObject::MBackgroundStyle01 +
                            "border-image: url(:/tiny/btn_search_main_hover);");
    *input = new MusicLocalSongSearchEdit(searchPair);
    searchPairLayout->addWidget(showIcon);
    searchPairLayout->addWidget(*input);
    searchPair->setLayout(searchPairLayout);

    return searchPair;
}

void MusicAdvancedSearchedWidget::updateServerPlaceholderText()
{
    int downloadServer = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
}
