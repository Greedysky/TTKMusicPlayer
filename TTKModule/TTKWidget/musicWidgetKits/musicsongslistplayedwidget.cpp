#include "musicsongslistplayedwidget.h"
#include "musictinyuiobject.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"
#include "musicgiflabelwidget.h"
#include "musicleftareawidget.h"
#include "musicnumberdefine.h"
#include "musicwidgetheaders.h"

#include <QTimer>

MusicSongsListPlayedWidget::MusicSongsListPlayedWidget(int index, QWidget *parent)
    : QWidget(parent), m_parentClass(parent)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(0, 0, 0, 20)));
    setPalette(pal);
    setAutoFillBackground(true);

    m_currentPlayIndex = index;

    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(MusicUIObject::MColorStyle11);
    m_textLabel->setGeometry(23, 0, 170, 30);

    m_gifLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Gif_Radio_Blue, this);
    m_gifLabel->setInterval(50);
    m_gifLabel->setGeometry(193, 8, 14, 14);

    m_downloadButton = new QPushButton(this);
    m_downloadButton->setGeometry(220, 7, 16, 16);
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setToolTip(tr("songDownload"));
    m_downloadButton->setStyleSheet(MusicUIObject::MKGTinyBtnUnDownload);

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(245, 7, 16, 16);
    m_deleteButton->setStyleSheet(MusicUIObject::MKGTinyBtnDelete);
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("deleteMusic"));

    m_moreButton = new QPushButton(this);
    m_moreButton->setGeometry(270, 7, 16, 16);
    m_moreButton->setStyleSheet(MusicUIObject::MPushButtonStyle13 + MusicUIObject::MKGTinyBtnMore);
    m_moreButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_moreButton->setToolTip(tr("moreFunction"));

#ifdef Q_OS_UNIX
    m_downloadButton->setFocusPolicy(Qt::NoFocus);
    m_deleteButton->setFocusPolicy(Qt::NoFocus);
    m_moreButton->setFocusPolicy(Qt::NoFocus);
#endif

    QMenu *menu = new QMenu(this);
    createMoreMenu(menu);
    m_moreButton->setMenu(menu);

    connect(m_downloadButton, SIGNAL(clicked()), MusicLeftAreaWidget::instance(), SLOT(musicDownloadSongToLocal()));
    connect(m_deleteButton, SIGNAL(clicked()), SLOT(setDeleteItemAt()));
    connect(this, SIGNAL(enterChanged(int,int)), m_parentClass, SLOT(listCellEntered(int,int)));

}

MusicSongsListPlayedWidget::~MusicSongsListPlayedWidget()
{
    delete m_textLabel;
    delete m_gifLabel;
    delete m_downloadButton;
    delete m_deleteButton;
    delete m_moreButton;
}

void MusicSongsListPlayedWidget::setParameter(const QString &name)
{
    m_textLabel->setText(MusicUtils::Widget::elidedText(font(), name, Qt::ElideRight, 168));
    m_textLabel->setToolTip(name);
    m_gifLabel->start();
}

void MusicSongsListPlayedWidget::setDeleteItemAt()
{
    QTimer::singleShot(MT_MS, m_parentClass, SLOT(setDeleteItemAt()));
}

void MusicSongsListPlayedWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    emit enterChanged(m_currentPlayIndex, -1);
}

void MusicSongsListPlayedWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(MusicUIObject::MMenuStyle02);

    QMenu *addMenu = menu->addMenu(QIcon(":/contextMenu/btn_add"), tr("addToList"));
    addMenu->addAction(tr("musicCloud"));

    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("similar"), m_parentClass, SLOT(musicPlayedSimilarFoundWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("songShare"), m_parentClass, SLOT(musicSongPlayedSharedWidget()));
}
