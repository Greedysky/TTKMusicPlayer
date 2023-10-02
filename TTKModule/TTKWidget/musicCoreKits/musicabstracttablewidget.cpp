#include "musicabstracttablewidget.h"

MusicAbstractTableWidget::MusicAbstractTableWidget(QWidget *parent)
    : TTKAbstractTableWidget(parent)
{
    setStyleSheet(TTK::UI::TableWidgetStyle01 + TTK::UI::ScrollBarStyle01 + TTK::UI::LineEditStyle01);

    TTK::Widget::setTransparent(this, 50);
#if defined Q_OS_UNIX && !TTK_QT_VERSION_CHECK(5,7,0) //Fix linux selection-background-color stylesheet bug
    TTK::Widget::setTransparent(this, QColor(20, 20, 20, 10));
#endif

}
