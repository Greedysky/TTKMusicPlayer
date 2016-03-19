#include "musiclocalsongsearch.h"
#include "musiclocalsongsearchedit.h"
#include "musicuiobject.h"

#include <QLabel>
#include <QToolButton>

MusicLocalSongSearch::MusicLocalSongSearch(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    resize(330, 35);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background,QColor(176, 196, 222));
    setAutoFillBackground(true);
    setPalette(Pal);

    m_showIcon = new QLabel(this);
    m_searchLine = new MusicLocalSongSearchEdit(this);
    m_closeButton = new QToolButton(this);

    m_showIcon->setGeometry(5, 5, 25, 25);
    m_showIcon->setStyleSheet(MusicUIObject::MCustomStyle05);
    m_searchLine->setGeometry(30, 5, 295, 25);

    m_closeButton->setGeometry(303, 5, 25, 25);
    m_closeButton->setStyleSheet(MusicUIObject::MCustomStyle06);

    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_searchLine, SIGNAL(cursorPositionChanged(int,int)),
                  parent, SLOT(musicSearchIndexChanged(int,int)));
}

MusicLocalSongSearch::~MusicLocalSongSearch()
{
    delete m_showIcon;
    delete m_searchLine;
    delete m_closeButton;
}

bool MusicLocalSongSearch::close()
{
    clearSearchedText();
    return QDialog::close();
}

QString MusicLocalSongSearch::getSearchedText() const
{
    return m_searchLine->text();
}

void MusicLocalSongSearch::clearSearchedText() const
{
    m_searchLine->clear();
}
