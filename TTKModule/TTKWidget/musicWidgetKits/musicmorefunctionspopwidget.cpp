#include "musicmorefunctionspopwidget.h"
#include "musicrightareawidget.h"

MusicMoreFunctionsPopWidget::MusicMoreFunctionsPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initialize();
}

void MusicMoreFunctionsPopWidget::setCurrentSongName(const QString &name)
{
    m_currentSongName = name;
}

void MusicMoreFunctionsPopWidget::functionClicked(QAction *index)
{
    switch(index->data().toInt())
    {
        case 0: MusicRightAreaWidget::instance()->showSongCommentsWidget(); break;
        case 1: MusicRightAreaWidget::instance()->showSimilarFound(m_currentSongName); break;
        case 2: MusicRightAreaWidget::instance()->showAlbumFound(m_currentSongName, QString()); break;
        case 3: MusicRightAreaWidget::instance()->showArtistFound(m_currentSongName, QString()); break;
        case 4: MusicRightAreaWidget::instance()->showToplistFound(); break;
        case 5: MusicRightAreaWidget::instance()->showPlaylistFound(QString()); break;
        case 6: MusicRightAreaWidget::instance()->showRecommendFound(); break;
        case 7: MusicRightAreaWidget::instance()->showAdvancedSearchFound(); break;
        case 8: MusicRightAreaWidget::instance()->showArtistCategoryFound(); break;
        default: break;
    }
}

void MusicMoreFunctionsPopWidget::initialize()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(140, 285);
    m_menu->removeAction(m_menu->actions().front());

    QActionGroup *group = new QActionGroup(this);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_message_hover"), tr("Message")))->setData(0);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_similar_hover"), tr("Similar")))->setData(1);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_ablum_hover"), tr("Ablum")))->setData(2);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_artist_hover"), tr("Artist")))->setData(3);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_toplist_hover"), tr("Toplist")))->setData(4);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_playlist_hover"), tr("Playlist")))->setData(5);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_recommend_hover"), tr("Recommend")))->setData(6);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_search_hover"), tr("Advance")))->setData(7);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_artist_hover"), tr("Artists")))->setData(8);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(functionClicked(QAction*)));
}
