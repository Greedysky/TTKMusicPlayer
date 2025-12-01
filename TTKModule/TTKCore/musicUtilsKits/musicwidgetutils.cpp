#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "musicimageutils.h"
#include "ttkdesktopscreen.h"

#include <QBitmap>
#include <QScrollArea>
#include <QStyledItemDelegate>

void TTK::Widget::setFontSize(QWidget *widget, int size)
{
    QFont font = widget->font();
    font.setPointSize(size);
    widget->setFont(font);
}

void TTK::Widget::setFontStyle(QWidget *widget, TTK::FontStyleMode type)
{
    QFont font = widget->font();
    switch(type)
    {
        case TTK::FontStyleMode::Bold: font.setBold(true); break;
        case TTK::FontStyleMode::Italic: font.setItalic(true); break;
        case TTK::FontStyleMode::Underline: font.setUnderline(true); break;
        case TTK::FontStyleMode::Overline: font.setOverline(true); break;
        case TTK::FontStyleMode::StrikeOut: font.setStrikeOut(true); break;
        case TTK::FontStyleMode::FixedPitch: font.setFixedPitch(true); break;
        case TTK::FontStyleMode::Kerningt: font.setKerning(true); break;
        default: break;
    }
    widget->setFont(font);
}

QString TTK::Widget::elidedText(const QFont &font, const QString &text, Qt::TextElideMode mode, int width)
{
    const QFontMetrics ftm(font);
    return ftm.elidedText(text, mode, width);
}

QString TTK::Widget::elidedTitleText(const QFont &font, const QString &text, int width)
{
    const int index = text.lastIndexOf("[");
    const QString &prefix = text.left(index);
    const QString &suffix = text.right(text.length() - index);
    const int offset = TTK::Widget::fontTextWidth(font, suffix);
    return TTK::Widget::elidedText(font, prefix, Qt::ElideRight, width - offset) + suffix;
}

int TTK::Widget::fontTextWidth(const QFont &font, const QString &text)
{
    const QFontMetrics ftm(font);
    return QtFontWidth(ftm, text);
}

int TTK::Widget::fontTextHeight(const QFont &font)
{
    const QFontMetrics ftm(font);
    return ftm.height();
}

void TTK::Widget::adjustMenuPosition(QMenu *menu)
{
#if TTK_QT_VERSION_CHECK(5,12,0)
    const QList<QAction*> actions(menu->actions());
    if(!actions.empty())
    {
        QAction* action(actions.first());
        if(action->icon().isNull())
        {
            action->setIcon(QIcon(":/tiny/lb_transparent"));
        }
    }
#else
    Q_UNUSED(menu);
#endif
}

void TTK::Widget::adjustWidgetPosition(QWidget *widget)
{
    const QRect &rect = TTKDesktopScreen::currentGeometry();
    widget->move(rect.x() + (rect.width() - widget->width()) / 2, rect.y() + (rect.height() - widget->height()) / 2);
}

void TTK::Widget::setTransparent(QWidget *widget, int alpha)
{
    QPalette plt(widget->palette());
    plt.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, alpha)));
    widget->setPalette(plt);
}

void TTK::Widget::setTransparent(QWidget *widget, const QColor &color)
{
    QPalette plt(widget->palette());
    plt.setBrush(QPalette::Highlight, QBrush(color));
    widget->setPalette(plt);
}

void TTK::Widget::setComboBoxText(QComboBox *widget, const QString &text)
{
    if(widget->isEditable())
    {
        widget->setEditText(text);
    }
    else
    {
        const int index = widget->findText(text);
        if(index > -1)
        {
            widget->setCurrentIndex(index);
        }
    }
}

void TTK::Widget::widgetToRound(QWidget *widget, int ratioX, int ratioY)
{
    widget->setMask(TTK::Image::generateMask(widget->rect(), ratioX, ratioY));
}

void TTK::Widget::generateComboBoxStyle(QComboBox *widget, const QString &style)
{
    widget->setItemDelegate(new QStyledItemDelegate(widget));
    widget->setStyleSheet(style.isEmpty() ? TTK::UI::ComboBoxStyle01 + TTK::UI::ItemView01 : style);
    widget->view()->setStyleSheet(TTK::UI::ScrollBarStyle01);
}

void TTK::Widget::generateVScrollAreaStyle(QWidget *widget, QWidget *parent, bool background)
{
    QScrollArea *area = TTKObjectCast(QScrollArea*, widget);
    if(area)
    {
        const QString &style = background ? TTK::UI::ScrollBarStyle01 : TTK::UI::ScrollBarStyle03;
        area->setWidgetResizable(true);
        area->setFrameShape(QFrame::NoFrame);
        area->setFrameShadow(QFrame::Plain);
        area->setAlignment(Qt::AlignLeft);
        area->setWidget(parent);
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setStyleSheet(style);
        area->verticalScrollBar()->setStyleSheet(style);
    }
}

void TTK::Widget::generateHScrollAreaStyle(QWidget *widget, QWidget *parent, bool background)
{
    QScrollArea *area = TTKObjectCast(QScrollArea*, widget);
    if(area)
    {
        const QString &style = background ? TTK::UI::ScrollBarStyle02 : TTK::UI::ScrollBarStyle04;
        area->setWidgetResizable(true);
        area->setFrameShape(QFrame::NoFrame);
        area->setFrameShadow(QFrame::Plain);
        area->setAlignment(Qt::AlignLeft);
        area->setWidget(parent);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setStyleSheet(style);
        area->horizontalScrollBar()->setStyleSheet(style);
    }
}
