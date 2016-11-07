#include "musicsongsplayedlistwidget.h"

MusicSongsPlayedListWidget::MusicSongsPlayedListWidget(QWidget *parent)
    : QWidget(parent)
{

}

MusicSongsPlayedListWidget::~MusicSongsPlayedListWidget()
{

}

QString MusicSongsPlayedListWidget::getClassName()
{
    return staticMetaObject.className();
}
