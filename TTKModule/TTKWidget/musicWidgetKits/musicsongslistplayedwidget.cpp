#include "musicsongslistplayedwidget.h"
#include "musictinyuiobject.h"
#include "musicwidgetutils.h"
#include "musicgiflabelwidget.h"
#include "musicleftareawidget.h"
#include "musicwidgetheaders.h"

#include <QTimer>

MusicSongsListPlayedWidget::MusicSongsListPlayedWidget(int index, QWidget *parent)
    : QWidget(parent),
      m_currentPlayIndex(index),
      m_parent(parent)
{
    QPalette plt(palette());
    plt.setBrush(QPalette::Base, QBrush(QColor(0, 0, 0, 20)));
    setPalette(plt);
    setAutoFillBackground(true);

    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(TTK::UI::ColorStyle10);
    m_textLabel->setGeometry(23, 0, 170, 30);

    m_gifLabel = new MusicGifLabelWidget(MusicGifLabelWidget::Module::RadioBlue, this);
    m_gifLabel->setInterval(50);
    m_gifLabel->setGeometry(193, 8, 14, 14);

    m_downloadButton = new QPushButton(this);
    m_downloadButton->setGeometry(220, 7, 16, 16);
    m_downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_downloadButton->setToolTip(tr("Download"));
    m_downloadButton->setStyleSheet(TTK::UI::TinyBtnUnDownload);

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(245, 7, 16, 16);
    m_deleteButton->setStyleSheet(TTK::UI::TinyBtnDelete);
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("Delete"));

    m_moreButton = new QPushButton(this);
    m_moreButton->setGeometry(270, 7, 16, 16);
    m_moreButton->setStyleSheet(TTK::UI::PushButtonStyle10 + TTK::UI::TinyBtnMore);
    m_moreButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_moreButton->setToolTip(tr("More"));

#ifdef Q_OS_UNIX
    m_downloadButton->setFocusPolicy(Qt::NoFocus);
    m_deleteButton->setFocusPolicy(Qt::NoFocus);
    m_moreButton->setFocusPolicy(Qt::NoFocus);
#endif

    QMenu *menu = new QMenu(this);
    createMoreMenu(menu);
    m_moreButton->setMenu(menu);

    connect(m_downloadButton, SIGNAL(clicked()), MusicLeftAreaWidget::instance(), SLOT(downloadSongToLocal()));
    connect(m_deleteButton, SIGNAL(clicked()), SLOT(removeItemAt()));
    connect(this, SIGNAL(enterChanged(int,int)), m_parent, SLOT(itemCellEntered(int,int)));
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
    m_textLabel->setText(TTK::Widget::elidedText(font(), name, Qt::ElideRight, 168));
    m_textLabel->setToolTip(name);
    m_gifLabel->start();
}

void MusicSongsListPlayedWidget::removeItemAt()
{
    QTimer::singleShot(MT_ONCE, m_parent, SLOT(removeItemAt()));
}

void MusicSongsListPlayedWidget::enterEvent(QtEnterEvent *event)
{
    QWidget::enterEvent(event);
    Q_EMIT enterChanged(m_currentPlayIndex, -1);
}

void MusicSongsListPlayedWidget::createMoreMenu(QMenu *menu)
{
    menu->setStyleSheet(TTK::UI::MenuStyle02);
    menu->addAction(QIcon(":/contextMenu/btn_similar"), tr("Similar"), m_parent, SLOT(showPlayedSimilarQueryWidget()));
    menu->addAction(QIcon(":/contextMenu/btn_share"), tr("Share"), m_parent, SLOT(showPlayedSongSharedWidget()));
}
