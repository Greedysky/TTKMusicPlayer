#include "musiclocalsongsearchdialog.h"
#include "musiclocalsongsearchedit.h"
#include "musicuiobject.h"
#include "musictinyuiobject.h"

#include <QLabel>
#include <QBoxLayout>
#include <QToolButton>

MusicLocalSongSearchDialog::MusicLocalSongSearchDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    resize(320, 35);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, QColor(255, 255, 255));
    setAutoFillBackground(true);
    setPalette(Pal);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 4, 10, 4);

    QWidget *searchPair = new QWidget(this);
    searchPair->setStyleSheet(MusicUIObject::MBorderStyle02);
    QHBoxLayout *searchPairLayout = new QHBoxLayout(searchPair);
    searchPairLayout->setContentsMargins(5, 2, 5, 0);
    QLabel *showIcon = new QLabel(searchPair);
    showIcon->setFixedSize(16, 16);
    showIcon->setStyleSheet(MusicUIObject::MBackgroundStyle01 +
                            "border-image: url(:/tiny/btn_search_line_normal);");
    m_searchLine = new MusicLocalSongSearchEdit(searchPair);
    searchPairLayout->addWidget(showIcon);
    searchPairLayout->addWidget(m_searchLine);
    searchPair->setLayout(searchPairLayout);

    QToolButton *closeButton = new QToolButton(this);
    closeButton->setFixedSize(16, 16);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    closeButton->setStyleSheet(MusicUIObject::MKGTinyBtnClose);

    layout->addWidget(searchPair);
    layout->addWidget(closeButton);
    setLayout(layout);

    connect(closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_searchLine, SIGNAL(cursorPositionChanged(int,int)),
                  parent, SLOT(musicSearchIndexChanged(int,int)));
}

MusicLocalSongSearchDialog::~MusicLocalSongSearchDialog()
{
    delete m_searchLine;
}

QString MusicLocalSongSearchDialog::getClassName()
{
    return staticMetaObject.className();
}

bool MusicLocalSongSearchDialog::close()
{
    clearSearchedText();
    return QDialog::close();
}

QString MusicLocalSongSearchDialog::getSearchedText() const
{
    return m_searchLine->text();
}

void MusicLocalSongSearchDialog::clearSearchedText() const
{
    m_searchLine->clear();
}
