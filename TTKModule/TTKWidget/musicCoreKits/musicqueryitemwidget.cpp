#include "musicqueryitemwidget.h"
#include "musicwidgetutils.h"
#include "musicimagerenderer.h"
#include "musictinyuiobject.h"
#include "musicwidgetheaders.h"

MusicSquareQueryItemWidget::MusicSquareQueryItemWidget(QWidget *parent)
    : QLabel(parent),
      m_showTime(false),
      m_showCount(true)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_listenButton = new QPushButton(this);
    m_listenButton->setGeometry(0, 0, WIDTH_LABEL_SIZE, 20);
    m_listenButton->setIcon(QIcon(":/tiny/btn_listen_hover"));
    m_listenButton->setText(TTK_DEFAULT_STR);
    m_listenButton->setStyleSheet(TTK::UI::BorderStyle01 + TTK::UI::BackgroundStyle04 + TTK::UI::ColorStyle06);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry(110, 110, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(TTK::UI::TinyBtnPlaylist + TTK::UI::PushButtonStyle01);
    connect(m_playButton, SIGNAL(clicked()), SLOT(itemClicked()));

#ifdef Q_OS_UNIX
    m_listenButton->setFocusPolicy(Qt::NoFocus);
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new TTKClickedLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);
    connect(m_iconLabel, SIGNAL(clicked()), SLOT(itemClicked()));

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 150, WIDTH_LABEL_SIZE, 25);
    m_nameLabel->setText(TTK_DEFAULT_STR);

    m_valueLabel = new QLabel(this);
    m_valueLabel->setGeometry(0, 175, WIDTH_LABEL_SIZE, 25);
    m_valueLabel->setStyleSheet(TTK::UI::ColorStyle03);
    m_valueLabel->setText("-");
}

MusicSquareQueryItemWidget::~MusicSquareQueryItemWidget()
{
    delete m_listenButton;
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
    delete m_valueLabel;
}

void MusicSquareQueryItemWidget::setResultDataItem(const MusicResultDataItem &item, MusicCoverRequest *cover)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(TTK::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(m_showTime)
    {
        m_valueLabel->setToolTip(item.m_time);
    }
    else
    {
        m_valueLabel->setToolTip("by " + item.m_nickName);
    }
    m_valueLabel->setText(TTK::Widget::elidedText(m_valueLabel->font(), m_valueLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(!m_showCount)
    {
        m_listenButton->setVisible(false);
    }
    else
    {
        bool ok = false;
        const int count = item.m_count.toInt(&ok);
        if(ok)
        {
            if(count >= 10000)
            {
                m_listenButton->setText(tr("%1W").arg(count / 10000));
            }
            else
            {
                m_listenButton->setText(QString::number(count));
            }
        }
        else
        {
            m_listenButton->setText(item.m_count);
        }
    }

    if(cover && TTK::isCoverValid(item.m_coverUrl))
    {
        connect(cover, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        cover->startToRequest(item.m_coverUrl);
    }
}

void MusicSquareQueryItemWidget::downloadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicImageRenderer *render = new MusicImageRenderer(sender());
    connect(render, SIGNAL(renderFinished(QPixmap)), SLOT(renderFinished(QPixmap)));
    render->setInputData(bytes, m_iconLabel->size());
    render->start();
}

void MusicSquareQueryItemWidget::renderFinished(const QPixmap &data)
{
    m_iconLabel->setPixmap(data);
    m_listenButton->raise();
    m_playButton->raise();
}

void MusicSquareQueryItemWidget::itemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}


MusicRectQueryItemWidget::MusicRectQueryItemWidget(QWidget *parent)
    : QLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_playButton = new QPushButton(this);
    m_playButton->setGeometry((WIDTH_LABEL_SIZE - 30) / 2, (HEIGHT_LABEL_SIZE - 30) / 2, 30, 30);
    m_playButton->setCursor(Qt::PointingHandCursor);
    m_playButton->setStyleSheet(TTK::UI::TinyBtnPlaylist);
    connect(m_playButton, SIGNAL(clicked()), SLOT(itemClicked()));

#ifdef Q_OS_UNIX
    m_playButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_iconLabel = new TTKClickedLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);
    connect(m_iconLabel, SIGNAL(clicked()), SLOT(itemClicked()));

    m_nameLabel = new QLabel(this);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);
    m_nameLabel->setStyleSheet(TTK::UI::ColorStyle06 + TTK::UI::FontStyle05 + TTK::UI::FontStyle01);
    m_nameLabel->setText(TTK_DEFAULT_STR);
}

MusicRectQueryItemWidget::~MusicRectQueryItemWidget()
{
    delete m_playButton;
    delete m_iconLabel;
    delete m_nameLabel;
}

void MusicRectQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(TTK::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *req = new MusicCoverSourceRequest(this);
        connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        req->startToRequest(item.m_coverUrl);
    }

    m_playButton->hide();
}

void MusicRectQueryItemWidget::downloadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    MusicImageRenderer *render = new MusicImageRenderer(sender());
    connect(render, SIGNAL(renderFinished(QPixmap)), SLOT(renderFinished(QPixmap)));
    render->setInputData(bytes, m_iconLabel->size());
    render->start();
}

void MusicRectQueryItemWidget::renderFinished(const QPixmap &data)
{
    m_iconLabel->setPixmap(data);
    m_playButton->raise();
}

void MusicRectQueryItemWidget::itemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}

void MusicRectQueryItemWidget::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);

    m_playButton->show();
    m_nameLabel->hide();
}

void MusicRectQueryItemWidget::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);

    m_playButton->hide();
    m_nameLabel->show();
}


MusicLabelQueryItemWidget::MusicLabelQueryItemWidget(QWidget *parent)
    : TTKClickedLabel(parent)
{
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setGeometry(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setGeometry(0, 65, WIDTH_LABEL_SIZE, 25);
    m_nameLabel->setAlignment(Qt::AlignCenter);
    m_nameLabel->setText(TTK_DEFAULT_STR);

    connect(this, SIGNAL(clicked()), SLOT(itemClicked()));
}

MusicLabelQueryItemWidget::~MusicLabelQueryItemWidget()
{
    delete m_iconLabel;
    delete m_nameLabel;
}

void MusicLabelQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    m_nameLabel->setToolTip(item.m_name);
    m_nameLabel->setText(TTK::Widget::elidedText(m_nameLabel->font(), m_nameLabel->toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));

    if(TTK::isCoverValid(item.m_coverUrl))
    {
        MusicCoverRequest *req = new MusicCoverSourceRequest(this);
        connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
        req->startToRequest(item.m_coverUrl);
    }
}

void MusicLabelQueryItemWidget::downloadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
        return;
    }

    QPixmap pix;
    pix.loadFromData(bytes);
    if(!pix.isNull())
    {
        m_iconLabel->setPixmap(pix.copy(0, 0, WIDTH_LABEL_SIZE, WIDTH_LABEL_SIZE));
    }
}

void MusicLabelQueryItemWidget::itemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}


MusicTextQueryItemWidget::MusicTextQueryItemWidget(QWidget *parent)
    : TTKClickedLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setStyleSheet(TTK::UI::ColorStyle02);
    setFixedSize(WIDTH_LABEL_SIZE, HEIGHT_LABEL_SIZE);

    connect(this, SIGNAL(clicked()), SLOT(itemClicked()));
}

void MusicTextQueryItemWidget::setResultDataItem(const MusicResultDataItem &item)
{
    m_itemData = item;
    setToolTip(item.m_name);
    setText(TTK::Widget::elidedText(font(), toolTip(), Qt::ElideRight, WIDTH_LABEL_SIZE));
}

void MusicTextQueryItemWidget::itemClicked()
{
    Q_EMIT currentItemClicked(m_itemData);
}


