#include "musicmorefunctionspopwidget.h"
#include "musicsongsharingwidget.h"
#include "musicrightareawidget.h"
#include "musicuiobject.h"

MusicMoreFunctionsPopWidget::MusicMoreFunctionsPopWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();
}

QString MusicMoreFunctionsPopWidget::getClassName()
{
    return staticMetaObject.className();
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
                MusicRightAreaWidget::instance()->musicAlbumFound(m_currentSongName);
                break;
            }
        case 3:
            {
                MusicRightAreaWidget::instance()->musicArtistFound(m_currentSongName);
                break;
            }
        case 4:
            {
                MusicRightAreaWidget::instance()->musicPlaylistFound();
                break;
            }
        case 5:
            {
                MusicSongSharingWidget shareWidget;
                shareWidget.setSongName(m_currentSongName);
                shareWidget.exec();
                break;
            }
        case 6:
            {
                MusicRightAreaWidget::instance()->musicVideoButtonSearched(m_currentSongName);
                break;
            }
        default: break;
    }
}

void MusicMoreFunctionsPopWidget::initWidget()
{
    setTranslucentBackground();
    m_containWidget->setFixedSize(140, 220);
    m_menu->removeAction(m_menu->actions().first());

    QActionGroup *group = new QActionGroup(this);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_message_hover"), tr("message")))->setData(0);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_similar_hover"), tr("similar")))->setData(1);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_ablum_hover"), tr("ablum")))->setData(2);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_artist_hover"), tr("artist")))->setData(3);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_playlist_hover"), tr("playlist")))->setData(4);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_share_hover"), tr("share")))->setData(5);
    group->addAction(m_menu->addAction(QIcon(":/functions/btn_mv_hover"), tr("showMv")))->setData(6);
    connect(group, SIGNAL(triggered(QAction*)), SLOT(musicFunctionClicked(QAction*)));
}
