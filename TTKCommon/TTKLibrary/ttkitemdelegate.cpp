#include "ttkitemdelegate.h"

#include <QLabel>
#include <QPainter>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressBar>

TTKAbstractItemDelegate::TTKAbstractItemDelegate(QObject *parent)
    : QItemDelegate(parent),
      m_mode(TTKAbstractItemDelegate::Null),
      m_container(nullptr)
{

}

void TTKAbstractItemDelegate::setStyleSheet(const QString &style) const
{
    if(m_container)
    {
        m_container->setStyleSheet(style);
    }
}

QSize TTKAbstractItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}

void TTKAbstractItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if !TTK_QT_VERSION_CHECK(5,7,0)
    QItemDelegate::paint(painter, option, index);
#else
    drawBackground(painter, option, index);
#endif
}



TTKCheckBoxItemDelegate::TTKCheckBoxItemDelegate(QObject *parent)
    : TTKAbstractItemDelegate(parent),
      m_background(false)
{
    m_checkBox = new QCheckBox;
#ifdef Q_OS_UNIX
    m_checkBox->setFocusPolicy(Qt::NoFocus);
#endif
    m_container = m_checkBox;
}

TTKCheckBoxItemDelegate::~TTKCheckBoxItemDelegate()
{
    delete m_checkBox;
}

void TTKCheckBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    TTKAbstractItemDelegate::paint(painter, option, index);

    if((m_mode & TTKAbstractItemDelegate::TreeMode) && !index.parent().isValid())
    {
        drawDisplay(painter, option, option.rect, index.data(Qt::DisplayRole).toString());
        return;
    }

    painter->save();
    const bool displayMode = m_mode & TTKAbstractItemDelegate::DisplayMode;
    if(displayMode)
    {
        m_checkBox->resize(option.rect.size());
        m_checkBox->setText(index.data(TTK_DISPLAY_ROLE).toString());
        m_checkBox->setEnabled(index.data(TTK_ENABLED_ROLE).toBool());
    }
    else
    {
        const int minSize = qMin(option.rect.width(), option.rect.height());
        m_checkBox->resize(minSize, minSize);
    }

    const bool state = m_checkBox->isChecked();
    m_checkBox->setChecked(TTKStaticCast(Qt::CheckState, index.data(TTK_CHECKED_ROLE).toInt()) == Qt::Checked);
    if(m_checkBox->isEnabled() && state != m_checkBox->isChecked())
    {
        Q_EMIT TTKConstCast(TTKCheckBoxItemDelegate*, this)->buttonChecked();
    }
    painter->translate(displayMode ? 0 : (option.rect.width() - 16) / 2, 0);

    m_checkBox->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



TTKProgressBarItemDelegate::TTKProgressBarItemDelegate(QObject *parent)
    : TTKAbstractItemDelegate(parent)
{
    m_progress = new QProgressBar;
    m_container = m_progress;
}

TTKProgressBarItemDelegate::~TTKProgressBarItemDelegate()
{
    delete m_progress;
}

void TTKProgressBarItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    TTKAbstractItemDelegate::paint(painter, option, index);

    if((m_mode & TTKAbstractItemDelegate::TreeMode) && !index.parent().isValid())
    {
        return;
    }

    painter->save();
    m_progress->resize(option.rect.width() - 21, option.rect.height() - 17);
    m_progress->setValue(index.data(TTK_PROGRESS_ROLE).toInt());
    painter->translate(10, 10);

    m_progress->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



TTKLabelItemDelegate::TTKLabelItemDelegate(QObject *parent)
    : TTKAbstractItemDelegate(parent)
{
    m_label  = new QLabel;
    m_label->setAlignment(Qt::AlignCenter);
    m_container = m_label;
}

TTKLabelItemDelegate::~TTKLabelItemDelegate()
{
    delete m_label;
}

void TTKLabelItemDelegate::setAlignment(Qt::Alignment alignment) const
{
    m_label->setAlignment(alignment);
}

void TTKLabelItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    TTKAbstractItemDelegate::paint(painter, option, index);

    if((m_mode & TTKAbstractItemDelegate::TreeMode) && !index.parent().isValid())
    {
        drawDisplay(painter, option, option.rect, index.data(Qt::DisplayRole).toString());
        return;
    }

    painter->save();
    const QColor &color = index.data(Qt::ForegroundRole).value<QColor>();
    if(color.isValid())
    {
        QPalette plt(m_label->palette());
        plt.setColor(QPalette::WindowText, color);
        m_label->setPalette(plt);
    }

    const QString &text = index.data(TTK_DISPLAY_ROLE).toString();
    const QFontMetrics ftm(m_label->font());
    m_label->setText(m_mode & TTKAbstractItemDelegate::ElideMode ? ftm.elidedText(text, Qt::ElideRight, option.rect.width() - 5) : text);
    m_label->resize(option.rect.size());
    painter->translate(0, 0);

    m_label->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



TTKPushButtonItemDelegate::TTKPushButtonItemDelegate(QObject *parent)
    : TTKAbstractItemDelegate(parent)
{
    m_pushButton = new QPushButton;
    m_pushButton->setCursor(QCursor(Qt::PointingHandCursor));
#ifdef Q_OS_UNIX
    m_pushButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_container = m_pushButton;
}

TTKPushButtonItemDelegate::~TTKPushButtonItemDelegate()
{
    delete m_pushButton;
}

void TTKPushButtonItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    TTKAbstractItemDelegate::paint(painter, option, index);

    if((m_mode & TTKAbstractItemDelegate::TreeMode) && !index.parent().isValid())
    {
        drawDisplay(painter, option, option.rect, index.data(Qt::DisplayRole).toString());
        return;
    }

    painter->save();
    m_pushButton->setText(index.data(TTK_DISPLAY_ROLE).toString());
    m_pushButton->resize(option.rect.size() - QSize(10, 10));
    painter->translate(5, 5);

    m_pushButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}
