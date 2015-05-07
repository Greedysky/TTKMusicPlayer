#include "musiclocalsongsearch.h"
#include "musiclocalsongsearchedit.h"

MusicLocalSongSearch::MusicLocalSongSearch(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(50,406,333,35);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background,QColor(176,196,222));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    m_showIcon = new QLabel(this);
    m_searchLine = new MusicLocalSongSearchEdit(this);
    m_closeButton = new QToolButton(this);

    m_showIcon->setGeometry(5,5,25,25);
    m_showIcon->setStyleSheet("background-color:transparent;border-image: url(:/image/searchlineleft);");
    m_searchLine->setGeometry(30,5,298,25);

    m_closeButton->setGeometry(303,5,25,25);
    m_closeButton->setStyleSheet("border-image: url(:/share/searchclosed);");

    m_closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_closeButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(m_searchLine,SIGNAL(cursorPositionChanged(int,int)),parent,SLOT(musicSearchIndexChanged(int,int)));
}

MusicLocalSongSearch::~MusicLocalSongSearch()
{
    delete m_showIcon;
    delete m_searchLine;
    delete m_closeButton;
}

QString MusicLocalSongSearch::getSearchedText() const
{
    return m_searchLine->text();
}

void MusicLocalSongSearch::clearSearchedText()
{
    return m_searchLine->clear();
}
