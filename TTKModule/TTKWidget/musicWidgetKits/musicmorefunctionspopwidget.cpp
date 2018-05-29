#include "musicmorefunctionspopwidget.h"
#include "musicrightareawidget.h"
#include "musicuiobject.h"

MusicMoreFunctionsPopWidget::MusicMoreFunctionsPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

void MusicMoreFunctionsPopWidget::setCurrentSongName(const QString &name)
{
    m_currentSongName = name;
}

void MusicMoreFunctionsPopWidget::musicFunctionClicked(QAction *index)
{
    switch(index->data().toInt())
    {
        case 0:
            {
                MusicRightAreaWidget::instance()->musicSongCommentsWidget();
                break;
            }
        case 1:
            {
                MusicRightAreaWidget::instance()->musicSimilarFound(m_currentSongName);
                break;
            }
        case 2:
            {
                MusicRightAreaWidget::instance()->musicAlbumFound(m_currentSongName, QString());
                break;
            }
        case 3:
            {
                MusicRightAreaWidget::instance()->musicArtistFound(m_currentSongName, QString());
                break;
            }
        case 4:
            {
                MusicRightAreaWidget::instance()->musicToplistFound();
                break;
            }
        case 5:
            {
                MusicRightAreaWidget::instance()->musicPlaylistFound(QString());
                break;
            }
        case 6:
            {
                MusicRightAreaWidget::instance()->musicRecommendFound();
                break;
            }
        case 7:
            {
                MusicRightAreaWidget::instance()->musicAdvancedSearch();
                break;
            }
        case 8:
            {
                MusicRightAreaWidget::instance()->musicArtistCategoryFound();
                break;
            }
        default: break;
    }
}

void MusicMoreFunctionsPopWidget::initWidget()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(140, 285);
    m_menu->removeAction(m_menu->actions().first());

    QActionGroup *group = new QActionGroup(this);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_message_hover"), tr("message")))->setData(0);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_similar_hover"), tr("similar")))->setData(1);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_ablum_hover"), tr("ablum")))->setData(2);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_artist_hover"), tr("artist")))->setData(3);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_toplist_hover"), tr("toplist")))->setData(4);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_playlist_hover"), tr("playlist")))->setData(5);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_recommend_hover"), tr("recommend")))->setData(6);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_search_hover"), tr("search")))->setData(7);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_artist_hover"), tr("artists")))->setData(8);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(musicFunctionClicked(QAction*)));
}
