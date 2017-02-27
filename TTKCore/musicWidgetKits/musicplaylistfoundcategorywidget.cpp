#include "musicplaylistfoundcategorywidget.h"
#include "musicuiobject.h"

MusicPlaylistFoundCategoryWidget::MusicPlaylistFoundCategoryWidget(QWidget *parent)
    : MusicToolMenuWidget(parent)
{
    initWidget();

    disconnect(this, SIGNAL(clicked()), this, SLOT(popupMenu()));
    connect(this, SIGNAL(clicked()), SLOT(popupMenu()));
}

MusicPlaylistFoundCategoryWidget::~MusicPlaylistFoundCategoryWidget()
{

}

QString MusicPlaylistFoundCategoryWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlaylistFoundCategoryWidget::popupMenu()
{
    m_menu->exec( mapToGlobal(QPoint(0, 0)) );
}

void MusicPlaylistFoundCategoryWidget::initWidget()
{
    setFixedSize(100, 30);
    setTranslucentBackground();
    setStyleSheet(MusicUIObject::MBorderStyle04 + MusicUIObject::MBackgroundStyle17);
    m_menu->setStyleSheet(MusicUIObject::MMenuStyle05);

    m_containWidget->setFixedSize(600, 370);

}
