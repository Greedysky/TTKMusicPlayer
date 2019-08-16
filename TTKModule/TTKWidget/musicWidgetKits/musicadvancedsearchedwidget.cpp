#include "musicadvancedsearchedwidget.h"
#include "musiclocalsongsearchedit.h"
#include "musicsettingmanager.h"
#include "musicuiobject.h"
#include "musicrightareawidget.h"
#include "musicsongsharingwidget.h"
#include "musicalgorithmutils.h"
#include "musicmessagebox.h"

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
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainWidget->setLayout(mainLayout);
    layout->addWidget(mainWidget);
    setLayout(layout);

    m_tabWidget = new QTabWidget(mainWidget);
    m_tabWidget->setFixedHeight(300);
    m_tabWidget->setStyleSheet(MusicUIObject::MTabWidgetStyle01 + "QTabBar::tab{ width:120px;}");
    //
    QWidget *songWidget = new QWidget(m_tabWidget);
    QVBoxLayout *songWidgetLayout = new QVBoxLayout(songWidget);
    QLabel *songLabel = new QLabel(tr("Input Song ID Or Url Link"), songWidget);
    songLabel->setStyleSheet(MusicUIObject::MFontStyle05);
    songWidgetLayout->setSpacing(10);
    songWidgetLayout->addStretch(1);
    songWidgetLayout->addWidget(songLabel);
    songWidgetLayout->addWidget( createSearchPairWidget(&m_songEdit) );
    songWidgetLayout->addStretch(1);
    songWidget->setLayout(songWidgetLayout);
    m_tabWidget->addTab(songWidget, tr("Song"));
    //
    QWidget *artistWidget = new QWidget(m_tabWidget);
    QVBoxLayout *artistWidgetLayout = new QVBoxLayout(artistWidget);
    QLabel *artistLabel = new QLabel(tr("Input Artist ID Or Url Link"), artistWidget);
    artistLabel->setStyleSheet(MusicUIObject::MFontStyle05);
    artistWidgetLayout->setSpacing(10);
    artistWidgetLayout->addStretch(1);
    artistWidgetLayout->addWidget(artistLabel);
    artistWidgetLayout->addWidget( createSearchPairWidget(&m_artistEdit) );
    artistWidgetLayout->addStretch(1);
    artistWidget->setLayout(artistWidgetLayout);
    m_tabWidget->addTab(artistWidget, tr("Artist"));
    //
    QWidget *albumWidget = new QWidget(m_tabWidget);
    QVBoxLayout *albumWidgetLayout = new QVBoxLayout(albumWidget);
    QLabel *albumLabel = new QLabel(tr("Input Album ID Or Url Link"), albumWidget);
    albumLabel->setStyleSheet(MusicUIObject::MFontStyle05);
    albumWidgetLayout->setSpacing(10);
    albumWidgetLayout->addStretch(1);
    albumWidgetLayout->addWidget(albumLabel);
    albumWidgetLayout->addWidget( createSearchPairWidget(&m_albumEdit) );
    albumWidgetLayout->addStretch(1);
    albumWidget->setLayout(albumWidgetLayout);
    m_tabWidget->addTab(albumWidget, tr("Album"));
    //
    QWidget *playlistWidget = new QWidget(m_tabWidget);
    QVBoxLayout *playlistWidgetLayout = new QVBoxLayout(playlistWidget);
    QLabel *playlistLabel = new QLabel(tr("Input Playlist ID Or Url Link"), playlistWidget);
    playlistLabel->setStyleSheet(MusicUIObject::MFontStyle05);
    playlistWidgetLayout->setSpacing(10);
    playlistWidgetLayout->addStretch(1);
    playlistWidgetLayout->addWidget(playlistLabel);
    playlistWidgetLayout->addWidget( createSearchPairWidget(&m_playlistEdit) );
    playlistWidgetLayout->addStretch(1);
    playlistWidget->setLayout(playlistWidgetLayout);
    m_tabWidget->addTab(playlistWidget, tr("Playlist"));
    //
    QWidget *movieWidget = new QWidget(m_tabWidget);
    QVBoxLayout *movieWidgetLayout = new QVBoxLayout(movieWidget);
    QLabel *movieLabel = new QLabel(tr("Input Movie ID Or Url Link"), movieWidget);
    movieLabel->setStyleSheet(MusicUIObject::MFontStyle05);
    movieWidgetLayout->setSpacing(10);
    movieWidgetLayout->addStretch(1);
    movieWidgetLayout->addWidget(movieLabel);
    movieWidgetLayout->addWidget( createSearchPairWidget(&m_movieEdit) );
    movieWidgetLayout->addStretch(1);
    movieWidget->setLayout(movieWidgetLayout);
    m_tabWidget->addTab(movieWidget, tr("Movie"));
    //
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

void MusicAdvancedSearchedWidget::searchButtonClicked()
{
    const int server = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    if(server > 5 || server < 0)
    {
        MusicMessageBox message;
        message.setText(tr("Current server not support search!"));
        message.exec();
        return;
    }

    switch(m_tabWidget->currentIndex())
    {
        case 0:
            {
                QString key = !m_songEdit->text().isEmpty() ? m_songEdit->text() : m_songEdit->placeholderText();
                        key = getSearchedKeyWork(0, key);
                MusicRightAreaWidget::instance()->musicSingleSearchedFound(key);
                break;
            }
        case 1:
            {
                QString key = !m_artistEdit->text().isEmpty() ? m_artistEdit->text() : m_artistEdit->placeholderText();
                        key = getSearchedKeyWork(1, key);
                MusicRightAreaWidget::instance()->musicArtistFound(QString(), key);
                break;
            }
        case 2:
            {
                QString key = !m_albumEdit->text().isEmpty() ? m_albumEdit->text() : m_albumEdit->placeholderText();
                        key = getSearchedKeyWork(2, key);
                MusicRightAreaWidget::instance()->musicAlbumFound(QString(), key);
                break;
            }
        case 3:
            {
                QString key = !m_playlistEdit->text().isEmpty() ? m_playlistEdit->text() : m_playlistEdit->placeholderText();
                        key = getSearchedKeyWork(3, key);
                MusicRightAreaWidget::instance()->musicPlaylistFound(key);
                break;
            }
        case 4:
            {
                QString key = !m_movieEdit->text().isEmpty() ? m_movieEdit->text() : m_movieEdit->placeholderText();
                        key = getSearchedKeyWork(4, key);
                MusicRightAreaWidget::instance()->musicVideoButtonSearched(QString(), key);
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
    showIcon->setStyleSheet(MusicUIObject::MBackgroundStyle01 + "border-image: url(:/tiny/btn_search_main_hover);");
    *input = new MusicLocalSongSearchEdit(searchPair);
    searchPairLayout->addWidget(showIcon);
    searchPairLayout->addWidget(*input);
    searchPair->setLayout(searchPairLayout);

    return searchPair;
}

void MusicAdvancedSearchedWidget::updateServerPlaceholderText()
{
    switch(M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt())
    {
        case 0:
            {
                m_songEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_SG_SHARE, ALG_LOW_KEY, false).arg("482395261"));
                m_artistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_AR_SHARE, ALG_LOW_KEY, false).arg("12146142"));
                m_albumEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_AL_SHARE, ALG_LOW_KEY, false).arg("35579387"));
                m_playlistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_PL_SHARE, ALG_LOW_KEY, false).arg("988604820"));
                m_movieEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(WY_MV_SHARE, ALG_LOW_KEY, false).arg("522362"));
                break;
            }
        case 1:
            {
                m_songEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_SG_SHARE, ALG_LOW_KEY, false).arg("003fOZMP0ep9Fh"));
                m_artistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_AR_SHARE, ALG_LOW_KEY, false).arg("0029gk1t3wpGWU"));
                m_albumEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_AL_SHARE, ALG_LOW_KEY, false).arg("003fk65m1ZaCG2"));
                m_playlistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_PL_SHARE, ALG_LOW_KEY, false).arg("889413740"));
                m_movieEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(QQ_MV_SHARE, ALG_LOW_KEY, false).arg("b0013k1imsl"));
                break;
            }
        case 2:
            {
                m_songEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(XM_SG_SHARE, ALG_LOW_KEY, false).arg("1796845157"));
                m_artistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(XM_AR_SHARE, ALG_LOW_KEY, false).arg("2110200690"));
                m_albumEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(XM_AL_SHARE, ALG_LOW_KEY, false).arg("2102764975"));
                m_playlistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(XM_PL_SHARE, ALG_LOW_KEY, false).arg("358404263"));
                m_movieEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(XM_MV_SHARE, ALG_LOW_KEY, false).arg("K6KGcx"));
                break;
            }
        case 3:
            {
                m_songEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(BD_SG_SHARE, ALG_LOW_KEY, false).arg("589759196"));
                m_artistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(BD_AR_SHARE, ALG_LOW_KEY, false).arg("2517"));
                m_albumEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(BD_AL_SHARE, ALG_LOW_KEY, false).arg("611662508"));
                m_playlistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(BD_PL_SHARE, ALG_LOW_KEY, false).arg("557963176"));
                m_movieEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(BD_MV_SHARE, ALG_LOW_KEY, false).arg("611661931"));
                break;
            }
        case 4:
            {
                m_songEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_SG_SHARE, ALG_LOW_KEY, false).arg("23983067"));
                m_artistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_AR_SHARE, ALG_LOW_KEY, false).arg("373"));
                m_albumEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_AL_SHARE, ALG_LOW_KEY, false).arg("2698608"));
                m_playlistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_PL_SHARE, ALG_LOW_KEY, false).arg("236350390"));
                m_movieEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KW_MV_SHARE, ALG_LOW_KEY, false).arg("9812716"));
                break;
            }
        case 5:
            {
                m_songEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_SG_SHARE, ALG_LOW_KEY, false).arg("0624926cfbdf905ae97d4ef6e01fea95"));
                m_artistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_AR_SHARE, ALG_LOW_KEY, false).arg("1169"));
                m_albumEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_AL_SHARE, ALG_LOW_KEY, false).arg("2179324"));
                m_playlistEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_PL_SHARE, ALG_LOW_KEY, false).arg("155318"));
                m_movieEdit->setPlaceholderText(MusicUtils::Algorithm::mdII(KG_MV_SHARE, ALG_LOW_KEY, false).arg("630600"));
                break;
            }
        default: break;
    }
}

QString MusicAdvancedSearchedWidget::getSearchedKeyWork(int type, const QString &url)
{
    QString key;
    switch(M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt())
    {
        case 0:
            {
                QRegExp regx("id=(\\d+)");
                key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
                break;
            }
        case 1:
            {
                QRegExp regx("/(\\w+).html");
                key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
                break;
            }
        case 2:
            {
                QRegExp regx(type == 4 ? "/mv/(\\w+)$" : "/(\\d+)");
                key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
                break;
            }
        case 3:
            {
                QRegExp regx("/(\\d+)");
                key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
                break;
            }
        case 4:
            {
                QRegExp regx;
                if(type == 1)
                    regx.setPattern("id=(\\d+)");
                else if(type == 3)
                    regx.setPattern("pid=(\\d+)");
                else
                    regx.setPattern("/(\\d+)");

                key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
                break;
            }
        case 5:
            {
                QRegExp regx;
                if(type == 0)
                    regx.setPattern("hash=(\\w+)");
                else if(type == 4)
                    regx.setPattern("/mv_(\\d+)");
                else
                    regx.setPattern("/(\\d+)");
                key = (url.indexOf(regx) != -1) ? regx.cap(1) : url;
                break;
            }
        default: break;
    }

    return key;
}

void MusicAdvancedSearchedWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
