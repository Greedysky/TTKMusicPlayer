#include "musiclrcposterwidget.h"
#include "ui_musiclrcposterwidget.h"
#include "musicbackgroundmanager.h"
#include "musicfileutils.h"
#include "musicimageutils.h"
#include "musicextractwrapper.h"
#include "musicbackgroundconfigmanager.h"
#include "ttkdesktopwrapper.h"

#include "qalgorithm/imagewrapper.h"

#include <qmath.h>

static constexpr int ITEM_WIDTH = 275;
static constexpr int ITEM_HEIGHT = 330;
static constexpr int ITEM_BORDER = 3;
static constexpr int ITEM_SCROLL_WIDTH = 8;

MusicLrcPosterItemWidget::MusicLrcPosterItemWidget(QWidget *parent)
    : QWidget(parent),
      m_type(Format::Type01)
{
    m_pixmap.load(G_BACKGROUND_PTR->artistImageDefaultPath());
    if(m_pixmap.isNull())
    {
        MusicBackgroundImage image;
        MusicExtractWrapper::outputSkin(&image, G_BACKGROUND_PTR->backgroundUrl());
        m_pixmap = image.m_pix;
    }
}

bool MusicLrcPosterItemWidget::hasScroll() const
{
     return height() > ITEM_HEIGHT;
}

void MusicLrcPosterItemWidget::setImagePath(const QString &path)
{
    m_pixmap.load(path);
    update();
}

void MusicLrcPosterItemWidget::setCurrentSongName(const QString &name)
{
    m_title = name;
}

void MusicLrcPosterItemWidget::textChanged(const QStringList &data)
{
    m_data = data;
    update();
}

void MusicLrcPosterItemWidget::currentTypeChanged(int type)
{
    m_type = TTKStaticCast(Format, type);
    setFixedHeight(ITEM_HEIGHT);

    update();
}

void MusicLrcPosterItemWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    switch(m_type)
    {
        case Format::Type01: drawTheme1(&painter); break;
        case Format::Type02: drawTheme2(&painter); break;
        case Format::Type03: drawTheme3(&painter); break;
        case Format::Type04: drawTheme4(&painter); break;
        case Format::Type05: drawTheme5(&painter); break;
        case Format::Type06: drawTheme6(&painter); break;
        case Format::Type07: drawTheme7(&painter); break;
        case Format::Type08: drawTheme8(&painter); break;
        case Format::Type09: drawTheme9(&painter); break;
        case Format::Type10: drawTheme10(&painter); break;
        case Format::Type11: drawTheme11(&painter); break;
        case Format::Type12: drawTheme12(&painter); break;
        case Format::Type13: drawTheme13(&painter); break;
        case Format::Type14: drawTheme14(&painter); break;
        case Format::Type15: drawTheme15(&painter); break;
        case Format::Type16: drawTheme16(&painter); break;
        case Format::Type17: drawTheme17(&painter); break;
        default: break;
    }
}

void MusicLrcPosterItemWidget::drawTheme1(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);

    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = pix.height() + 5 * ITEM_BORDER;
    int v = 1;
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 4 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(2 * ITEM_BORDER, offset, ITEM_WIDTH - 4 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 4 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + v * lineHeight;
    painter->drawText(2 * ITEM_BORDER, offset, ITEM_WIDTH - 4 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    offset += v;
    //
    offset += 3 * ITEM_BORDER;
    painter->drawText(2 * ITEM_BORDER, offset, ITEM_WIDTH - 4 * ITEM_BORDER, lineHeight, Qt::AlignCenter, tr("TTKMusicPlayer"));
    offset += 3 * ITEM_BORDER;
    //
    setFixedHeight(offset >= ITEM_HEIGHT ? offset + 2 * lineHeight : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme2(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 5 * ITEM_BORDER + lineHeight;
    int v = 1;
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        offset += v + 3 * ITEM_BORDER + v * lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    //
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + v * lineHeight;
    offset += v + 10 * ITEM_BORDER;
    if(offset < ITEM_HEIGHT)
    {
        offset = ITEM_HEIGHT;
    }
    painter->drawText(8 * ITEM_BORDER, offset - v - 10 * ITEM_BORDER, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QPen(QColor(0xBB, 0xBB, 0xBB), ITEM_BORDER));
    painter->drawRect(5 * ITEM_BORDER, 5 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, offset - 10 * ITEM_BORDER);
    setFixedHeight(offset);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    offset = 8 * ITEM_BORDER;
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme3(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);

    const int lineHeight = TTK::Widget::fontTextHeight(font());
    const int delta = 5 * ITEM_BORDER + lineHeight;
    int offset = 0;
    TTKIntList list;
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    painter->translate(2 * ITEM_BORDER, pix.height());
    painter->rotate(TTK_AN_90);
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    list << TTK::Widget::fontTextWidth(font(), title);
    painter->drawText(8 * ITEM_BORDER, 0, title);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta * (i + 1);
        if(offset <= delta)
        {
            break;
        }

        list << TTK::Widget::fontTextWidth(font(), m_data[i]);
        painter->drawText(8 * ITEM_BORDER, -offset, m_data[i]);
    }
    std::sort(list.begin(), list.end());
    //
    offset = pix.height() + list.back() + 10 * ITEM_BORDER;
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme4(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    const int delta = 5 * ITEM_BORDER + lineHeight;
    int offset = ITEM_WIDTH - delta;
    TTKIntList list;
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    painter->translate(2 * ITEM_BORDER, 0);
    painter->rotate(TTK_AN_90);
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta * (i + 1);
        if(offset <= delta || offset <= 2 * delta)
        {
            break;
        }

        list << TTK::Widget::fontTextWidth(font(), m_data[i]);
        painter->drawText(3 * ITEM_BORDER, -offset, m_data[i]);
    }
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    list << TTK::Widget::fontTextWidth(font(), title);
    if(!m_data.isEmpty())
    {
        offset -= delta;
    }
    painter->drawText(3 * ITEM_BORDER, -offset, title);
    //
    std::sort(list.begin(), list.end());
    painter->rotate(-TTK_AN_90);
    painter->translate(-2 * ITEM_BORDER, 0);

    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    offset = list.back() + delta - 2 * ITEM_BORDER;
    painter->drawPixmap(ITEM_BORDER, offset, pix);
    offset += ITEM_BORDER + pix.height();
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme5(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 5 * ITEM_BORDER;
    int v = 1;
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 4 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(2 * ITEM_BORDER, offset, ITEM_WIDTH - 4 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 4 * ITEM_BORDER) + 1;
    painter->drawText(2 * ITEM_BORDER, offset, ITEM_WIDTH - 4 * ITEM_BORDER, v * lineHeight, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    offset += 3 * ITEM_BORDER + lineHeight;
    painter->drawPixmap(ITEM_BORDER, offset, pix);
    offset += ITEM_BORDER + pix.height();
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme6(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (pix.width() - offset) / 2;
    pix = TTK::Image::roundedPixmap(pix, QRect(0, 0, offset, offset), offset, offset);
    painter->drawPixmap(ITEM_BORDER + fixedOffset, ITEM_BORDER, pix);

    const int lineHeight = TTK::Widget::fontTextHeight(font());
    const int delta = 5 * ITEM_BORDER + lineHeight;
    offset = ITEM_WIDTH - delta;
    TTKIntList list;
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    painter->translate(2 * ITEM_BORDER, pix.height());
    painter->rotate(TTK_AN_90);
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    list << TTK::Widget::fontTextWidth(font(), title);
    painter->drawText(8 * ITEM_BORDER, -offset, title);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta * (i + 2);
        if(offset <= delta)
        {
            break;
        }

        list << TTK::Widget::fontTextWidth(font(), m_data[i]);
        painter->drawText(8 * ITEM_BORDER, -offset, m_data[i]);
    }
    std::sort(list.begin(), list.end());
    //
    offset = pix.height() + list.back() + 10 * ITEM_BORDER;
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme7(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    const int delta = 5 * ITEM_BORDER + lineHeight;
    int offset = 8 * ITEM_BORDER;
    int v = 1;
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    painter->drawLine(5 * ITEM_BORDER, 5 * ITEM_BORDER, ITEM_WIDTH - 5 * ITEM_BORDER, 5 * ITEM_BORDER);
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        v = 5 * ITEM_BORDER + v * lineHeight;
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;

        painter->setPen(QColor(0xBB, 0xBB, 0xBB));
        painter->drawLine(5 * ITEM_BORDER, offset - 3 * ITEM_BORDER, ITEM_WIDTH - 5 * ITEM_BORDER, offset - 3 * ITEM_BORDER);
    }
    //
    offset += delta;
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 10 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + v * lineHeight;
    painter->drawText(5 * ITEM_BORDER, offset - v, ITEM_WIDTH - 10 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    offset += v;

    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme8(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 5 * ITEM_BORDER;
    int v = 1;
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 10 * ITEM_BORDER) + 1;
        offset += v + 3 * ITEM_BORDER + v * lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    //
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 10 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + v * lineHeight;
    offset += v + 10 * ITEM_BORDER;
    if(offset < ITEM_HEIGHT)
    {
        offset = ITEM_HEIGHT;
    }
    painter->drawText(5 * ITEM_BORDER, offset - v - 10 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->translate(0, 0);
    QPixmap borer(":/lrc/lb_poster_letter_line");
    for(int i = 0; i <= ceil(ITEM_WIDTH/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width() * i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width() * i, offset - borer.height(), borer.width(), borer.height(), borer);
    }
    painter->translate(borer.height(), 0);
    painter->rotate(TTK_AN_90);
    for(int i = 0; i <= ceil(offset/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width() * i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width() * i, borer.height() - ITEM_WIDTH, borer.width(), borer.height(), borer);
    }
    painter->rotate(-TTK_AN_90);
    painter->translate(0, 0);
    //
    setFixedHeight(offset);
    //
    offset = 5 * ITEM_BORDER;
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 10 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(5 * ITEM_BORDER, offset, ITEM_WIDTH - 10 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;

        painter->setPen(QColor(0xBB, 0xBB, 0xBB));
        painter->drawLine(5 * ITEM_BORDER, offset - ITEM_BORDER, ITEM_WIDTH - 5 * ITEM_BORDER, offset - ITEM_BORDER);
    }
}

void MusicLrcPosterItemWidget::drawTheme9(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 5 * ITEM_BORDER;
    int v = 1;
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 10 * ITEM_BORDER) + 1;
        offset += v + 3 * ITEM_BORDER + v * lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    //
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 10 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + v * lineHeight;
    offset += v + 10 * ITEM_BORDER;
    if(offset < ITEM_HEIGHT)
    {
        offset = ITEM_HEIGHT;
    }
    painter->drawText(5 * ITEM_BORDER, offset - v - 10 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->translate(0, 0);
    QPixmap borer(":/lrc/lb_poster_letter_line");
    for(int i = 0; i <= ceil(ITEM_WIDTH/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width() * i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width() * i, offset - borer.height(), borer.width(), borer.height(), borer);
    }
    painter->translate(borer.height(), 0);
    painter->rotate(TTK_AN_90);
    for(int i = 0; i <= ceil(offset/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width() * i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width() * i, borer.height() - ITEM_WIDTH, borer.width(), borer.height(), borer);
    }
    painter->rotate(-TTK_AN_90);
    painter->translate(0, 0);
    //
    QPixmap background(":/lrc/lb_poster_letter_background");
    painter->drawPixmap(ITEM_WIDTH - background.width() - 2 * ITEM_BORDER, background.height(), background.width(), background.height(), background);
    //
    setFixedHeight(offset);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    offset = 5 * ITEM_BORDER;
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 10 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(5 * ITEM_BORDER, offset, ITEM_WIDTH - 10 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme10(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (ITEM_HEIGHT - offset) / 2;
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER + fixedOffset, pix);

    const int gWidth = pix.width() * 3 / 4;
    const int gHeight = pix.height() * 3 / 4;
    const QRect &gaussRect = QRect((pix.width() - gWidth) / 2, (pix.height() - gHeight) / 2, gWidth, gHeight);

    QAlgorithm::GaussBlur blur;
    blur.input(gaussRect);

    const QRect &drawRect = QRect((pix.width() - gWidth) / 2, (pix.height() - gHeight) / 2 + fixedOffset, gWidth, gHeight);
    painter->drawPixmap(drawRect, blur.render(pix, 10));
    //
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int v = 1;
    offset = 3 * ITEM_BORDER;

    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(drawRect.width() - 6 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        if(offset + v >= gaussRect.height())
        {
            break;
        }
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(3 * ITEM_BORDER + drawRect.x(), offset + drawRect.y(), drawRect.width() - 6 * ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme11(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 8 * ITEM_BORDER;
    int v = 1;
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        offset += v + 3 * ITEM_BORDER + v * lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + (v + 1) * lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_spring");
    int h = TTKStaticCast(int, pix.height() * width() * 1.0f / pix.width());
    int w = width();

    if(h <= height())
    {
        w = TTKStaticCast(int, width() * qMax(1.0f, height() * 1.0f / pix.height()));
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2 * ITEM_BORDER, h - 2 * ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5 * ITEM_BORDER, 5 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, height() - 10 * ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8 * ITEM_BORDER;
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme12(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 8 * ITEM_BORDER;
    int v = 1;
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        offset += v + 3 * ITEM_BORDER + v * lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + (v + 1) * lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_summer");
    int h = TTKStaticCast(int, pix.height() * width() * 1.0f / pix.width());
    int w = width();

    if(h <= height())
    {
        w = TTKStaticCast(int, width() * qMax(1.0f, height() * 1.0f / pix.height()));
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2 * ITEM_BORDER, h - 2 * ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5 * ITEM_BORDER, 5 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, height() - 10 * ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8 * ITEM_BORDER;
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme13(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 8 * ITEM_BORDER;
    int v = 1;
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        offset += v + 3 * ITEM_BORDER + v * lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + (v + 1) * lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_autumn");
    int h = TTKStaticCast(int, pix.height() * width() * 1.0f / pix.width());
    int w = width();

    if(h <= height())
    {
        w = TTKStaticCast(int, width() * qMax(1.0f, height() * 1.0f / pix.height()));
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2 * ITEM_BORDER, h - 2 * ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5 * ITEM_BORDER, 5 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, height() - 10 * ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8 * ITEM_BORDER;
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme14(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int offset = 8 * ITEM_BORDER;
    int v = 1;
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        offset += v + 3 * ITEM_BORDER + v * lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + (v + 1) * lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_winter");
    int h = TTKStaticCast(int, pix.height() * width() * 1.0f / pix.width());
    int w = width();

    if(h <= height())
    {
        w = TTKStaticCast(int, width() * qMax(1.0f, height() * 1.0f / pix.height()));
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2 * ITEM_BORDER, h - 2 * ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5 * ITEM_BORDER, 5 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, height() - 10 * ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8 * ITEM_BORDER;
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 16 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        painter->drawText(8 * ITEM_BORDER, offset, ITEM_WIDTH - 16 * ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme15(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (ITEM_HEIGHT - offset) / 2;
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER + fixedOffset, pix);

    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int v = 1;
    offset = 3 * ITEM_BORDER;
    //
    const QString &title = QString("%1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 6 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + (v + 1) * lineHeight;
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    offset += v;
    painter->drawText(3 * ITEM_BORDER, offset, ITEM_WIDTH - 6 * ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, title);
    //
    for(int i = 0; i < m_data.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), m_data[i])/(ITEM_WIDTH - 6 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        if(offset + v >= pix.height())
        {
            break;
        }
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(3 * ITEM_BORDER, offset + fixedOffset, ITEM_WIDTH - 6 * ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme16(QPainter *painter)
{
    QStringList splData;
    for(QString var : qAsConst(m_data))
    {
        QString afVar;
        var.replace(TTK_SPACE, " / ");
        for(int i = 0; i < var.length(); ++i)
        {
            const QChar &ch = var.at(i);
            afVar.append(ch);
            if(TTK::String::isChinese(ch))
            {
                afVar.append(" / ");
            }
        }
        splData << afVar;
    }
    //
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2 * ITEM_BORDER, ITEM_WIDTH - 2 * ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (ITEM_HEIGHT - offset) / 2;
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER + fixedOffset, pix);

    const int lineHeight = TTK::Widget::fontTextHeight(font());
    int v = 1;
    offset = 3 * ITEM_BORDER;
    //
    const QString &title = QString("%1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    v = TTK::Widget::fontTextWidth(font(), title)/(ITEM_WIDTH - 6 * ITEM_BORDER) + 1;
    v = 3 * ITEM_BORDER + (v + 1) * lineHeight;
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    offset += v;
    painter->drawText(3 * ITEM_BORDER, offset, ITEM_WIDTH - 6 * ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, title);
    //
    for(int i = 0; i < splData.count(); ++i)
    {
        v = TTK::Widget::fontTextWidth(font(), splData[i])/(ITEM_WIDTH - 6 * ITEM_BORDER) + 1;
        v = 3 * ITEM_BORDER + v * lineHeight;
        if(offset + v >= pix.height())
        {
            break;
        }
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(3 * ITEM_BORDER, offset + fixedOffset, ITEM_WIDTH - 6 * ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, splData[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme17(QPainter *painter)
{
    const int lineHeight = TTK::Widget::fontTextHeight(font());
    const int delta = 5 * ITEM_BORDER + lineHeight;
    int offset = delta;
    TTKIntList list;
    //
    painter->translate(2 * ITEM_BORDER, 0);
    painter->rotate(TTK_AN_90);
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("%1 ● %2").arg(TTK::generateSongArtist(m_title), TTK::generateSongTitle(m_title));

    list << TTK::Widget::fontTextWidth(font(), title);
    offset = ITEM_WIDTH - delta;
    painter->drawText(12 * ITEM_BORDER, lineHeight - offset, title);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    for(int i = 0; i < m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta * (i + 2);
        if(offset <= delta)
        {
            break;
        }

        list << TTK::Widget::fontTextWidth(font(), m_data[i]);
        painter->drawText(12 * ITEM_BORDER, lineHeight - offset, m_data[i]);
    }
    std::sort(list.begin(), list.end());
    painter->rotate(-TTK_AN_90);
    painter->translate(-2 * ITEM_BORDER, 0);
    //
    offset = list.back() + 2 * (5 + 7) * ITEM_BORDER;
    offset = qMax(offset, ITEM_HEIGHT);
    setFixedHeight(offset);
    //
    painter->setPen(QPen(QColor(0xFF, 0, 0), ITEM_BORDER));
    painter->drawRect(5 * ITEM_BORDER, 5 * ITEM_BORDER, ITEM_WIDTH - 10 * ITEM_BORDER, offset - 10 * ITEM_BORDER);
    painter->setPen(QColor(0xFF, 0, 0));
    painter->drawRect(7 * ITEM_BORDER, 7 * ITEM_BORDER, ITEM_WIDTH - 14 * ITEM_BORDER, offset - 14 * ITEM_BORDER);

    int ofs = ITEM_WIDTH, i = 0;
    while(ofs > 2 * delta)
    {
        ++i;
        ofs = ITEM_WIDTH - 5 * ITEM_BORDER - i * delta;
        painter->drawLine(ofs, 7 * ITEM_BORDER, ofs, offset - 7 * ITEM_BORDER);
    }
}



MusicLrcPosterTableWidget::MusicLrcPosterTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setColumnCount(2);

    QHeaderView *headerView = horizontalHeader();
    headerView->resizeSection(0, 30);
    headerView->resizeSection(1, 237);

    verticalScrollBar()->setStyleSheet(TTK::UI::ScrollBarStyle01);
}

void MusicLrcPosterTableWidget::addCellItems(const QStringList &lrcs)
{
    setRowCount(lrcs.count());

    for(int i = 0; i < lrcs.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(TTK_CHECKED_ROLE, Qt::Unchecked);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(lrcs[i]);
        QtItemSetTextAlignment(item, Qt::AlignCenter);
        setItem(i, 1, item);
    }
}

void MusicLrcPosterTableWidget::itemCellClicked(int row, int column)
{
    MusicFillItemTableWidget::itemCellClicked(row, column);

    QStringList data;
    for(int i = 0; i < rowCount(); ++i)
    {
        if(TTKStaticCast(Qt::CheckState, item(i, 0)->data(TTK_CHECKED_ROLE).toInt()) == Qt::Checked)
        {
            data << item(i, 1)->text();
        }
    }

    Q_EMIT textChanged(data);
}



MusicLrcPosterThemeListWidget::MusicLrcPosterThemeListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setIconSize(QSize(40, 40));
    setFrameShape(QFrame::NoFrame);
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setFlow(QListView::TopToBottom);
    setFocusPolicy(Qt::ClickFocus);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet(TTK::UI::ListWidgetStyle02);

    addCelltItems();
}

void MusicLrcPosterThemeListWidget::addCelltItems()
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb1") ,tr("Default"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb2") ,tr("Plain"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb3") ,tr("Classical"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb4") ,tr("Quiet"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb5") ,tr("Peaceful"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb6") ,tr("Bright"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb7") ,tr("Notepad"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb8") ,tr("Letter"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb9") ,tr("Envelope"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb10") ,tr("Photo"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb11") ,tr("Spring"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb12") ,tr("Summer"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb13") ,tr("Autumn"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb14") ,tr("Winter"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb15") ,tr("Compile"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb16") ,tr("Slender"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

                     item = new QListWidgetItem(QIcon(":/lrc/lb_poster_thumb17") ,tr("Red"), this);
    item->setSizeHint(QSize(70, 80));
    addItem(item);

    setFixedWidth(70 * count());
}



MusicLrcPosterWidget::MusicLrcPosterWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcPosterWidget)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_itemWidget = new MusicLrcPosterItemWidget(this);
    TTK::Widget::generateVScrollAreaStyle(m_ui->viewArea, m_itemWidget);
    connect(m_ui->lrcArea, SIGNAL(textChanged(QStringList)), m_itemWidget, SLOT(textChanged(QStringList)));

    m_themeWidget = new MusicLrcPosterThemeListWidget(this);
    TTK::Widget::generateHScrollAreaStyle(m_ui->listArea, m_themeWidget);

    m_ui->openButton->setStyleSheet(TTK::UI::PushButtonStyle04);
    m_ui->saveButton->setStyleSheet(TTK::UI::PushButtonStyle04);
#ifdef Q_OS_UNIX
    m_ui->openButton->setFocusPolicy(Qt::NoFocus);
    m_ui->saveButton->setFocusPolicy(Qt::NoFocus);
#endif
    connect(m_ui->openButton, SIGNAL(clicked()), SLOT(openButtonClicked()));
    connect(m_ui->saveButton, SIGNAL(clicked()), SLOT(saveButtonClicked()));
    connect(m_themeWidget, SIGNAL(currentRowChanged(int)), m_itemWidget, SLOT(currentTypeChanged(int)));
}

MusicLrcPosterWidget::~MusicLrcPosterWidget()
{
    delete m_itemWidget;
    delete m_themeWidget;
    delete m_ui;
}

void MusicLrcPosterWidget::setCurrentLrcs(const QStringList &lrcs, const QString &name)
{
    m_itemWidget->setCurrentSongName(name);
    m_ui->lrcArea->addCellItems(lrcs);
}

void MusicLrcPosterWidget::openButtonClicked()
{
    const QString &path = TTK::File::getOpenFileName(this);
    if(path.isEmpty())
    {
        return;
    }

    m_itemWidget->setImagePath(path);
}

void MusicLrcPosterWidget::saveButtonClicked()
{
    const QString &path = TTK::File::getSaveFileName(this, "Jpeg Files (*.jpg)");
    if(!path.isEmpty())
    {
        QRect rect = m_itemWidget->rect();
        if(m_itemWidget->hasScroll())
        {
            rect.setWidth(rect.width() + ITEM_SCROLL_WIDTH);
        }

        TTKDesktopWrapper::grabWidget(m_itemWidget, rect).save(path, JPG_FILE_SUFFIX);
    }
}
