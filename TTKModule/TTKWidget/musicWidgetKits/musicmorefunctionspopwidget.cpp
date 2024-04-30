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
        case 0: MusicRightAreaWidget::instance()->showSongCommentsFound(); break;
        case 1: MusicRightAreaWidget::instance()->showSimilarFound(m_currentSongName); break;
        case 2: MusicRightAreaWidget::instance()->showAlbumFound(m_currentSongName, {}); break;
        case 3: MusicRightAreaWidget::instance()->showArtistFound(m_currentSongName, {}); break;
        case 4: MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::SongDailyWidget); break;
        case 5: MusicRightAreaWidget::instance()->functionClicked(MusicRightAreaWidget::AdvancedSearchWidget); break;
        default: break;
    }
}

void MusicMoreFunctionsPopWidget::initialize()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(140, 190);
    m_menu->removeAction(m_menu->actions().front());

    QActionGroup *group = new QActionGroup(this);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_message_hover"), tr("Message")))->setData(0);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_similar_hover"), tr("Similar")))->setData(1);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_ablum_hover"), tr("Ablum")))->setData(2);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_artist_hover"), tr("Artist")))->setData(3);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_recommend_hover"), tr("Daily")))->setData(4);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_search_hover"), tr("Advance")))->setData(5);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(functionClicked(QAction*)));
}
