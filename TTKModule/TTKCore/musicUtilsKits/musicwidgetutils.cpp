#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "musicimageutils.h"
#include "ttkdesktopwrapper.h"

#include <QBitmap>
#include <QScrollArea>
#include <QStyledItemDelegate>

void MusicUtils::Widget::setLabelFontSize(QWidget *widget, int size)
{
    QFont font = widget->font();
    font.setPointSize(size);
    widget->setFont(font);
}

void MusicUtils::Widget::setLabelFontStyle(QWidget *widget, MusicObject::FontStyleMode type)
{
    QFont font = widget->font();
    switch(type)
    {
        case MusicObject::FontStyleMode::Bold: font.setBold(true); break;
        case MusicObject::FontStyleMode::Italic: font.setItalic(true); break;
        case MusicObject::FontStyleMode::Underline: font.setUnderline(true); break;
        case MusicObject::FontStyleMode::Overline: font.setOverline(true); break;
        case MusicObject::FontStyleMode::StrikeOut: font.setStrikeOut(true); break;
        case MusicObject::FontStyleMode::FixedPitch: font.setFixedPitch(true); break;
        case MusicObject::FontStyleMode::Kerningt: font.setKerning(true); break;
        default: break;
    }
    widget->setFont(font);
}

QString MusicUtils::Widget::elidedText(const QFont &font, const QString &text, Qt::TextElideMode mode, int width)
{
    const QFontMetrics ftm(font);
    return ftm.elidedText(text, mode, width);
}

int MusicUtils::Widget::fontTextWidth(const QFont &font, const QString &text)
{
    const QFontMetrics ftm(font);
    return QtFontWidth(ftm, text);
}

int MusicUtils::Widget::fontTextHeight(const QFont &font)
{
    const QFontMetrics ftm(font);
    return ftm.height();
}

void MusicUtils::Widget::adjustMenuPosition(QMenu *menu)
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

void MusicUtils::Widget::positionInCenter(QWidget *widget)
{
    const QRect &rect = TTKDesktopWrapper::screenGeometry();
    widget->move((rect.width() - widget->width()) / 2, (rect.height() - widget->height()) / 2);
}

void MusicUtils::Widget::setTransparent(QWidget *widget, int alpha)
{
    QPalette plt(widget->palette());
    plt.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, alpha)));
    widget->setPalette(plt);
}

void MusicUtils::Widget::setTransparent(QWidget *widget, const QColor &color)
{
    QPalette plt(widget->palette());
    plt.setBrush(QPalette::Highlight, QBrush(color));
    widget->setPalette(plt);
}

void MusicUtils::Widget::setComboBoxText(QComboBox *object, const QString &text)
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

void MusicUtils::Widget::widgetToRound(QWidget *widget, int ratioX, int ratioY)
{
    widget->setMask(MusicUtils::Image::generateMask(widget->rect(), ratioX, ratioY));
}

void MusicUtils::Widget::generateVScrollAreaFormat(QWidget *widget, QWidget *parent, bool background)
{
    QScrollArea *area = TTKObject_cast(QScrollArea*, widget);
    if(area)
    {
        const QString &style = background ? MusicUIObject::MQSSScrollBarStyle01 : MusicUIObject::MQSSScrollBarStyle03;
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

void MusicUtils::Widget::generateHScrollAreaFormat(QWidget *widget, QWidget *parent, bool background)
{
    QScrollArea *area = TTKObject_cast(QScrollArea*, widget);
    if(area)
    {
        const QString &style = background ? MusicUIObject::MQSSScrollBarStyle02 : MusicUIObject::MQSSScrollBarStyle04;
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

void MusicUtils::Widget::generateComboBoxFormat(QComboBox *widget, const QString &style)
{
    widget->setItemDelegate(new QStyledItemDelegate(widget));
    widget->setStyleSheet(style.isEmpty() ? MusicUIObject::MQSSComboBoxStyle01 + MusicUIObject::MQSSItemView01 : style);
    widget->view()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle01);
}
