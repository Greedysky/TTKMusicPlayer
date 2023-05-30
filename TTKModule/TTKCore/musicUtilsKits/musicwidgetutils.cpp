#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "musicimageutils.h"
#include "ttkdesktopwrapper.h"

#include <QBitmap>
#include <QScrollArea>
#include <QStyledItemDelegate>

void TTK::Widget::setLabelFontSize(QWidget *widget, int size)
{
    QFont font = widget->font();
    font.setPointSize(size);
    widget->setFont(font);
}

void TTK::Widget::setLabelFontStyle(QWidget *widget, TTK::FontStyleMode type)
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
        QAction* action(actions.front());
        if(action->icon().isNull())
        {
            action->setIcon(QIcon(":/contextMenu/btn_transparent"));
        }
    }
#else
    Q_UNUSED(menu);
#endif
}

void TTK::Widget::adjustWidgetPosition(QWidget *widget)
{
    const QRect &rect = TTKDesktopWrapper::screenGeometry();
    widget->move((rect.width() - widget->width()) / 2, (rect.height() - widget->height()) / 2);
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

void TTK::Widget::setComboBoxText(QComboBox *object, const QString &text)
{
    if(object->isEditable())
    {
        object->setEditText(text);
    }
    else
    {
        const int i = object->findText(text);
        if(i > -1)
        {
            object->setCurrentIndex(i);
        }
    }
}

void TTK::Widget::widgetToRound(QWidget *widget, int ratioX, int ratioY)
{
    widget->setMask(TTK::Image::generateMask(widget->rect(), ratioX, ratioY));
}

void TTK::Widget::generateVScrollAreaFormat(QWidget *widget, QWidget *parent, bool background)
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

void TTK::Widget::generateHScrollAreaFormat(QWidget *widget, QWidget *parent, bool background)
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

void TTK::Widget::generateComboBoxFormat(QComboBox *widget, const QString &style)
{
    widget->setItemDelegate(new QStyledItemDelegate(widget));
    widget->setStyleSheet(style.isEmpty() ? TTK::UI::ComboBoxStyle01 + TTK::UI::ItemView01 : style);
    widget->view()->setStyleSheet(TTK::UI::ScrollBarStyle01);
}
