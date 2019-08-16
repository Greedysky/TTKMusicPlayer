#include "musiclrcposterwidget.h"
#include "ui_musiclrcposterwidget.h"
#include "musicitemdelegate.h"
#include "musicbackgroundmanager.h"
#include "musicstringutils.h"
#include "musicuiobject.h"
#include "musicfileutils.h"
#include "musicextractwrap.h"
#include "musicbackgroundconfigmanager.h"

#include "qimage/qimagewrap.h"

#include <qmath.h>
#include <QPainter>

#define ITEM_SCROLL_WIDTH   8
#define ITEM_BORDER         3
#define ITEM_WIDTH          275
#define ITEM_HEIGHT         330

MusicLrcPosterItemWidget::MusicLrcPosterItemWidget(QWidget *parent)
    : QWidget(parent)
{
    m_type = Type_01;

    m_pixmap.load(M_BACKGROUND_PTR->getArtistPhotoPathNoIndex());
    if(m_pixmap.isNull())
    {
        MusicBackgroundImage image;
        MusicExtractWrap::outputSkin(&image, M_BACKGROUND_PTR->getBackgroundUrl());
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
    m_type = MStatic_cast(Type, type);
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
        case Type_01: drawTheme1(&painter); break;
        case Type_02: drawTheme2(&painter); break;
        case Type_03: drawTheme3(&painter); break;
        case Type_04: drawTheme4(&painter); break;
        case Type_05: drawTheme5(&painter); break;
        case Type_06: drawTheme6(&painter); break;
        case Type_07: drawTheme7(&painter); break;
        case Type_08: drawTheme8(&painter); break;
        case Type_09: drawTheme9(&painter); break;
        case Type_10: drawTheme10(&painter); break;
        case Type_11: drawTheme11(&painter); break;
        case Type_12: drawTheme12(&painter); break;
        case Type_13: drawTheme13(&painter); break;
        case Type_14: drawTheme14(&painter); break;
        case Type_15: drawTheme15(&painter); break;
        case Type_16: drawTheme16(&painter); break;
        case Type_17: drawTheme17(&painter); break;
        default: break;
    }
}

void MusicLrcPosterItemWidget::drawTheme1(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);

    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = pix.height() + 5*ITEM_BORDER;
    int v = 1;
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 4*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(2*ITEM_BORDER, offset, ITEM_WIDTH - 4*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 4*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + v*lineHeight;
    painter->drawText(2*ITEM_BORDER, offset, ITEM_WIDTH - 4*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    offset += v;
    //
    offset += 3*ITEM_BORDER;
    painter->drawText(2*ITEM_BORDER, offset, ITEM_WIDTH - 4*ITEM_BORDER, lineHeight, Qt::AlignCenter, tr("TTKMusicPlayer"));
    offset += 3*ITEM_BORDER;
    //
    setFixedHeight(offset >= ITEM_HEIGHT ? offset + 2*lineHeight : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme2(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 5*ITEM_BORDER + lineHeight;
    int v = 1;
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        offset += v + 3*ITEM_BORDER + v*lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    //
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + v*lineHeight;
    offset += v + 10*ITEM_BORDER;
    if(offset < ITEM_HEIGHT)
    {
        offset = ITEM_HEIGHT;
    }
    painter->drawText(8*ITEM_BORDER, offset - v - 10*ITEM_BORDER, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QPen(QColor(0xBB, 0xBB, 0xBB), ITEM_BORDER));
    painter->drawRect(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, offset - 10*ITEM_BORDER);
    setFixedHeight(offset);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    offset = 8*ITEM_BORDER;
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme3(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);

    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    const int delta = 5*ITEM_BORDER + lineHeight;
    int offset = 0;
    MIntList list;
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    painter->translate(2*ITEM_BORDER, pix.height());
    painter->rotate(MA_90);
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    list << fm.width(title);
    painter->drawText(8*ITEM_BORDER, 0, title);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta*(i+1);
        if(offset <= delta)
        {
            break;
        }
        list << fm.width(m_data[i]);
        painter->drawText(8*ITEM_BORDER, -offset, m_data[i]);
    }
    qSort(list);
    //
    offset = pix.height() + list.last() + 10*ITEM_BORDER;
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme4(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    const int delta = 5*ITEM_BORDER + lineHeight;
    int offset = ITEM_WIDTH - delta;
    MIntList list;
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    painter->translate(2*ITEM_BORDER, 0);
    painter->rotate(MA_90);
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta*(i+1);
        if(offset <= delta || offset <=  2*delta)
        {
            break;
        }
        list << fm.width(m_data[i]);
        painter->drawText(3*ITEM_BORDER, -offset, m_data[i]);
    }
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    list << fm.width(title);
    if(!m_data.isEmpty())
    {
        offset -= delta;
    }
    painter->drawText(3*ITEM_BORDER, -offset, title);
    //
    qSort(list);
    painter->rotate(-MA_90);
    painter->translate(-2*ITEM_BORDER, 0);

    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    offset = list.last() + delta - 2*ITEM_BORDER;
    painter->drawPixmap(ITEM_BORDER, offset, pix);
    offset += ITEM_BORDER + pix.height();
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme5(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 5*ITEM_BORDER;
    int v = 1;
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 4*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(2*ITEM_BORDER, offset, ITEM_WIDTH - 4*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 4*ITEM_BORDER) + 1;
    painter->drawText(2*ITEM_BORDER, offset, ITEM_WIDTH - 4*ITEM_BORDER, v*lineHeight, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    offset += 3*ITEM_BORDER + lineHeight;
    painter->drawPixmap(ITEM_BORDER, offset, pix);
    offset += ITEM_BORDER + pix.height();
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme6(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (pix.width() - offset)/2;
    pix = MusicUtils::Widget::pixmapToRound(pix, QRect(0, 0, offset, offset), offset, offset);
    painter->drawPixmap(ITEM_BORDER + fixedOffset, ITEM_BORDER, pix);

    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    const int delta = 5*ITEM_BORDER + lineHeight;
    offset = ITEM_WIDTH - delta;
    MIntList list;
    //
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    painter->translate(2*ITEM_BORDER, pix.height());
    painter->rotate(MA_90);
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    list << fm.width(title);
    painter->drawText(8*ITEM_BORDER, -offset, title);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta*(i+2);
        if(offset <= delta)
        {
            break;
        }
        list << fm.width(m_data[i]);
        painter->drawText(8*ITEM_BORDER, -offset, m_data[i]);
    }
    qSort(list);
    //
    offset = pix.height() + list.last() + 10*ITEM_BORDER;
    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme7(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    const int delta = 5*ITEM_BORDER + lineHeight;
    int offset = 8*ITEM_BORDER;
    int v = 1;
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    painter->drawLine(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 5*ITEM_BORDER, 5*ITEM_BORDER);
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        v = 5*ITEM_BORDER + v*lineHeight;
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;

        painter->setPen(QColor(0xBB, 0xBB, 0xBB));
        painter->drawLine(5*ITEM_BORDER, offset - 3*ITEM_BORDER, ITEM_WIDTH - 5*ITEM_BORDER, offset - 3*ITEM_BORDER);
    }
    //
    offset += delta;
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 10*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + v*lineHeight;
    painter->drawText(5*ITEM_BORDER, offset - v, ITEM_WIDTH - 10*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    offset += v;

    setFixedHeight(offset >= ITEM_HEIGHT ? offset : ITEM_HEIGHT);
}

void MusicLrcPosterItemWidget::drawTheme8(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 5*ITEM_BORDER;
    int v = 1;
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 10*ITEM_BORDER) + 1;
        offset += v + 3*ITEM_BORDER + v*lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    //
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 10*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + v*lineHeight;
    offset += v + 10*ITEM_BORDER;
    if(offset < ITEM_HEIGHT)
    {
        offset = ITEM_HEIGHT;
    }
    painter->drawText(5*ITEM_BORDER, offset - v - 10*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->translate(0, 0);
    QPixmap borer(":/lrc/lb_poster_letter_line");
    for(int i=0; i<=ceil(ITEM_WIDTH/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width()*i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width()*i, offset - borer.height(), borer.width(), borer.height(), borer);
    }
    painter->translate(borer.height(), 0);
    painter->rotate(MA_90);
    for(int i=0; i<=ceil(offset/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width()*i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width()*i, borer.height() - ITEM_WIDTH, borer.width(), borer.height(), borer);
    }
    painter->rotate(-MA_90);
    painter->translate(0, 0);
    //
    setFixedHeight(offset);
    //
    offset = 5*ITEM_BORDER;
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 10*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(5*ITEM_BORDER, offset, ITEM_WIDTH - 10*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;

        painter->setPen(QColor(0xBB, 0xBB, 0xBB));
        painter->drawLine(5*ITEM_BORDER, offset - ITEM_BORDER, ITEM_WIDTH - 5*ITEM_BORDER, offset - ITEM_BORDER);
    }
}

void MusicLrcPosterItemWidget::drawTheme9(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 5*ITEM_BORDER;
    int v = 1;
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 10*ITEM_BORDER) + 1;
        offset += v + 3*ITEM_BORDER + v*lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    //
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 10*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + v*lineHeight;
    offset += v + 10*ITEM_BORDER;
    if(offset < ITEM_HEIGHT)
    {
        offset = ITEM_HEIGHT;
    }
    painter->drawText(5*ITEM_BORDER, offset - v - 10*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->translate(0, 0);
    QPixmap borer(":/lrc/lb_poster_letter_line");
    for(int i=0; i<=ceil(ITEM_WIDTH/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width()*i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width()*i, offset - borer.height(), borer.width(), borer.height(), borer);
    }
    painter->translate(borer.height(), 0);
    painter->rotate(MA_90);
    for(int i=0; i<=ceil(offset/borer.width()); ++i)
    {
        painter->drawPixmap(borer.width()*i, 0, borer.width(), borer.height(), borer);
        painter->drawPixmap(borer.width()*i, borer.height() - ITEM_WIDTH, borer.width(), borer.height(), borer);
    }
    painter->rotate(-MA_90);
    painter->translate(0, 0);
    //
    QPixmap background(":/lrc/lb_poster_letter_background");
    painter->drawPixmap(ITEM_WIDTH - background.width() - 2*ITEM_BORDER, background.height(), background.width(), background.height(), background);
    //
    setFixedHeight(offset);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    //
    offset = 5*ITEM_BORDER;
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 10*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(5*ITEM_BORDER, offset, ITEM_WIDTH - 10*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme10(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (ITEM_HEIGHT - offset)/2;
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER + fixedOffset, pix);

    const int gWidth = pix.width()*3/4;
    const int gHeight = pix.height()*3/4;
    QRect gaussRect = QRect((pix.width() - gWidth)/2, (pix.height() - gHeight)/2, gWidth, gHeight);
    const QImage& gauss = pix.copy(gaussRect).toImage();
    QImageWrap().GaussFilter(gauss);

    gaussRect = QRect((pix.width() - gWidth)/2, (pix.height() - gHeight)/2 + fixedOffset, gWidth, gHeight);
    painter->drawPixmap(gaussRect, QPixmap::fromImage(gauss));
    //
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int v = 1;
    offset = 3*ITEM_BORDER;

    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(gaussRect.width() - 6*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        if(offset + v >= gauss.height())
        {
            break;
        }
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(3*ITEM_BORDER + gaussRect.x(), offset + gaussRect.y(), gaussRect.width() - 6*ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme11(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 8*ITEM_BORDER;
    int v = 1;
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        offset += v + 3*ITEM_BORDER + v*lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + (v + 1)*lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_spring");
    int h = pix.height()*width()*1.0f/pix.width();
    int w = width();

    if(h <= height())
    {
        w = width()*qMax(1.0f, height()*1.0f/pix.height());
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2*ITEM_BORDER, h - 2*ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, height() - 10*ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8*ITEM_BORDER;
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme12(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 8*ITEM_BORDER;
    int v = 1;
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        offset += v + 3*ITEM_BORDER + v*lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + (v + 1)*lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_summer");
    int h = pix.height()*width()*1.0f/pix.width();
    int w = width();

    if(h <= height())
    {
        w = width()*qMax(1.0f, height()*1.0f/pix.height());
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2*ITEM_BORDER, h - 2*ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, height() - 10*ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8*ITEM_BORDER;
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme13(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 8*ITEM_BORDER;
    int v = 1;
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        offset += v + 3*ITEM_BORDER + v*lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + (v + 1)*lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_autumn");
    int h = pix.height()*width()*1.0f/pix.width();
    int w = width();

    if(h <= height())
    {
        w = width()*qMax(1.0f, height()*1.0f/pix.height());
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2*ITEM_BORDER, h - 2*ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, height() - 10*ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8*ITEM_BORDER;
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme14(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int offset = 8*ITEM_BORDER;
    int v = 1;
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        offset += v + 3*ITEM_BORDER + v*lineHeight;
    }
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("--- %1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + (v + 1)*lineHeight;
    setFixedHeight(qMax(offset + v, ITEM_HEIGHT));
    //
    QPixmap pix(":/lrc/lb_poster_winter");
    int h = pix.height()*width()*1.0f/pix.width();
    int w = width();

    if(h <= height())
    {
        w = width()*qMax(1.0f, height()*1.0f/pix.height());
        h = height();
    }

    if(hasScroll())
    {
        w += ITEM_SCROLL_WIDTH;
    }

    pix = pix.scaled(w - 2*ITEM_BORDER, h - 2*ITEM_BORDER);
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER, pix);
    painter->fillRect(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, height() - 10*ITEM_BORDER, QColor(255, 255, 255, 200));
    //
    painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::AlignRight | Qt::TextWordWrap, title);
    //
    painter->setPen(QColor(0x66, 0x66, 0x66));
    offset = 8*ITEM_BORDER;
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 16*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        painter->drawText(8*ITEM_BORDER, offset, ITEM_WIDTH - 16*ITEM_BORDER, v, Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme15(QPainter *painter)
{
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (ITEM_HEIGHT - offset)/2;
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER + fixedOffset, pix);

    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int v = 1;
    offset = 3*ITEM_BORDER;
    //
    const QString &title = QString("%1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 6*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + (v + 1)*lineHeight;
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    offset += v;
    painter->drawText(3*ITEM_BORDER, offset, ITEM_WIDTH - 6*ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, title);
    //
    for(int i=0; i<m_data.count(); ++i)
    {
        v = fm.width(m_data[i])/(ITEM_WIDTH - 6*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        if(offset + v >= pix.height())
        {
            break;
        }
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(3*ITEM_BORDER, offset + fixedOffset, ITEM_WIDTH - 6*ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, m_data[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme16(QPainter *painter)
{
    QStringList splData;
    foreach(QString var, m_data)
    {
        QString afVar;
        var.replace(" ", " / ");
        for(int i=0; i<var.length(); ++i)
        {
            const QChar &ch = var.at(i);
            afVar.append(ch);
            if(MusicUtils::String::isChinese(ch))
            {
                afVar.append(" / ");
            }
        }
        splData << afVar;
    }
    //
    QPixmap pix(m_pixmap);
    pix = pix.scaled(ITEM_WIDTH - 2*ITEM_BORDER, ITEM_WIDTH - 2*ITEM_BORDER, Qt::KeepAspectRatio);
    int offset = qMin(pix.width(), pix.height());
    const int fixedOffset = (ITEM_HEIGHT - offset)/2;
    painter->drawPixmap(ITEM_BORDER, ITEM_BORDER + fixedOffset, pix);

    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    int v = 1;
    offset = 3*ITEM_BORDER;
    //
    const QString &title = QString("%1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    v = fm.width(title)/(ITEM_WIDTH - 6*ITEM_BORDER) + 1;
    v = 3*ITEM_BORDER + (v + 1)*lineHeight;
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    offset += v;
    painter->drawText(3*ITEM_BORDER, offset, ITEM_WIDTH - 6*ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, title);
    //
    for(int i=0; i<splData.count(); ++i)
    {
        v = fm.width(splData[i])/(ITEM_WIDTH - 6*ITEM_BORDER) + 1;
        v = 3*ITEM_BORDER + v*lineHeight;
        if(offset + v >= pix.height())
        {
            break;
        }
        painter->setPen(QColor(0x66, 0x66, 0x66));
        painter->drawText(3*ITEM_BORDER, offset + fixedOffset, ITEM_WIDTH - 6*ITEM_BORDER, v, Qt::AlignHCenter | Qt::TextWordWrap, splData[i]);
        offset += v;
    }
}

void MusicLrcPosterItemWidget::drawTheme17(QPainter *painter)
{
    QFontMetrics fm(font());
    const int lineHeight = fm.height();
    const int delta = 5*ITEM_BORDER + lineHeight;
    int offset = delta;
    MIntList list;
    //
    painter->translate(2*ITEM_BORDER, 0);
    painter->rotate(MA_90);
    painter->setPen(QColor(0xBB, 0xBB, 0xBB));
    const QString &title = QString("%1 ● %2").arg(MusicUtils::String::artistName(m_title)).arg(MusicUtils::String::songName(m_title));

    list << fm.width(title);
    offset = ITEM_WIDTH - delta;
    painter->drawText(12*ITEM_BORDER, lineHeight - offset, title);
    painter->setPen(QColor(0x66, 0x66, 0x66));
    for(int i=0; i<m_data.count(); ++i)
    {
        offset = ITEM_WIDTH - delta*(i+2);
        if(offset <= delta)
        {
            break;
        }
        list << fm.width(m_data[i]);
        painter->drawText(12*ITEM_BORDER, lineHeight - offset, m_data[i]);
    }
    qSort(list);
    painter->rotate(-MA_90);
    painter->translate(-2*ITEM_BORDER, 0);
    //
    offset = list.last() + 2*(5+7)*ITEM_BORDER;
    offset = qMax(offset, ITEM_HEIGHT);
    setFixedHeight(offset);
    //
    painter->setPen(QPen(QColor(0xFF, 0, 0), ITEM_BORDER));
    painter->drawRect(5*ITEM_BORDER, 5*ITEM_BORDER, ITEM_WIDTH - 10*ITEM_BORDER, offset - 10*ITEM_BORDER);
    painter->setPen(QColor(0xFF, 0, 0));
    painter->drawRect(7*ITEM_BORDER, 7*ITEM_BORDER, ITEM_WIDTH - 14*ITEM_BORDER, offset - 14*ITEM_BORDER);

    int ofs = ITEM_WIDTH, i = 0;
    while(ofs > 2*delta)
    {
        ++i;
        ofs = ITEM_WIDTH - 5*ITEM_BORDER - i*delta;
        painter->drawLine(ofs, 7*ITEM_BORDER, ofs, offset - 7*ITEM_BORDER);
    }
}



MusicLrcPosterTableWidget::MusicLrcPosterTableWidget(QWidget *parent)
    : MusicFillItemTableWidget(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);

    setColumnCount(2);
    QHeaderView *headerview = horizontalHeader();
    headerview->resizeSection(0, 30);
    headerview->resizeSection(1, 237);
}

void MusicLrcPosterTableWidget::createAllItems(const QStringList &lrcs)
{
    setRowCount(lrcs.count());
    for(int i=0; i<lrcs.count(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(MUSIC_CHECK_ROLE, false);
        setItem(i, 0, item);

                          item = new QTableWidgetItem;
        item->setText(lrcs[i]);
        item->setTextAlignment(Qt::AlignCenter);
        setItem(i, 1, item);
    }
}

void MusicLrcPosterTableWidget::listCellClicked(int row, int column)
{
    MusicFillItemTableWidget::listCellClicked(row, column);

    QStringList data;
    for(int i=0; i<rowCount(); ++i)
    {
        if(item(i, 0)->data(MUSIC_CHECK_ROLE).toBool())
        {
            data << item(i, 1)->text();
        }
    }

    emit textChanged(data);
}



MusicLrcPosterThemeListWidget::MusicLrcPosterThemeListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setIconSize(QSize(40, 40));
    setFrameShape(QFrame::NoFrame);
    setViewMode(QListView::IconMode);
    setMovement(QListView::Static);
    setFlow(QListView::TopToBottom);
    setSelectionMode(QListWidget::SingleSelection);
    setFocusPolicy(Qt::ClickFocus);
    setHorizontalScrollMode(QListWidget::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    addListWidgetItem();
}

void MusicLrcPosterThemeListWidget::addListWidgetItem()
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

    setFixedWidth(70*count());
}



MusicLrcPosterWidget::MusicLrcPosterWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicLrcPosterWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_itemWidget = new MusicLrcPosterItemWidget(this);
    m_ui->viewArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->viewArea->setWidgetResizable(true);
    m_ui->viewArea->setFrameShape(QFrame::NoFrame);
    m_ui->viewArea->setFrameShadow(QFrame::Plain);
    m_ui->viewArea->setAlignment(Qt::AlignVCenter);
    m_ui->viewArea->setWidget(m_itemWidget);
    m_ui->viewArea->verticalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle03);
    connect(m_ui->lrcArea, SIGNAL(textChanged(QStringList)), m_itemWidget, SLOT(textChanged(QStringList)));

    m_themeWidget = new MusicLrcPosterThemeListWidget(this);
    m_ui->listArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->listArea->setWidgetResizable(true);
    m_ui->listArea->setFrameShape(QFrame::NoFrame);
    m_ui->listArea->setFrameShadow(QFrame::Plain);
    m_ui->listArea->setAlignment(Qt::AlignVCenter);
    m_ui->listArea->setWidget(m_themeWidget);
    m_ui->listArea->horizontalScrollBar()->setStyleSheet(MusicUIObject::MScrollBarStyle04);

    m_ui->openButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    m_ui->saveButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
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
    m_ui->lrcArea->createAllItems(lrcs);
}

void MusicLrcPosterWidget::openButtonClicked()
{
    const QString &picPath = MusicUtils::File::getOpenFileDialog(this);
    if(picPath.isEmpty())
    {
        return;
    }

    m_itemWidget->setImagePath(picPath);
}

void MusicLrcPosterWidget::saveButtonClicked()
{
    const QString &filename = MusicUtils::File::getSaveFileDialog(this, "Jpeg(*.jpg)");
    if(!filename.isEmpty())
    {
        QRect rect = m_itemWidget->rect();
        if(m_itemWidget->hasScroll())
        {
            rect.setWidth(rect.width() + ITEM_SCROLL_WIDTH);
        }
#ifdef TTK_GREATER_NEW
        m_itemWidget->grab(rect).save(filename, JPG_FILE_PREFIX);
#else
        QPixmap::grabWidget(this, rect).save(filename, JPG_FILE_PREFIX);
#endif
    }
}

int MusicLrcPosterWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
