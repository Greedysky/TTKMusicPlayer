#include "musicsongsearchdialog.h"
#include "musicitemsearchedit.h"
#include "musictinyuiobject.h"

MusicSongSearchDialog::MusicSongSearchDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    resize(LEFT_SIDE_WIDTH_MIN, 35);

    QPalette plt(palette());
    plt.setColor(QPalette::Window, QColor(255, 255, 225));
    setAutoFillBackground(true);
    setPalette(plt);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 4, 10, 4);

    m_searchLine = new MusicItemSearchEdit(this);
    QToolButton *closeButton = new QToolButton(this);
    closeButton->setFixedSize(16, 16);
    closeButton->setCursor(QCursor(Qt::PointingHandCursor));
    closeButton->setStyleSheet(TTK::UI::TinyBtnClose);

    layout->addWidget(m_searchLine);
    layout->addWidget(closeButton);
    setLayout(layout);

    connect(closeButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_searchLine->editor(), SIGNAL(cursorPositionChanged(int,int)), parent, SLOT(searchResultChanged(int,int)));
}

MusicSongSearchDialog::~MusicSongSearchDialog()
{
    delete m_searchLine;
}

bool MusicSongSearchDialog::close()
{
    clear();
    return QDialog::close();
}

QString MusicSongSearchDialog::text() const
{
    return m_searchLine->editor()->text();
}

void MusicSongSearchDialog::clear() const
{
    m_searchLine->editor()->clear();
}

void MusicSongSearchDialog::setVisible(bool visible)
{
    QDialog::setVisible(visible);
    m_searchLine->editor()->setFocus();
}
