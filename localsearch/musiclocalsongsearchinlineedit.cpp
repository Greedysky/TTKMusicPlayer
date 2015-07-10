#include "musiclocalsongsearchinlineedit.h"
#include <QDebug>
#include <QListWidget>

MusicLocalSongSearchInlineEdit::MusicLocalSongSearchInlineEdit(QWidget *parent)
    : MusicLocalSongSearchEdit(parent)
{
    m_historyLists = new QListWidget(parent);
    m_historyLists->setGeometry(484, 35, 285, 150);
    m_historyLists->setStyleSheet("background-color:black");
    m_historyLists->hide();
}

MusicLocalSongSearchInlineEdit::~MusicLocalSongSearchInlineEdit()
{

}

void MusicLocalSongSearchInlineEdit::focusInEvent(QFocusEvent *event)
{
    MusicLocalSongSearchEdit::focusInEvent(event);
    m_historyLists->raise();
    m_historyLists->show();
}

void MusicLocalSongSearchInlineEdit::focusOutEvent(QFocusEvent *event)
{
    MusicLocalSongSearchEdit::focusOutEvent(event);
    m_historyLists->lower();
    m_historyLists->hide();
}
