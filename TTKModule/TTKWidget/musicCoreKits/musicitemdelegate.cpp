#include "musicitemdelegate.h"
#include "musicitemrenameedit.h"
#include "musicwidgetheaders.h"
#include "musicwidgetutils.h"

MusicAbstractDelegate::MusicAbstractDelegate(QObject *parent)
    : QItemDelegate(parent),
      m_mode(MusicAbstractDelegate::Null),
      m_container(nullptr)
{

}

MusicAbstractDelegate::~MusicAbstractDelegate()
{

}

void MusicAbstractDelegate::setStyleSheet(const QString &style) const
{
    if(m_container)
    {
        m_container->setStyleSheet(style);
    }
}

QSize MusicAbstractDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    QSize size = option.rect.size();
    size.setHeight(25);
    return size;
}



MusicCheckBoxDelegate::MusicCheckBoxDelegate(QObject *parent)
    : MusicAbstractDelegate(parent),
      m_background(false)
{
    m_checkBox = new QCheckBox;
    m_checkBox->setStyleSheet(MusicUIObject::MQSSCheckBoxStyle01);
#ifdef Q_OS_UNIX
    m_checkBox->setFocusPolicy(Qt::NoFocus);
#endif
    m_container = m_checkBox;
}

MusicCheckBoxDelegate::~MusicCheckBoxDelegate()
{
    delete m_checkBox;
}

void MusicCheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if !TTK_QT_VERSION_CHECK(5,7,0)
    QItemDelegate::paint(painter, option, index);
#else
    drawBackground(painter, option, index);
#endif

    if((m_mode & MusicAbstractDelegate::TreeMode) && !index.parent().isValid())
    {
        drawDisplay(painter, option, option.rect, index.data(Qt::DisplayRole).toString());
        return;
    }

    painter->save();
    const bool textMode = m_mode & MusicAbstractDelegate::TextMode;
    if(textMode)
    {
        m_checkBox->resize(option.rect.size());
        m_checkBox->setText(index.data(MUSIC_TEXT_ROLE).toString());
        m_checkBox->setEnabled(index.data(MUSIC_ENABLE_ROLE).toBool());
    }
    else
    {
        const int minSize = qMin(option.rect.width(), option.rect.height());
        m_checkBox->resize(minSize, minSize);
    }

    const bool state = m_checkBox->isChecked();
    m_checkBox->setChecked(TTKStatic_cast(Qt::CheckState, index.data(MUSIC_CHECK_ROLE).toInt()) == Qt::Checked);
    if(m_checkBox->isEnabled() && state != m_checkBox->isChecked())
    {
        Q_EMIT TTKConst_cast(MusicCheckBoxDelegate*, this)->buttonChecked();
    }
    painter->translate(textMode ? 0 : (option.rect.width() - 16) / 2, 0);

    m_checkBox->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicProgressBarDelegate::MusicProgressBarDelegate(QObject *parent)
    : MusicAbstractDelegate(parent)
{
    m_progress = new QProgressBar;
    m_progress->setStyleSheet(MusicUIObject::MQSSProgressBar01);
    m_container = m_progress;
}

MusicProgressBarDelegate::~MusicProgressBarDelegate()
{
    delete m_progress;
}

void MusicProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if !TTK_QT_VERSION_CHECK(5,7,0)
    QItemDelegate::paint(painter, option, index);
#else
    drawBackground(painter, option, index);
#endif

    if((m_mode & MusicAbstractDelegate::TreeMode) && !index.parent().isValid())
    {
        return;
    }

    painter->save();
    m_progress->resize(option.rect.width() - 21, option.rect.height() - 17);
    m_progress->setValue(index.data(MUSIC_PROGRESS_ROLE).toInt());
    painter->translate(10, 10);

    m_progress->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicLabelDelegate::MusicLabelDelegate(QObject *parent)
    : MusicAbstractDelegate(parent)
{
    m_label  = new QLabel;
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet(MusicUIObject::MQSSBackgroundStyle13);
    m_container = m_label;
}

MusicLabelDelegate::~MusicLabelDelegate()
{
    delete m_label;
}

void MusicLabelDelegate::setAlignment(Qt::Alignment alignment) const
{
    m_label->setAlignment(alignment);
}

void MusicLabelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if !TTK_QT_VERSION_CHECK(5,7,0)
    QItemDelegate::paint(painter, option, index);
#else
    drawBackground(painter, option, index);
#endif

    if((m_mode & MusicAbstractDelegate::TreeMode) && !index.parent().isValid())
    {
        drawDisplay(painter, option, option.rect, index.data(Qt::DisplayRole).toString());
        return;
    }

    painter->save();
    const QColor &color = index.data(Qt::TextColorRole).value<QColor>();
    if(color.isValid())
    {
        QPalette plt(m_label->palette());
        plt.setColor(QPalette::WindowText, color);
        m_label->setPalette(plt);
    }

    const QString &text = index.data(MUSIC_TEXT_ROLE).toString();
    m_label->setText(m_mode & MusicAbstractDelegate::ElideMode ? MusicUtils::Widget::elidedText(m_label->font(), text, Qt::ElideRight, option.rect.width() - 5) : text);
    m_label->resize(option.rect.size());
    painter->translate(0, 0);

    m_label->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicPushButtonDelegate::MusicPushButtonDelegate(QObject *parent)
    : MusicAbstractDelegate(parent)
{
    m_pushButton = new QPushButton;
    m_pushButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_pushButton->setStyleSheet(MusicUIObject::MQSSBorderStyle03 + MusicUIObject::MQSSBorderStyle06 + MusicUIObject::MQSSBackgroundStyle10);
#ifdef Q_OS_UNIX
    m_pushButton->setFocusPolicy(Qt::NoFocus);
#endif
    m_container = m_pushButton;
}

MusicPushButtonDelegate::~MusicPushButtonDelegate()
{
    delete m_pushButton;
}

void MusicPushButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if !TTK_QT_VERSION_CHECK(5,7,0)
    QItemDelegate::paint(painter, option, index);
#else
    drawBackground(painter, option, index);
#endif

    if((m_mode & MusicAbstractDelegate::TreeMode) && !index.parent().isValid())
    {
        drawDisplay(painter, option, option.rect, index.data(Qt::DisplayRole).toString());
        return;
    }

    painter->save();
    m_pushButton->setText(index.data(MUSIC_TEXT_ROLE).toString());
    m_pushButton->resize(option.rect.size() - QSize(10, 10));
    painter->translate(5, 5);

    m_pushButton->render(painter, option.rect.topLeft(), QRegion(), QWidget::DrawChildren);
    painter->restore();
}



MusicLineEditDelegate::MusicLineEditDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

QWidget *MusicLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    QString text;
    const QAbstractItemModel *model = index.model();
    if(model)
    {
        text = model->data(index, Qt::DisplayRole).toString();
    }

    return new MusicItemRenameEidt(text, parent);
}
