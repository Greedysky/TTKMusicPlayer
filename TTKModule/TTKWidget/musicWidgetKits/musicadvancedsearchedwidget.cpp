#include "musicadvancedsearchedwidget.h"
#include "musicabstractqueryrequest.h"
#include "musicitemsearchedit.h"
#include "musicsettingmanager.h"
#include "musicrightareawidget.h"
#include "musicsongsharingwidget.h"
#include "musictoastlabel.h"
#include "musicwidgetheaders.h"

MusicAdvancedSearchedWidget::MusicAdvancedSearchedWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(MusicUIObject::MQSSBackgroundStyle12);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);

    m_tabWidget = new QTabWidget(mainWidget);
    m_tabWidget->setFixedHeight(300);
    m_tabWidget->setStyleSheet(MusicUIObject::MQSSTabWidgetStyle01 + "QTabBar::tab{ width:120px; }");
    //
    QWidget *songWidget = new QWidget(m_tabWidget);
    QVBoxLayout *songWidgetLayout = new QVBoxLayout(songWidget);
    QLabel *songLabel = new QLabel(tr("Input song id or url link"), songWidget);
    songLabel->setStyleSheet(MusicUIObject::MQSSFontStyle04);
    m_songEdit = new MusicItemSearchEdit(songWidget);
    m_songEdit->setFixedHeight(33);
    songWidgetLayout->setSpacing(10);
    songWidgetLayout->addStretch(1);
    songWidgetLayout->addWidget(songLabel);
    songWidgetLayout->addWidget(m_songEdit);
    songWidgetLayout->addStretch(1);
    songWidget->setLayout(songWidgetLayout);
    m_tabWidget->addTab(songWidget, tr("Song"));
    //
    QWidget *artistWidget = new QWidget(m_tabWidget);
    QVBoxLayout *artistWidgetLayout = new QVBoxLayout(artistWidget);
    QLabel *artistLabel = new QLabel(tr("Input artist id or url link"), artistWidget);
    artistLabel->setStyleSheet(MusicUIObject::MQSSFontStyle04);
    m_artistEdit = new MusicItemSearchEdit(songWidget);
    m_artistEdit->setFixedHeight(33);
    artistWidgetLayout->setSpacing(10);
    artistWidgetLayout->addStretch(1);
    artistWidgetLayout->addWidget(artistLabel);
    artistWidgetLayout->addWidget(m_artistEdit);
    artistWidgetLayout->addStretch(1);
    artistWidget->setLayout(artistWidgetLayout);
    m_tabWidget->addTab(artistWidget, tr("Artist"));
    //
    QWidget *albumWidget = new QWidget(m_tabWidget);
    QVBoxLayout *albumWidgetLayout = new QVBoxLayout(albumWidget);
    QLabel *albumLabel = new QLabel(tr("Input album id or url link"), albumWidget);
    albumLabel->setStyleSheet(MusicUIObject::MQSSFontStyle04);
    m_albumEdit = new MusicItemSearchEdit(songWidget);
    m_albumEdit->setFixedHeight(33);
    albumWidgetLayout->setSpacing(10);
    albumWidgetLayout->addStretch(1);
    albumWidgetLayout->addWidget(albumLabel);
    albumWidgetLayout->addWidget(m_albumEdit);
    albumWidgetLayout->addStretch(1);
    albumWidget->setLayout(albumWidgetLayout);
    m_tabWidget->addTab(albumWidget, tr("Album"));
    //
    QWidget *playlistWidget = new QWidget(m_tabWidget);
    QVBoxLayout *playlistWidgetLayout = new QVBoxLayout(playlistWidget);
    QLabel *playlistLabel = new QLabel(tr("Input playlist id or url link"), playlistWidget);
    playlistLabel->setStyleSheet(MusicUIObject::MQSSFontStyle04);
    m_playlistEdit = new MusicItemSearchEdit(songWidget);
    m_playlistEdit->setFixedHeight(33);
    playlistWidgetLayout->setSpacing(10);
    playlistWidgetLayout->addStretch(1);
    playlistWidgetLayout->addWidget(playlistLabel);
    playlistWidgetLayout->addWidget(m_playlistEdit);
    playlistWidgetLayout->addStretch(1);
    playlistWidget->setLayout(playlistWidgetLayout);
    m_tabWidget->addTab(playlistWidget, tr("Playlist"));
    //
    QWidget *movieWidget = new QWidget(m_tabWidget);
    QVBoxLayout *movieWidgetLayout = new QVBoxLayout(movieWidget);
    QLabel *movieLabel = new QLabel(tr("Input movie id or url link"), movieWidget);
    movieLabel->setStyleSheet(MusicUIObject::MQSSFontStyle04);
    m_movieEdit = new MusicItemSearchEdit(songWidget);
    m_movieEdit->setFixedHeight(33);
    movieWidgetLayout->setSpacing(10);
    movieWidgetLayout->addStretch(1);
    movieWidgetLayout->addWidget(movieLabel);
    movieWidgetLayout->addWidget(m_movieEdit);
    movieWidgetLayout->addStretch(1);
    movieWidget->setLayout(movieWidgetLayout);
    m_tabWidget->addTab(movieWidget, tr("Movie"));
    //
    mainLayout->addStretch(1);
    mainLayout->addWidget(m_tabWidget);
    QPushButton *button = new QPushButton(tr("Search"), mainWidget);
    button->setFixedSize(90, 30);
    button->setStyleSheet(MusicUIObject::MQSSPushButtonStyle03);
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

void MusicAdvancedSearchedWidget::searchButtonClicked()
{
    const int server = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    if(server > 5 || server < 0)
    {
        MusicToastLabel::popup(tr("Current server is not support search!"));
        return;
    }

    switch(m_tabWidget->currentIndex())
    {
        case 0:
        {
            QString key = !m_songEdit->editor()->text().isEmpty() ? m_songEdit->editor()->text() : m_songEdit->editor()->placeholderText();
                    key = searchedKeyWork(0, key);
            MusicRightAreaWidget::instance()->musicSingleSearchedFound(key);
            break;
        }
        case 1:
        {
            QString key = !m_artistEdit->editor()->text().isEmpty() ? m_artistEdit->editor()->text() : m_artistEdit->editor()->placeholderText();
                    key = searchedKeyWork(1, key);
            MusicRightAreaWidget::instance()->musicArtistFound(QString(), key);
            break;
        }
        case 2:
        {
            QString key = !m_albumEdit->editor()->text().isEmpty() ? m_albumEdit->editor()->text() : m_albumEdit->editor()->placeholderText();
                    key = searchedKeyWork(2, key);
            MusicRightAreaWidget::instance()->musicAlbumFound(QString(), key);
            break;
        }
        case 3:
        {
            QString key = !m_playlistEdit->editor()->text().isEmpty() ? m_playlistEdit->editor()->text() : m_playlistEdit->editor()->placeholderText();
                    key = searchedKeyWork(3, key);
            MusicRightAreaWidget::instance()->musicPlaylistFound(key);
            break;
        }
        case 4:
        {
            QString key = !m_movieEdit->editor()->text().isEmpty() ? m_movieEdit->editor()->text() : m_movieEdit->editor()->placeholderText();
                    key = searchedKeyWork(4, key);
            MusicRightAreaWidget::instance()->musicVideoButtonSearched(QString(), key);
            break;
        }
        default: break;
    }
}

void MusicAdvancedSearchedWidget::updateServerPlaceholderText()
{
    switch(TTKStatic_cast(DownloadQueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
    {
        case WYQueryServer:
        {
            m_songEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_SG_SHARE, ALG_UNIMP_KEY, false).arg("28830412"));
            m_artistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_AR_SHARE, ALG_UNIMP_KEY, false).arg("964486"));
            m_albumEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_AL_SHARE, ALG_UNIMP_KEY, false).arg("2901140"));
            m_playlistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_PL_SHARE, ALG_UNIMP_KEY, false).arg("988604820"));
            m_movieEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_MV_SHARE, ALG_UNIMP_KEY, false).arg("5343487"));
            break;
        }
        case QQQueryServer:
        {
            m_songEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_SG_SHARE, ALG_UNIMP_KEY, false).arg("004TXEXY2G2c7C"));
            m_artistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_AR_SHARE, ALG_UNIMP_KEY, false).arg("0029gk1t3wpGWU"));
            m_albumEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_AL_SHARE, ALG_UNIMP_KEY, false).arg("003fk65m1ZaCG2"));
            m_playlistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_PL_SHARE, ALG_UNIMP_KEY, false).arg("889413740"));
            m_movieEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_MV_SHARE, ALG_UNIMP_KEY, false).arg("o0032ayz7c2"));
            break;
        }
        case KWQueryServer:
        {
            m_songEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_SG_SHARE, ALG_UNIMP_KEY, false).arg("23983067"));
            m_artistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_AR_SHARE, ALG_UNIMP_KEY, false).arg("373"));
            m_albumEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_AL_SHARE, ALG_UNIMP_KEY, false).arg("2698608"));
            m_playlistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_PL_SHARE, ALG_UNIMP_KEY, false).arg("236350390"));
            m_movieEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_MV_SHARE, ALG_UNIMP_KEY, false).arg("9812716"));
            break;
        }
        case KGQueryServer:
        {
            m_songEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_SG_SHARE, ALG_UNIMP_KEY, false).arg("b2bc0eb8553b0eb70b66b950fc3ad287"));
            m_artistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_AR_SHARE, ALG_UNIMP_KEY, false).arg("832532"));
            m_albumEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_AL_SHARE, ALG_UNIMP_KEY, false).arg("41169925"));
            m_playlistEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_PL_SHARE, ALG_UNIMP_KEY, false).arg("155318"));
            m_movieEdit->editor()->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_MV_SHARE, ALG_UNIMP_KEY, false).arg("630600"));
            break;
        }
        default: break;
    }
}

QString MusicAdvancedSearchedWidget::searchedKeyWork(int type, const QString &url)
{
    QString key;
    switch(TTKStatic_cast(DownloadQueryServer, G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt()))
    {
        case WYQueryServer:
        {
            QRegExp regx("id=(\\d+)");
            key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
            break;
        }
        case QQQueryServer:
        {
            QRegExp regx("/(\\w+).html");
            key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
            break;
        }
        case KWQueryServer:
        {
            QRegExp regx;
            if(type == 1)
            {
                regx.setPattern("id=(\\d+)");
            }
            else if(type == 3)
            {
                regx.setPattern("pid=(\\d+)");
            }
            else
            {
                regx.setPattern("/(\\d+)");
            }
            key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
            break;
        }
        case KGQueryServer:
        {
            QRegExp regx;
            if(type == 0)
            {
                regx.setPattern("hash=(\\w+)");
            }
            else if(type == 4)
            {
                regx.setPattern("/mv_(\\d+)");
            }
            else
            {
                regx.setPattern("/(\\d+)");
            }
            key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
            break;
        }
        default: break;
    }
    return key;
}
